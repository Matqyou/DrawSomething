# Mini guide to running Django back-end!
### Prerequisites
At the moment the project should work on all Windows machines. \
You need a [Python interpreter](https://www.python.org/downloads/) installed.
## First time setup
1. Enter project directory.
2. Install python virtual environment(containing modules for the project).
3. Activate virtual environment.
4. Create the database(sqlite3 at the moment) on your machine. 
5. Run server.
```
cd backend
.\reinstall_venv.bat
venv/Scripts/activate
python manage.py migrate
python manage.py runserver
```
## Running backend after setup
1. Enter project directory.
2. Activate virtual environment.
3. Run server
```
cd backend
venv/Scripts/activate
python manage.py runserver
```

### Test by opening http://localhost:8000 and http://localhost:8000/bye
### For development you can read the [DJango Documentation here](docs.djangoproject.com/en/).

 PS: Secrets are pushed to the repository.. DO NOT FORGET TO CHANGE:
 `/backend/configuration/settings.py at SECRET_KEY = ...`
 