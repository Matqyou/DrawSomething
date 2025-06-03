from django.utils.timezone import now
from django.http import JsonResponse
from custom_app.models import User
from django.utils import timezone
from functools import wraps


def allowed_methods(methods):
    def decorator(func):
        @wraps(func)
        def wrapper(request, *args, **kwargs):
            if request.method not in methods:
                return JsonResponse({"error": "Invalid request method"}, status=405)
            return func(request, *args, **kwargs)
        return wrapper
    return decorator

def authed_user(view_func):
    """Decorator to ensure the request has a valid session key."""

    @wraps(view_func)
    def wrapper(request, *args, **kwargs):
        auth = request.headers.get("Authorization")
        if not auth:
            return JsonResponse({"error": "Missing session key"}, status=401)

        auth_split = auth.split()
        if len(auth_split) < 2 or not auth_split[1]:
            return JsonResponse({"error": "Missing session key"}, status=401)

        key_type, session_key = auth_split
        user: User = User.objects.filter(session_key=session_key).first()

        if not user:
            return JsonResponse({"error": f"Invalid session"}, status=401)

        if user.session_expiration < timezone.now():
            return JsonResponse({"error": f"Session expired {user.session_expiration}"}, status=401)

        user.activity_date = now()
        user.save()

        request.user = user
        return view_func(request, *args, **kwargs)
    return wrapper


def authed_admin(view_func):
    """Decorator to ensure the request is from an authenticated admin user."""

    @wraps(view_func)
    def wrapper(request, *args, **kwargs):
        # Use the existing authed_user decorator to authenticate the user first
        response = authed_user(view_func)(request, *args, **kwargs)

        if isinstance(response, JsonResponse) and response.status_code != 200:
            # If authed_user failed, return that response
            return response

        # After successful authentication, check if the user is an admin
        if not request.user.admin_user:
            return JsonResponse({"error": "No administrator permission"}, status=403)
        return view_func(request, *args, **kwargs)
    return wrapper
