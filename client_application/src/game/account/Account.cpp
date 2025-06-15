//
// Created by Matq on 26/04/2025.
//

#include <fstream>
#include <utility>
#include "Account.h"
#include "core/Strings.h"
#include "core/app/Assets.h"
#include "core/app/Drawing.h"

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

	bombs = 0; //

	profile_picture = nullptr;
	pfp_url = ""; //
	pfp_update_callback = { };
}

Account::~Account()
{

}

Account& Account::ParseFromJson(const json& account_data)
{
	std::string old_pfp = pfp_url;

	user_id = account_data.value("user_id", -1);
	username = account_data.value("username", "Not provided");
	nickname = account_data.value("display_name", "Not provided");
	email = account_data.value("email", "Not provided");
	is_administrator = account_data.value("admin_user", false);
	is_logged_in = account_data.value("logged_in", false);
	if (account_data.contains("profile_picture") && account_data["profile_picture"].is_string())
	{
		pfp_url = account_data["profile_picture"];
	}
	else
	{
		pfp_url = "";
	}

	// Expanded data
	games_played = account_data.value("games_played", games_played);
	rounds_played = account_data.value("rounds_played", rounds_played);
	highest_turn = account_data.value("highest_turn", highest_turn);
	coins = account_data.value("coins", coins);
	coins_earned = account_data.value("coins_earned", coins_earned);
	coins_spent = account_data.value("coins_spent", coins_spent);
	bombs = account_data.value("bombs", bombs);

	if (account_data.contains("colors") && account_data["colors"].is_array())
	{
		color_bundles.clear();
		for (const auto& bundle_id : account_data["colors"])
		{
			if (bundle_id.is_number_integer())
				color_bundles.push_back(bundle_id.get<int>());
		}
	}

	created = account_data.value("creation_date", created);

	if (old_pfp != pfp_url)
		FetchProfilePicture();//

	return *this;
}

Account& Account::SetProfilePictureUpdateCallback(Callback callback)
{
	pfp_update_callback = std::move(callback);
	return *this;
}

void Account::FetchProfilePicture()
{
	if (pfp_url.empty())
		return;

	auto file_packet = new FilePacket(pfp_url);
	file_packet->SetResponseCallback([file_packet, this](const std::vector<unsigned char>& data)
									 {
										 // Create a temporary file
										 std::string temp_filename = Strings::GenerateRandomFilename();
										 std::ofstream temp_file(temp_filename, std::ios::binary);
										 if (!temp_file)
										 {
											 dbg_msg("&cFailed to create temporary file: %s\n", SDL_GetError());
											 return;
										 }

										 // Write the image data to the temporary file
										 temp_file.write(reinterpret_cast<const char *>(data.data()), data.size());
										 temp_file.close();

										 // Load the image from the temporary file
										 SDL_Texture *texture = IMG_LoadTexture(Assets::Get()->GetDrawing()->Renderer(), temp_filename.c_str());
										 if (!texture)
										 {
											 dbg_msg("&cIMG_LoadTexture failed: %s\n", SDL_GetError());
											 std::remove(temp_filename.c_str()); // Clean up the temporary file
											 return;
										 }

										 profile_picture = (new Texture(texture))->FlagForAutomaticDeletion();

										 std::remove(temp_filename.c_str());
										 if (pfp_update_callback)
											 pfp_update_callback();

										 delete file_packet;
									 });
	file_packet->Send();
}

void Account::Logout()
{
	profile_picture = nullptr;
	//
}