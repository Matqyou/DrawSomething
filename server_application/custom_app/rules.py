import re

EMAIL_REGEX = re.compile(r"^[a-zA-Z0-9_.+-]+@[a-zA-Z0-9-]+\.[a-zA-Z0-9-.]+$")


def valid_username(username: str) -> (bool, str):
    if len(username) < 4:
        return False, "Username must be at least 4 characters long"

    if len(username) > 16:
        return False, f"Username must be at most 16 characters long"

    # if username.isalnum():
    #     return False, "Username must contain only letters or numbers"
    return True, ""


def valid_password(password: str) -> (bool, str):
    if len(password) < 4:
        return False, "Password must be at least 4 characters long"

    if len(password) > 32:
        return False, f"Password must be at most 32 characters long"

    # if username.isalnum():
    #     return False, "Username must contain only letters or numbers"
    return True, ""


def valid_email(email: str) -> (bool, str):
    if not EMAIL_REGEX.match(email):
        return False, "Invalid email address format"
    return True, ""
