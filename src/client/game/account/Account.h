//
// Created by Matq on 26/04/2025.
//

#pragma once

#include <string>
#include "../../network/NetworkClient.h"

struct Account
{
	int user_id;
	std::string username;
	std::string nickname;
	std::string email;
	bool is_administrator;
	bool is_logged_in;

	int games_played;
	int rounds_played;
	int highest_turn;
	int coins;
	int coins_earned;
	int coins_spent;

	std::string created;

public:
	Account();
	~Account();

	// Options
	Account& ParseFromJson(const json& account_data);

};
