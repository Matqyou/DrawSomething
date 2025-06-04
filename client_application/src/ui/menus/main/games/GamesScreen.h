//
// Created by Matq on 04/06/2025.
//

#pragma once

#include "ui/menus/ScreenMenu.h"
#include "network/NetworkClient.h"
#include "ui/components/element/TextElement.h"

class GamesScreen : public ScreenMenu
{
private:
	TextElement* average_coins;
	TextElement* total_coins_earned;
	TextElement* total_coins_circulation;
	TextElement* total_coins_spent;

	TextElement* total_games;
	TextElement* active_games;
	TextElement* average_turns;
	TextElement* games_24h;
	TextElement* games_7d;
	TextElement* highest_turn_reached;
	TextElement* highest_turn_game;

	TextElement* total_users;
	TextElement* active_users_24h;
	TextElement* active_users_7d;
	TextElement* new_users_24h;
	TextElement* new_users_7d;

public:
	GamesScreen();
	~GamesScreen();

	// Manipulating
	void ParseFromJson(const json& json_data);

};
