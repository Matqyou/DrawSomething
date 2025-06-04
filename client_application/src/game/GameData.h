//
// Created by Matq on 26/04/2025.
//

#pragma once

#include "game/account/Account.h"
#include "game/game_info/GameInfo.h"
#include "ui/menus/FullscreenMenu.h"
#include "game/account/OwnedColor.h"

class IntermissionMenu;
class AuthMenu;
class MainMenu;
class PickingMenu;
class IngameMenu;
class GameData
{
private:
	Account account;
	GameInfo *current_game;

	std::vector<OwnedColor*> owned_colors;
	std::vector<Account *> known_user_list;
	std::vector<GameInfo *> open_games;

	std::vector<Account *> admin_user_list;

public:
	// UI
	IntermissionMenu *intermission_menu;
	AuthMenu *auth_menu;
	MainMenu *main_menu;
	PickingMenu *picking_menu;
	IngameMenu *ingame_menu;
	FullscreenMenu *current_menu;

	void InitUI();

public:
	GameData();
	~GameData();

	// Sense
	[[nodiscard]] Account& GetAccount() { return account; }
	[[nodiscard]] GameInfo *GetCurrentGame() { return current_game; }
	[[nodiscard]] std::vector<OwnedColor *>& GetOwnedColors() { return owned_colors; }
	[[nodiscard]] std::vector<GameInfo *>& GetOpenGames() { return open_games; }
	[[nodiscard]] GameInfo *GetGame(int game_id) const;
	[[nodiscard]] std::vector<Account *>& GetAdminUserList() { return admin_user_list; }

	// Generating
	[[nodiscard]] Account *GetKnownUser(int user_id);

	// Options
	GameData& SetCurrentGame(GameInfo *new_current_game)
	{
		this->current_game = new_current_game;
		return *this;
	}

	// Manipulating
	void UpdateOwnedColors(const json& colors_data);
	Account *UpdateKnownUser(const json& user_data);

	void AddGame(GameInfo *new_game);
	void RemoveGame(int game_id);
	void ClearGames();
	void SortGames();

	void AddToUserList(Account *new_user);
	void ClearUserList();

};

extern GameData Centralized;