import random
from django.utils import timezone
from datetime import timedelta
from django.contrib.auth.hashers import make_password, check_password
from .decorators import authed_user, authed_admin, allowed_methods
from .rules import valid_nickname, valid_username, valid_password, valid_email
from custom_app.models import User, Game, Recording, ColorBundle, UserColorBundle, Color
from django.http import JsonResponse
from django.db.models import Count, Q, Case, When, IntegerField, Avg, Max, Sum
import json


# import os


def get_base_url(request):
    return request.build_absolute_uri('/').rstrip('/')


@allowed_methods(['GET'])
@authed_user
def stats(request):
    user: User = request.user  # type: ignore
    base_url = get_base_url(request)
    return JsonResponse({"code": "stats", "stats": user.display_fields(base_url)}, status=200)


@allowed_methods(['POST'])
@authed_user
def logout(request):
    user: User = request.user  # type: ignore

    # Clear session data (effectively logging the user out)
    user.session_key = None
    user.session_expiration = None
    user.save()

    return JsonResponse({"message": "Logged out successfully!"}, status=200)


@allowed_methods(['POST'])
def login(request):
    try:
        data = json.loads(request.body)
        username = data.get("username")
        password = data.get("password")

        if not username:
            return JsonResponse({"code": "login_fail", "message": "Please enter your username"}, status=401)

        username = username.lower()

        if not password:
            return JsonResponse({"code": "login_fail", "message": "Please enter your password"}, status=401)

        user: User = User.objects.filter(username=username).first()
        if not user or not check_password(password, user.password) or username == 'deleted':
            return JsonResponse({"code": "login_fail", "message": "Incorrect username or password"}, status=401)

        user_id = user.user_id

        # Generate a session key
        session_key = user.generate_session()

        base_url = get_base_url(request)
        games = [game.display_mini(base_url) for game in
                 Game.objects.filter((Q(party_a=user_id) | Q(party_b=user_id)) & Q(active=True))]

        return JsonResponse({
            "code": "login_success",
            "message": f"Welcome, {user.display_name}!",
            "session_key": session_key,
            "expires_at": user.session_expiration.isoformat(),

            "user": user.display_fields(base_url),
            "games": games,
        }, status=200)

    except Exception as e:
        return JsonResponse({"error": str(e)}, status=400)


@allowed_methods(['POST'])
def register(request):
    try:
        # Parse JSON request body
        data = json.loads(request.body)

        username = data.get("username")
        password = data.get("password")
        confirmation = data.get("confirmation")
        display_name = data.get("display_name")
        email = data.get("email")

        if not username:
            return JsonResponse({"code": "register_fail", "message": f"Please enter an username"}, status=400)

        valid, error = valid_username(username)
        if not valid:
            return JsonResponse({"code": "register_fail", "message": error}, status=400)

        username = username.lower()

        if User.objects.filter(username=username).exists():
            return JsonResponse({"code": "register_fail", "message": f"Username \"{username}\" is already in use!"},
                                status=400)

        if not display_name:
            return JsonResponse({"code": "register_fail", "message": f"Please enter a nickname"}, status=400)

        if not email:
            return JsonResponse({"code": "register_fail", "message": f"Please enter your email"}, status=400)

        valid, error = valid_email(email)
        if not valid:
            return JsonResponse({"code": "register_fail", "message": error}, status=400)

        if not password:
            return JsonResponse({"code": "register_fail", "message": f"Please enter a password"}, status=400)

        valid, error = valid_password(password)
        if not valid:
            return JsonResponse({"code": "register_fail", "message": error}, status=400)

        if not confirmation:
            return JsonResponse({"code": "register_fail", "message": f"Please enter your password again"}, status=400)

        if password != confirmation:
            return JsonResponse({"code": "register_fail", "message": f"The passwords do not match"}, status=400)

        # Create new user
        user = User.objects.create(
            username=username,
            password=make_password(password),  # Hash password before storing
            display_name=display_name,
            email=email,
        )
        if user.user_id == 1:
            user.admin_user = True
            user.coins += 499
            user.bombs += 99
            user.save()

        try:
            first_bundle = ColorBundle.objects.get(bundle_id=1)
            UserColorBundle.objects.create(
                user=user,
                color_bundle=first_bundle,
                purchase_price=0  # Free starter bundle
            )
        except ColorBundle.DoesNotExist:
            pass

        return JsonResponse({"code": "register_success", "message": "Registration complete!"}, status=201)
    except Exception as e:
        return JsonResponse({"error": str(e)}, status=400)


@allowed_methods(['GET'])
@authed_admin
def all_users(request):
    users = User.objects.all()
    base_url = get_base_url(request)
    return JsonResponse({"code": "all_users", "users": [user.display_fields(base_url) for user in users]}, status=200)


@allowed_methods(['POST'])
@authed_user
def upload_profile_picture(request):
    if request.method == 'POST':
        user: User = request.user  # type: ignore
        user.profile_picture = request.FILES['picture']
        user.save()
        return JsonResponse({"message": "Profile picture updated successfully"}, status=200)


@allowed_methods(['POST'])
@authed_user
def delete_profile(request):
    user: User = request.user  # type: ignore
    username = user.username

    user.username = "deleted"
    user.password = make_password("deleted")
    user.display_name = "Deleted User"
    user.email = ""
    user.profile_picture = None

    user.session_key = None
    user.session_expiration = None

    user.deleted = True
    user.save()

    return JsonResponse({"message": f"User `{username}` deleted successfully"}, status=200)


@allowed_methods(['POST'])
@authed_user
def buy_colors(request):
    user: User = request.user  # type: ignore

    try:
        # Parse JSON request body
        data = json.loads(request.body)

        bundle_id = data.get("bundle_id")

        if not bundle_id:
            return JsonResponse({"message": "No bundle_id supplied"}, status=400)

        try:
            bundle = ColorBundle.objects.get(bundle_id=bundle_id)
        except ColorBundle.DoesNotExist:
            return JsonResponse({"message": f"Color bundle #{bundle_id} was not found"}, status=404)

        # Check if user already owns this bundle
        if UserColorBundle.objects.filter(user=user, color_bundle=bundle).exists():
            return JsonResponse({"message": "You already own this color bundle"}, status=400)

        if user.coins < bundle.price:
            return JsonResponse({"message": "You don't have enough coins to buy this"}, status=400)

        # Process the purchase
        user.coins -= bundle.price
        user.coins_spent += bundle.price
        user.save()

        # Create the purchase record
        UserColorBundle.objects.create(
            user=user,
            color_bundle=bundle,
            purchase_price=bundle.price
        )

        base_url = get_base_url(request)
        return JsonResponse({
            "message": "Color bundle has been purchased",
            "user": user.display_fields(base_url)
        }, status=201)
    except Exception as e:
        return JsonResponse({"error": str(e)}, status=400)


@allowed_methods(['POST'])
@authed_user
def buy_bombs(request):
    user: User = request.user  # type: ignore

    try:
        # Parse JSON request body
        data = json.loads(request.body)

        bombs_id = data.get("bombs_id")

        if bombs_id is False:
            return JsonResponse({"message": "No bombs_id supplied"}, status=400)

        if not (0 <= bombs_id <= 2):
            return JsonResponse({"message": f"Bombs offer {bombs_id} was not found"}, status=404)

        price = (5, 12, 30)[bombs_id]
        bombs = (1, 3, 10)[bombs_id]

        if user.coins < price:
            return JsonResponse({"message": "You don't have enough coins to buy this"}, status=400)

        # Process the purchase
        user.coins -= price
        user.coins_spent += price
        user.bombs += bombs
        user.save()

        base_url = get_base_url(request)
        return JsonResponse({
            "message": "Bomb/s have been purchased",
            "user": user.display_fields(base_url)
        }, status=201)
    except Exception as e:
        return JsonResponse({"error": str(e)}, status=400)


@allowed_methods(['POST'])
@authed_user
def edit_profile(request):
    user: User = request.user  # type: ignore

    try:
        # Parse JSON request body
        data = json.loads(request.body)

        username = data.get("username")
        password = data.get("password")
        confirmation = data.get("confirmation")
        display_name = data.get("display_name")
        email = data.get("email")

        # Track if any changes were made
        changes_made = False

        if display_name:
            valid, error = valid_nickname(display_name)
            if not valid:
                return JsonResponse({"message": error}, status=400)

        if username:
            valid, error = valid_username(username)
            if not valid:
                return JsonResponse({"message": error}, status=400)

        if username != user.username and User.objects.filter(username=username).exists():
            return JsonResponse({"message": f"Username \"{username}\" is already in use!"}, status=400)

        if email:
            valid, error = valid_email(email)
            if not valid:
                return JsonResponse({"message": error}, status=400)

        if password or confirmation:
            valid, error = valid_password(password)
            if not valid:
                return JsonResponse({"message": error}, status=400)

            if not confirmation:
                return JsonResponse({"message": f"Please enter your password again"}, status=400)

            if password != confirmation:
                return JsonResponse({"message": f"The passwords do not match"}, status=400)

        # Check and update fields only if they're different
        if username and username != user.username:
            user.username = username
            changes_made = True
        if password and password != user.password:
            user.password = password
            changes_made = True
        if email and email != user.email:
            user.email = email
            changes_made = True
        if display_name and display_name != user.display_name:
            user.display_name = display_name
            changes_made = True

        if not changes_made:
            return JsonResponse({
                "message": "No changes were made"
            }, status=200)

        user.save()

        base_url = get_base_url(request)
        return JsonResponse({
            "message": "Profile updated successfully!",
            "user": user.display_fields(base_url)
        }, status=201)
    except Exception as e:
        return JsonResponse({"error": str(e)}, status=400)


@allowed_methods(['POST'])
@authed_admin
def modify_user(request, user_id):
    try:
        user = User.objects.get(user_id=user_id)
        data = json.loads(request.body)

        # Define a list of fields that should not be modifiable
        restricted_fields = ['user_id', 'password', 'session_key', 'admin_user']

        # Update fields only if they are provided in the request and are not in the restricted fields
        for field, value in data.items():
            if hasattr(user,
                       field) and field not in restricted_fields:  # Check if the field exists and is not restricted
                setattr(user, field, value)  # Update the field with the new value
            elif field in restricted_fields:
                # Optionally, log or raise an error if an attempt is made to modify a restricted field
                continue  # Skip the restricted field

        # Save the updated user instance
        user.save()

        return JsonResponse({"code": "updated_user_success", "message": "User updated successfully"}, status=200)
    except Exception as e:
        return JsonResponse({"error": str(e)}, status=400)


@allowed_methods(['DELETE'])
@authed_admin
def delete_user(request, user_id):
    try:
        user = User.objects.get(user_id=user_id)
        user.delete()
        return JsonResponse({"code": "deleted_user_success", "message": f"User {user_id} deleted successfully"},
                            status=200)
    except User.DoesNotExist:
        return JsonResponse({"error": "user.not_found"}, status=404)


@allowed_methods(['GET'])
def ping(request):
    return JsonResponse({"code": "pong"}, status=200)


@allowed_methods(['GET'])
def default(request):
    return JsonResponse({"code": "default", "message": "DrawSomething server"}, safe=False)


# Games
@allowed_methods(['POST'])
@authed_user
def create_game(request):
    user: User = request.user  # type: ignore

    try:
        # Step 1: Get candidates (non-deleted, not self)
        candidate_users = User.objects.filter(deleted=False).exclude(user_id=user.user_id)

        eligible_users = []
        for candidate in candidate_users:
            # Count active games
            active_game_count = Game.objects.filter(
                Q(active=True) & (Q(party_a=candidate) | Q(party_b=candidate))
            ).count()

            if active_game_count < 5:
                eligible_users.append(candidate)

        if not eligible_users:
            return JsonResponse({
                "code": "game_create_fail",
                "message": "No opponents could be found at this time"
            }, status=404)

        # Step 2: Pick random opponent
        random_user = random.choice(eligible_users)

        # Step 3: Randomly assign parties
        players = [user, random_user]
        random.shuffle(players)
        party_a, party_b = players

        # Step 4: Create game
        game = Game.objects.create(
            creator=user,
            party_a=party_a,
            party_b=party_b,
        )
        game.refresh_choices()

        return JsonResponse({
            "code": "game_create_success",
            "message": f"Game #{game.game_id} has been created",
            "game": game.display_mini(get_base_url(request))
        }, status=200)

    except Exception as e:
        return JsonResponse({"error": str(e)}, status=400)


@allowed_methods(['POST'])
@authed_user
def delete_game(request):
    # Get the use issuing the request
    user: User = request.user  # type: ignore
    user_id = user.user_id

    try:
        # Parse JSON request body
        data = json.loads(request.body)
        game_id = data.get("game_id")

        # Check if game_id was supplied
        if game_id is None:
            return JsonResponse({"code": "game_delete_fail", "message": f"No game_id provided"}, status=400)

        try:
            # Attempt to find the game
            game = Game.objects.get(game_id=game_id)
        except Game.DoesNotExist:
            return JsonResponse({"code": "game_delete_fail", "message": f"Game #{game_id} was not found"}, status=404)

        # Check if the user is a player of said game
        if user_id not in [game.party_a_id, game.party_b_id]:
            return JsonResponse({"code": "game_delete_fail", "message": f"You are not involved in game #{game_id}"},
                                status=400)

        game.active = False
        game.save()
        return JsonResponse(
            {"code": "game_delete_success", "message": f"Game has been deleted", "game_id": game_id},
            status=200)
    except Exception as e:
        return JsonResponse({"error": str(e)}, status=400)


@allowed_methods(['GET'])
@authed_user
def get_games(request):
    user: User = request.user  # type: ignore

    try:
        base_url = get_base_url(request)
        games = [game.display_mini(base_url) for game in
                 Game.objects.filter((Q(party_a=user) | Q(party_b=user)) & Q(active=True))]

        return JsonResponse({
            "code": "games_get_success",
            "user": user.display_fields(base_url),
            "games": games,
        }, status=200)
    except Exception as e:
        return JsonResponse({"error": str(e)}, status=400)


@allowed_methods(['GET'])
@authed_admin
def all_game_stats(request):
    # User Statistics
    total_users = User.objects.filter(deleted=False).count()  # Only non-deleted users
    deleted_users = User.objects.filter(deleted=True).count()
    active_users_24h = User.objects.filter(deleted=False,
                                           activity_date__gte=timezone.now() - timedelta(hours=24)).count()
    active_users_7d = User.objects.filter(deleted=False, activity_date__gte=timezone.now() - timedelta(days=7)).count()
    new_users_24h = User.objects.filter(deleted=False, creation_date__gte=timezone.now() - timedelta(hours=24)).count()
    new_users_7d = User.objects.filter(deleted=False, creation_date__gte=timezone.now() - timedelta(days=7)).count()

    # Game Statistics
    total_games = Game.objects.count()
    active_games = Game.objects.filter(active=True).count()
    completed_games = Game.objects.filter(active=False).count()
    games_created_24h = Game.objects.filter(creation_date__gte=timezone.now() - timedelta(hours=24)).count()
    games_created_7d = Game.objects.filter(creation_date__gte=timezone.now() - timedelta(days=7)).count()

    # Game turn statistics
    avg_turns_per_game = Game.objects.aggregate(avg_turns=Avg('turn_number'))['avg_turns'] or 0
    max_turns_ever = Game.objects.aggregate(max_turns=Max('turn_number'))['max_turns'] or 0
    max_turns_active = Game.objects.filter(active=True).aggregate(max_turns=Max('turn_number'))['max_turns'] or 0

    # Economy Statistics
    total_coins_in_circulation = User.objects.filter(deleted=False).aggregate(total_coins=Sum('coins'))[
                                     'total_coins'] or 0
    total_coins_earned = User.objects.filter(deleted=False).aggregate(total_earned=Sum('coins_earned'))[
                             'total_earned'] or 0
    total_coins_spent = User.objects.filter(deleted=False).aggregate(total_spent=Sum('coins_spent'))['total_spent'] or 0
    avg_coins_per_user = User.objects.filter(deleted=False).aggregate(avg_coins=Avg('coins'))['avg_coins'] or 0

    # Color Bundle Statistics
    total_bundles = ColorBundle.objects.count()
    total_bundle_purchases = UserColorBundle.objects.count()
    bundle_purchases_24h = UserColorBundle.objects.filter(
        purchase_date__gte=timezone.now() - timedelta(hours=24)).count()
    bundle_purchases_7d = UserColorBundle.objects.filter(purchase_date__gte=timezone.now() - timedelta(days=7)).count()

    # Most popular bundles
    popular_bundles = (ColorBundle.objects
                       .annotate(purchase_count=Count('purchased_by'))
                       .order_by('-purchase_count')[:5]
                       .values('bundle_id', 'name', 'price', 'purchase_count'))

    # Revenue from bundle sales
    total_bundle_revenue = UserColorBundle.objects.aggregate(total_revenue=Sum('purchase_price'))['total_revenue'] or 0

    # Player engagement stats
    total_games_played = User.objects.filter(deleted=False).aggregate(total_games=Sum('games_played'))[
                             'total_games'] or 0
    total_rounds_played = User.objects.filter(deleted=False).aggregate(total_rounds=Sum('rounds_played'))[
                              'total_rounds'] or 0
    avg_games_per_user = User.objects.filter(deleted=False).aggregate(avg_games=Avg('games_played'))['avg_games'] or 0
    avg_rounds_per_user = User.objects.filter(deleted=False).aggregate(avg_rounds=Avg('rounds_played'))[
                              'avg_rounds'] or 0

    # Top players (only non-deleted users)
    top_players_by_games = list(
        User.objects.filter(deleted=False).order_by('-games_played')[:5].values('username', 'display_name',
                                                                                'games_played', 'coins'))
    top_players_by_coins = list(
        User.objects.filter(deleted=False).order_by('-coins')[:5].values('username', 'display_name', 'coins',
                                                                         'games_played'))
    top_players_by_turns = list(
        User.objects.filter(deleted=False).order_by('-highest_turn')[:5].values('username', 'display_name',
                                                                                'highest_turn', 'games_played'))

    return JsonResponse({
        "user_stats": {
            "total_users": total_users,
            "deleted_users": deleted_users,
            "active_users_24h": active_users_24h,
            "active_users_7d": active_users_7d,
            "new_users_24h": new_users_24h,
            "new_users_7d": new_users_7d
        },
        "game_stats": {
            "total_games": total_games,
            "active_games": active_games,
            "completed_games": completed_games,
            "games_created_24h": games_created_24h,
            "games_created_7d": games_created_7d,
            "avg_turns_per_game": round(avg_turns_per_game, 2),
            "max_turns_ever": max_turns_ever,
            "max_turns_active": max_turns_active
        },
        "economy_stats": {
            "total_coins_in_circulation": total_coins_in_circulation,
            "total_coins_earned": total_coins_earned,
            "total_coins_spent": total_coins_spent,
            "avg_coins_per_user": round(avg_coins_per_user, 2),
            "total_bundle_revenue": total_bundle_revenue
        },
        "bundle_stats": {
            "total_bundles": total_bundles,
            "total_purchases": total_bundle_purchases,
            "purchases_24h": bundle_purchases_24h,
            "purchases_7d": bundle_purchases_7d,
            "popular_bundles": list(popular_bundles)
        },
        "engagement_stats": {
            "total_games_played": total_games_played,
            "total_rounds_played": total_rounds_played,
            "avg_games_per_user": round(avg_games_per_user, 2),
            "avg_rounds_per_user": round(avg_rounds_per_user, 2)
        },
        "leaderboards": {
            "top_players_by_games": top_players_by_games,
            "top_players_by_coins": top_players_by_coins,
            "top_players_by_turns": top_players_by_turns
        }
    }, status=200)


@allowed_methods(['POST'])
@authed_user
def start_game(request):
    user: User = request.user  # type: ignore
    user_id = user.user_id

    try:
        # Parse JSON request body
        data = json.loads(request.body)
        game_id = data.get("game_id")
        if not game_id:
            return JsonResponse({"code": "game_start_fail", "message": f"No game_id provided"}, status=400)

        try:
            game = Game.objects.get(game_id=game_id)
        except Game.DoesNotExist:
            return JsonResponse({"code": "game_start_fail", "message": f"Game #{game_id} was not found"}, status=404)

        if user_id not in [game.party_a_id, game.party_b_id]:
            return JsonResponse({"code": "game_delete_fail", "message": f"You are not involved in game #{game_id}"},
                                status=400)

        base_url = get_base_url(request)
        response_data = {
            "code": "game_start_success",
            "message": f"You have started the game #{game_id}",
            "game": game.display_fields(base_url),
        }

        # Append drawing recording if its guessing stage
        if game.turn_stage == Game.TurnStage.GUESSING:
            attempt = game.current_attempt
            turn_number = game.turn_number
            any_existing_recordings = Recording.objects.filter(game_id=game_id, turn_number=turn_number,
                                                               attempt=attempt)

            if any_existing_recordings.count() == 0:
                return JsonResponse({"code": "game_start_fail",
                                     "message": f"No recording found for Game #{game_id} turn {turn_number}"},
                                    status=404)

            if any_existing_recordings.count() > 1:
                return JsonResponse({"code": "game_start_fail",
                                     "message": f"Multiple recordings({any_existing_recordings.count()}) found for Game #{game_id} turn {turn_number}"},
                                    status=404)

            recording = any_existing_recordings.first()
            recording_filepath = recording.file_path

            with open(recording_filepath, "r") as f:
                recording_data = f.read()

            response_data["guessing_recording"] = recording_data
        elif game.turn_stage == Game.TurnStage.DRAWING:
            owned_colors = [color.display_fields() for color in Color.objects.filter(bundle__purchased_by__user=user)]
            response_data["owned_colors"] = owned_colors

        return JsonResponse(response_data, status=200)
    except Exception as e:
        return JsonResponse({"error": str(e)}, status=400)


@allowed_methods(['POST'])
@authed_user
def pick_word(request):
    user: User = request.user  # type: ignore
    user_id = user.user_id

    try:
        # Parse JSON request body
        data = json.loads(request.body)
        game_id = data.get("game_id")
        picked_word = data.get("picked_word")
        picked_difficulty = data.get("picked_difficulty")

        if not game_id:
            return JsonResponse({"code": "pick_word_fail", "message": f"No game_id provided"}, status=400)

        try:
            game = Game.objects.get(game_id=game_id)
        except Game.DoesNotExist:
            return JsonResponse({"code": "pick_word_fail", "message": f"Game #{game_id} was not found"},
                                status=404)

        if user_id not in [game.party_a_id, game.party_b_id]:
            return JsonResponse({"code": "pick_word_fail", "message": f"You are not involved in game #{game_id}"},
                                status=400)

        if game.turn_stage != Game.TurnStage.PICKING:
            return JsonResponse({"code": "pick_word_fail",
                                 "message": f"Game #{game_id} is not in the word picking stage, stage: {game.turn_stage}"},
                                status=400)

        if not picked_word:
            return JsonResponse({"code": "pick_word_fail",
                                 "message": f"No picked_word provided, options: {[game.choice_easy, game.choice_medium, game.choice_hard]}"},
                                status=400)

        if picked_word not in [game.choice_easy, game.choice_medium, game.choice_hard]:
            return JsonResponse({"code": "pick_word_fail",
                                 "message": f"Selected word is not in the allowed options of game #{game_id}: {[game.choice_easy, game.choice_medium, game.choice_hard]}"},
                                status=400)

        # Might be redundant
        if not picked_difficulty:
            return JsonResponse(
                {"code": "pick_word_fail", "message": f"No picked_difficulty provided, options: [EASY, MEDIUM, HARD]"},
                status=400)

        if not picked_difficulty in ['EASY', 'MEDIUM', 'HARD']:
            return JsonResponse({"code": "pick_word_fail",
                                 "message": f"Selected word `{picked_word}, {picked_difficulty}` difficulty does not match any "
                                            f"of game #{game_id} `{[game.choice_easy, game.choice_medium, game.choice_hard]}, [EASY, MEDIUM, HARD]`"},
                                status=400)
        #

        game.word = picked_word.upper()
        game.difficulty = picked_difficulty
        game.turn_stage = Game.TurnStage.DRAWING
        game.save()

        owned_colors = [color.display_fields() for color in Color.objects.filter(bundle__purchased_by__user=user)]

        base_url = get_base_url(request)
        return JsonResponse({
            "code": "pick_word_success",
            "message": f"Picked the word `{picked_word}, {picked_difficulty}`",
            "game": game.display_fields(base_url),
            "owned_colors": owned_colors,
        }, status=200)
    except Exception as e:
        return JsonResponse({"error": str(e)}, status=400)


@allowed_methods(['POST'])
@authed_user
def finished_drawing(request):
    user: User = request.user  # type: ignore
    user_id = user.user_id

    try:
        # Parse JSON request body
        data = json.loads(request.body)
        game_id = data.get("game_id")
        recording_data = data.get("recording_data")

        if not game_id:
            return JsonResponse({"code": "finish_drawing_fail", "message": f"No game_id provided"}, status=400)

        try:
            game = Game.objects.get(game_id=game_id)
        except Game.DoesNotExist:
            return JsonResponse({"code": "finish_drawing_fail", "message": f"Game #{game_id} was not found"},
                                status=404)

        if user_id not in [game.party_a_id, game.party_b_id]:
            return JsonResponse({"code": "finish_drawing_fail", "message": f"You are not involved in game #{game_id}"},
                                status=400)

        if game.turn_stage != Game.TurnStage.DRAWING:
            return JsonResponse({"code": "finish_drawing_fail",
                                 "message": f"Game #{game_id} is not in the drawing stage, stage: {game.turn_stage}"})

        attempt = game.current_attempt
        turn_number = game.turn_number
        try:
            any_existing_recordings = Recording.objects.filter(game_id=game_id, turn_number=turn_number,
                                                               attempt=attempt)
            if any_existing_recordings:
                return JsonResponse({"code": "finish_drawing_fail",
                                     "message": f"Game #{game_id} recording {turn_number} is already submitted"},
                                    status=404)
        except Recording.DoesNotExist:
            pass

        if not recording_data:
            return JsonResponse({"code": "finish_drawing_fail", "message": f"No recording_data provided"}, status=400)

        recording = Recording.objects.create(
            game_id=game_id,
            file_path=None,
            turn_number=game.turn_number,
            attempt=game.current_attempt,
            drawn_word=game.word,
            word_difficulty=game.difficulty,
        )
        file_path = f'recordings/{recording.recording_id}'
        recording.file_path = file_path
        recording.save()

        game.turn_stage = Game.TurnStage.GUESSING
        game.current_turn = (Game.CurrentTurn.PARTY_A, Game.CurrentTurn.PARTY_B)[
            game.current_turn == Game.CurrentTurn.PARTY_A]
        game.save()

        with open(file_path, 'w') as f:
            f.write(recording_data)

        base_url = get_base_url(request)
        return JsonResponse({
            "code": "finish_drawing_success",
            "message": f"Successfully uploaded recording data",
            "game": game.display_fields(base_url),
        }, status=200)
    except Exception as e:
        return JsonResponse({"error": str(e)}, status=400)


@allowed_methods(['POST'])
@authed_user
def guess_word(request):
    user: User = request.user  # type: ignore
    user_id = user.user_id

    try:
        # Parse JSON request body
        data = json.loads(request.body)
        game_id = data.get("game_id")
        guess = data.get("guess")

        if not game_id:
            return JsonResponse({"code": "guess_fail", "message": f"No game_id provided"}, status=400)

        try:
            game = Game.objects.get(game_id=game_id)
        except Game.DoesNotExist:
            return JsonResponse({"code": "guess_fail", "message": f"Game #{game_id} was not found"},
                                status=404)

        if user_id not in [game.party_a_id, game.party_b_id]:
            return JsonResponse({"code": "guess_fail", "message": f"You are not involved in game #{game_id}"},
                                status=400)

        if game.turn_stage != Game.TurnStage.GUESSING:
            return JsonResponse({"code": "guess_fail",
                                 "message": f"Game #{game_id} is not in the guessing stage, stage: {game.turn_stage}"},
                                status=400)

        if game.word != guess:
            return JsonResponse({"code": "guess_fail",
                                 "message": f"Game #{game_id} the guess {guess} is incorrect"},
                                status=400)

        next_turn = game.turn_number + 1
        coins_reward = (3, (2, 1)[game.difficulty != 'MEDIUM'])[game.difficulty != 'HARD']

        other_user_id = (game.party_a_id, game.party_b_id)[game.party_a_id == user.user_id]
        try:
            other_user = User.objects.get(user_id=other_user_id)
        except User.DoesNotExist:
            return JsonResponse({"code": "guess_fail",
                                 "message": f"Game #{game_id} other player(#{other_user_id}) was not found"},
                                status=400)

        user.coins_earned += coins_reward
        user.coins += coins_reward
        user.rounds_played += 1
        if game.turn_number == 1:
            user.games_played += 1
        if next_turn > user.highest_turn:
            user.highest_turn = next_turn
        user.save()
        # add coins to other party too
        other_user.coins_earned += coins_reward
        other_user.coins += coins_reward
        other_user.rounds_played += 1
        if game.turn_number == 1:
            other_user.games_played += 1
        if next_turn > other_user.highest_turn:
            other_user.highest_turn = next_turn
        other_user.save()

        game.turn_stage = Game.TurnStage.PICKING
        game.turn_number = next_turn
        game.refresh_choices()

        base_url = get_base_url(request)
        return JsonResponse({
            "code": "guess_success",
            "message": f"You guessed the word, it was `{guess}`",
            "user": user.display_fields(base_url),
            "game": game.display_fields(base_url),
        }, status=200)
    except Exception as e:
        return JsonResponse({"error": str(e)}, status=400)


@allowed_methods(['POST'])
@authed_user
def give_up_guessing(request):
    user: User = request.user  # type: ignore
    user_id = user.user_id

    try:
        # Parse JSON request body
        data = json.loads(request.body)
        game_id = data.get("game_id")

        if not game_id:
            return JsonResponse({"code": "guess_fail", "message": f"No game_id provided"}, status=400)

        try:
            game = Game.objects.get(game_id=game_id)
        except Game.DoesNotExist:
            return JsonResponse({"code": "guess_fail", "message": f"Game #{game_id} was not found"},
                                status=404)

        if user_id not in [game.party_a_id, game.party_b_id]:
            return JsonResponse({"code": "guess_fail", "message": f"You are not involved in game #{game_id}"},
                                status=400)

        if game.turn_stage != Game.TurnStage.GUESSING:
            return JsonResponse({"code": "guess_fail",
                                 "message": f"Game #{game_id} is not in the guessing stage, stage: {game.turn_stage}"},
                                status=400)

        game.current_attempt += 1
        game.turn_number = 1
        game.turn_stage = Game.TurnStage.PICKING
        game.refresh_choices()

        base_url = get_base_url(request)
        return JsonResponse({
            "message": f"You gave up, the word was `{game.word}`",
            "user": user.display_fields(base_url),
            "game": game.display_fields(base_url),
        }, status=200)
    except Exception as e:
        return JsonResponse({"error": str(e)}, status=400)


@allowed_methods(['GET'])
@authed_user
def get_shop(request):
    try:
        bundles = [bundle.display_fields() for bundle in ColorBundle.objects.all()]

        return JsonResponse({
            "code": "get_shop_success",
            "bundles": bundles,
        }, status=200)
    except Exception as e:
        return JsonResponse({"error": str(e)}, status=400)
