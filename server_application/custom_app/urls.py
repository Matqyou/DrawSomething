from django.urls import path
from . import views

# URLConf
urlpatterns = [
    path('', views.default),
    path('ping', views.ping),
    path('stats', views.stats),
    path('register', views.register),
    path('login', views.login),
    path('logout', views.logout),
    path('delete_profile', views.delete_profile),
    path('buy_colors', views.buy_colors),
    path('buy_bombs', views.buy_bombs),
    path('edit_profile', views.edit_profile),
    path('upload_profile_picture', views.upload_profile_picture),
    path('all_users', views.all_users),
    path('modify_user/<int:user_id>', views.modify_user),
    path('delete_user/<int:user_id>', views.delete_user),
    path('create_game', views.create_game),
    path('delete_game', views.delete_game),
    path('start_game', views.start_game),
    path('pick_word', views.pick_word),
    path('finished_drawing', views.finished_drawing),
    path('guess_word', views.guess_word),
    path('give_up_guessing', views.give_up_guessing),
    path('get_games', views.get_games),
    path('all_game_stats', views.all_game_stats),
    path('get_shop', views.get_shop),
]
