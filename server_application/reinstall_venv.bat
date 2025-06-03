@echo off
echo Deleting existing virtual environment...
rmdir /s /q venv

echo Creating new virtual environment...
python -m venv venv

echo Activating virtual environment...
call venv\Scripts\activate

echo Installing dependencies...
pip install -r requirements.txt

echo Setup complete!

exit
