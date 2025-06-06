# Generated by Django 5.2 on 2025-04-02 22:33

import django.utils.timezone
from django.db import migrations, models


class Migration(migrations.Migration):

    dependencies = [
        ('custom_app', '0003_user_coins_user_coins_earned_user_coins_spent_and_more'),
    ]

    operations = [
        migrations.AddField(
            model_name='user',
            name='activity_date',
            field=models.DateTimeField(auto_now=True),
        ),
        migrations.AddField(
            model_name='user',
            name='creation_date',
            field=models.DateTimeField(auto_now_add=True, default=django.utils.timezone.now),
            preserve_default=False,
        ),
    ]
