//
// Created by Matq on 26/04/2025.
//

#include "Account.h"
#include "core/Strings.h"

Account::Account()
{
	user_id = -1;
	is_administrator = false;
	is_logged_in = false;

	games_played = 0; //
	rounds_played = 0; //
	highest_turn = 0; //
	coins = 0; //
	coins_earned = 0; //
	coins_spent = 0; //
}

Account::~Account()
{

}

Account& Account::ParseFromJson(const json& account_data)
{
	user_id = account_data.value("user_id", -1);
	username = account_data.value("username", "Not provided");
	nickname = account_data.value("display_name", "Not provided");
	email = account_data.value("email", "Not provided");
	is_administrator = account_data.value("admin_user", false);
	is_logged_in = account_data.value("logged_in", false);

	games_played = account_data.value("games_played", -1);
	rounds_played = account_data.value("rounds_played", -1);
	highest_turn = account_data.value("highest_turn", -1);
	coins = account_data.value("coins", -1);
	coins_earned = account_data.value("coins_earned", -1);
	coins_spent = account_data.value("coins_spent", -1);

	created = account_data.value("creation_date", "Not provided");
	return *this;
}