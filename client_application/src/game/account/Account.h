//
// Created by Matq on 26/04/2025.
//

#pragma once

#include <string>
#include "network/NetworkClient.h"
#include "ui/structures/visual_texture/VisualTexture.h"

struct Account
{
	int user_id;
	std::string username;
	std::string nickname;
	std::string email;
	bool is_administrator;
	bool is_logged_in;

	std::string pfp_url;
	Texture *profile_picture;
	Callback pfp_update_callback;

	int games_played;
	int rounds_played;
	int highest_turn;
	int coins;
	int coins_earned;
	int coins_spent;
	std::vector<int> color_bundles;

	int bombs;

	std::string created;

public:
	Account();
	~Account();

	// Options
	Account& ParseFromJson(const json& account_data);
	Account& SetProfilePictureUpdateCallback(Callback callback);

	// Networking
	void FetchProfilePicture();
	void Logout();

};
