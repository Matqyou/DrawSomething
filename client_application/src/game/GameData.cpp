//
// Created by Matq on 26/04/2025.
//

#include "GameData.h"
#include "ui/menus/intermission/IntermissionMenu.h"
#include "ui/menus/auth/AuthMenu.h"
#include "ui/menus/main/MainMenu.h"
#include "ui/menus/picking/PickingMenu.h"
#include "ui/menus/ingame/IngameMenu.h"

GameData Centralized;

void GameData::InitUI()
{
	intermission_menu = new IntermissionMenu();
	auth_menu = new AuthMenu();
	main_menu = new MainMenu();
	picking_menu = new PickingMenu();
	ingame_menu = new IngameMenu();
	export_menu = new ExportMenu();
	current_menu = auth_menu;

	main_header = new MainHeader(main_menu);

	account.SetProfilePictureUpdateCallback([this]()
											{
												main_menu->Profile()->SetProfilePicture(account.profile_picture);
												main_menu->GetProfileScreen()->SetProfilePicture(account.profile_picture);
											});
}

GameData::GameData()
{
	current_game = nullptr;
	drawn_image = nullptr;

	// UI
	main_header = nullptr;

	intermission_menu = nullptr;
	auth_menu = nullptr;
	main_menu = nullptr;
	picking_menu = nullptr;
	ingame_menu = nullptr;
	current_menu = nullptr;
	export_menu = nullptr;
}

GameData::~GameData()
{
	delete main_header;

	delete intermission_menu;
	delete auth_menu;
	delete main_menu;
	delete picking_menu;
	delete ingame_menu;
	delete current_menu;
	delete export_menu;

	delete drawn_image;
	ClearGames();
	// clear accounts & colors
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

Account *GameData::GetKnownUser(int user_id)
{
	for (Account *user : known_user_list)
	{
		if (user && user->user_id == user_id)
			return user;
	}
	return nullptr;
}

void GameData::UpdateOwnedColors(const json& colors_data)
{
	for (auto color : owned_colors)
		delete color;
	owned_colors.clear();

	for (const json& color_data : colors_data)
	{
		auto owned_color = (new OwnedColor())
			->ParseFromJson(color_data);
		owned_colors.push_back(owned_color);
	}
}

Account *GameData::UpdateKnownUser(const json& user_data)
{
	auto user_id = user_data.value("user_id", 0);
	if (!user_id)
		return nullptr;

	auto user = GetKnownUser(user_id);
	if (!user)
	{
		user = new Account();
		known_user_list.push_back(user);
	}
	user->ParseFromJson(user_data);
	return user;
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