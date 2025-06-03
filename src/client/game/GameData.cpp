//
// Created by Matq on 26/04/2025.
//

#include "GameData.h"
#include "client/ui/menus/intermission/IntermissionMenu.h"
#include "client/ui/menus/auth/AuthMenu.h"
#include "client/ui/menus/main/MainMenu.h"
#include "client/ui/menus/picking/PickingMenu.h"
#include "client/ui/menus/ingame/IngameMenu.h"

GameData Centralized;

void GameData::InitUI()
{
	intermission_menu = new IntermissionMenu();
	auth_menu = new AuthMenu();
	main_menu = new MainMenu();
	picking_menu = new PickingMenu();
	ingame_menu = new IngameMenu();
	current_menu = auth_menu;
}

GameData::GameData()
{
	current_game = nullptr;

	// UI
	intermission_menu = nullptr;
	auth_menu = nullptr;
	main_menu = nullptr;
	picking_menu = nullptr;
	ingame_menu = nullptr;
	current_menu = nullptr;
}

GameData::~GameData()
{
	ClearGames();
}

GameInfo *GameData::GetGame(int game_id) const
{
	for (GameInfo *game : open_games)
	{
		if (game && game->game_id == game_id)
			return game;
	}
	return nullptr;
}

void GameData::AddGame(GameInfo *new_game)
{
	if (!new_game || new_game->game_id == -1)
		return;

	open_games.push_back(new_game);
}

void GameData::RemoveGame(int game_id)
{
	for (auto it = open_games.begin(); it != open_games.end(); ++it)
	{
		auto game = *it;
		if (game->game_id == game_id)
		{
			delete game;
			open_games.erase(it);
			break;
		}
	}
}

void GameData::ClearGames()
{
	for (GameInfo *game : open_games)
		delete game;
	open_games.clear();
}

void GameData::SortGames()
{
	std::sort(open_games.begin(), open_games.end(), [](GameInfo *a, GameInfo *b)
	{
		if (a->game_state != b->game_state)
			return a->game_state < b->game_state;

		if (a->game_turn != b->game_turn)
			return a->game_turn > b->game_turn;

		return a->game_id < b->game_id;
	});
}

void GameData::AddToUserList(Account *new_user)
{
	if (!new_user || new_user->user_id == -1)
		return;

	admin_user_list.push_back(new_user);
}

void GameData::ClearUserList()
{
	for (Account *user : admin_user_list)
		delete user;
	admin_user_list.clear();
}