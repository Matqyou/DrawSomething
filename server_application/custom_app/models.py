import random

from django.utils import timezone
from datetime import timedelta
from django.db import models
import uuid


# ============== Example ==============
# class Model:
#  class Enum:
#   OPTION = value, display_name
#
#  field = value
# =====================================
# to update migrations after changing the model
#  > `python manage.py makemigrations`
# to run the migrations and update the database (possible loss of data based on changes)
#  > `python manage.py migrate`

class User(models.Model):
    user_id = models.AutoField(primary_key=True)
    username = models.CharField(max_length=16)
    password = models.CharField(max_length=32)
    display_name = models.CharField(max_length=32)
    email = models.EmailField()
    profile_picture = models.ImageField(upload_to="profile_pictures/", blank=True, null=True)

    games_played = models.IntegerField(default=0)
    rounds_played = models.IntegerField(default=0)
    coins = models.IntegerField(default=0)
    coins_earned = models.IntegerField(default=0)
    coins_spent = models.IntegerField(default=0)
    highest_turn = models.IntegerField(default=0)

    admin_user = models.BooleanField(default=False)

    session_key = models.CharField(max_length=64, null=True, blank=True, unique=True)
    session_expiration = models.DateTimeField(null=True, blank=True)

    creation_date = models.DateTimeField(auto_now_add=True)
    activity_date = models.DateTimeField(auto_now=True)

    def generate_session(self):
        """Generate a new session key and expiration time."""
        self.session_key = uuid.uuid4().hex
        self.session_expiration = timezone.now() + timedelta(hours=1)
        self.save()
        return self.session_key

    def display_fields(self, base_url: str):
        pfp_url = None
        if self.profile_picture:
            pfp_url = base_url + self.profile_picture.url

        return {
            "user_id": self.user_id,
            "username": self.username,
            "display_name": self.display_name,
            "email": self.email,
            "profile_picture": pfp_url,
            "games_played": self.games_played,
            "rounds_played": self.rounds_played,
            "coins": self.coins,
            "coins_earned": self.coins_earned,
            "coins_spent": self.coins_spent,
            "highest_turn": self.highest_turn,
            "admin_user": self.admin_user,
            "creation_date": self.creation_date,
            "activity_date": self.activity_date,
            "logged_in": self.is_logged_in()
        }

    def display_mini(self, base_url: str):
        pfp_url = None
        if self.profile_picture:
            pfp_url = base_url + self.profile_picture.url

        return {
            "user_id": self.user_id,
            "username": self.username,
            "display_name": self.display_name,
            "profile_picture": pfp_url,
            "admin_user": self.admin_user,
            "activity_date": self.activity_date,
            "logged_in": self.is_logged_in()
        }

    def is_logged_in(self) -> bool:
        return self.session_expiration is not None and timezone.now() < self.session_expiration

    def __str__(self):
        return f'{self.user_id}#{self.username}|{self.display_name}|{self.email}|{self.session_key}|{self.session_expiration}'


# Game model for migration to the database
class Game(models.Model):
    # Classification of the current turn
    class CurrentTurn(models.TextChoices):
        PARTY_A = "PARTY_A", "Party A"
        PARTY_B = "PARTY_B", "Party B"

    # Classification of the current game stage
    class TurnStage(models.TextChoices):
        PICKING = "PICKING", "Picking word"
        DRAWING = "DRAWING", "Drawing word"
        GUESSING = "GUESSING", "Guessing word"

    # Classification of the selected word difficulty
    class WordDifficulty(models.TextChoices):
        EASY = "EASY", "Easy"
        MEDIUM = "MEDIUM", "Medium"
        HARD = "HARD", "Hard"

    # Primary / Foreign keys
    game_id = models.AutoField(primary_key=True)
    creator = models.ForeignKey('User', on_delete=models.CASCADE, related_name='created_games')

    party_a = models.ForeignKey('User', on_delete=models.CASCADE, related_name='games_as_a')
    coins_a = models.IntegerField(default=0)
    party_b = models.ForeignKey('User', on_delete=models.CASCADE, related_name='games_as_b')
    coins_b = models.IntegerField(default=0)

    # Game words and difficulty
    word = models.CharField(max_length=32, default="")
    difficulty = models.CharField(choices=WordDifficulty.choices, default=WordDifficulty.EASY, max_length=16)

    choice_easy = models.CharField(max_length=32, default="")
    choice_medium = models.CharField(max_length=32, default="")
    choice_hard = models.CharField(max_length=32, default="")

    # Game statiostics and state
    turn_number = models.IntegerField(default=1)
    current_turn = models.CharField(choices=CurrentTurn.choices, default=CurrentTurn.PARTY_A, max_length=16)
    turn_stage = models.CharField(choices=TurnStage.choices, default=TurnStage.PICKING, max_length=16)
    active = models.BooleanField(default=True)

    activity_date = models.DateTimeField(auto_now=True)
    creation_date = models.DateTimeField(auto_now_add=True)

    # Return for players of the game
    def display_fields(self, base_url: str):
        return {
            "game_id": self.game_id,
            "creator_id": self.creator_id,
            "party_a": self.party_a.display_mini(base_url),
            "party_b": self.party_b.display_mini(base_url),
            "word": self.word,
            "difficulty": self.difficulty,
            "choice_easy": self.choice_easy,
            "choice_medium": self.choice_medium,
            "choice_hard": self.choice_hard,
            "turn_number": self.turn_number,
            "current_turn": self.current_turn,
            "turn_stage": self.turn_stage,
            "active": self.active,
            "activity_date": self.activity_date,
            "creation_date": self.creation_date,
        }

    # Return for the main menu games listing
    def display_mini(self, base_url: str):
        return {
            "game_id": self.game_id,
            "creator_id": self.creator_id,
            "party_a": self.party_a.display_mini(base_url),
            "party_b": self.party_b.display_mini(base_url),
            "turn_number": self.turn_number,
            "current_turn": self.current_turn,
            "turn_stage": self.turn_stage,
            "active": self.active,
            "activity_date": self.activity_date,
            "creation_date": self.creation_date,
        }

    # Pick new words to be chosen in the next round
    def refresh_choices(self):
        # Use words file that contains many options
        with open('words', 'r') as f:
            words = f.read().splitlines(keepends=False)

        self.choice_easy = random.choice(words)
        self.choice_medium = random.choice(words)
        self.choice_hard = random.choice(words)
        self.save()


class Recording(models.Model):
    recording_id = models.AutoField(primary_key=True)
    game = models.ForeignKey(Game, on_delete=models.CASCADE, related_name='recordings')
    file_path = models.CharField(max_length=256, null=True)
    turn_number = models.IntegerField(default=0)
    drawn_word = models.CharField(max_length=32)
    word_difficulty = models.CharField(max_length=16)

    created_at = models.DateTimeField(auto_now_add=True)

    # description = models.TextField(blank=True, null=True)  # Optional: small description

    def display_mini(self):
        return {
            "recording_id": self.recording_id,
            "game_id": self.game_id,
            "file_path": self.file_path,
            "turn_number": self.turn_number,
            "drawn_word": self.drawn_word,
            "word_difficulty": self.word_difficulty,
        }


class ColorBundle(models.Model):
    bundle_id = models.AutoField(primary_key=True)
    rarity = models.IntegerField(default=0)
    name = models.CharField(max_length=64)
    price = models.IntegerField(default=999999)

    def display_fields(self):
        return {
            "bundle_id": self.bundle_id,
            "rarity": self.rarity,
            "name": self.name,
            "price": self.price,
            "colors": [color.display_fields() for color in self.colors.all()]
        }


class Color(models.Model):
    color_id = models.AutoField(primary_key=True)
    bundle = models.ForeignKey(ColorBundle, on_delete=models.CASCADE, related_name='colors')

    rarity = models.IntegerField(default=0)
    r = models.IntegerField(default=0)
    g = models.IntegerField(default=0)
    b = models.IntegerField(default=0)

    def display_fields(self):
        return {
            "color_id": self.color_id,
            "rarity": self.rarity,
            "r": self.r,
            "g": self.g,
            "b": self.b
        }