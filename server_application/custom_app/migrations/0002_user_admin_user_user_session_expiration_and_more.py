# Generated by Django 5.2 on 2025-04-02 21:55

from django.db import migrations, models


class Migration(migrations.Migration):

    dependencies = [
        ('custom_app', '0001_initial'),
    ]

    operations = [
        migrations.AddField(
            model_name='user',
            name='admin_user',
            field=models.BooleanField(default=False),
        ),
        migrations.AddField(
            model_name='user',
            name='session_expiration',
            field=models.DateTimeField(blank=True, null=True),
        ),
        migrations.AddField(
            model_name='user',
            name='session_key',
            field=models.CharField(blank=True, max_length=64, null=True, unique=True),
        ),
    ]
