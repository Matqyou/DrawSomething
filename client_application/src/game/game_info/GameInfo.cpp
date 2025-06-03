//
// Created by Matq on 30/01/2025.
//

#include "GameInfo.h"
#include "game/GameData.h"
#include "core/app/Assets.h"
#include "core/app/Drawing.h"
#include <fstream>

GameInfo::GameInfo()
	: players{{ "undefined", "", nullptr, false },
			  { "undefined", "", nullptr, false }},
	  game_turn(0),
	  game_state(GameState::THEIR_MOVE),
	  word("-"),
	  populated_with_data(false)
{
	pfp_update_callback = nullptr;
}

GameInfo::~GameInfo()
{

}

GameInfo *GameInfo::ParseFromJson(const json& game_data, bool full_data)
{
	int id = game_data.value("game_id", -1);
	int coins_a = game_data.value("coins_a", 0);
	int coins_b = game_data.value("coins_b", 0);
	int turn_number = game_data.value("turn_number", 0);
	std::string current_turn = game_data.value("current_turn", "Not provided");
	std::string stage = game_data.value("turn_stage", "Not provided");
	if (full_data)// redundant
	{
		word = game_data.value("word", "-");
	}

	int party_a_id = -1;
	std::string party_a_username = "Not provided";
	std::string party_a_pfp = "";
	bool party_a_online = false;
	if (game_data.contains("party_a") && game_data["party_a"].is_object())
	{
		party_a_id = game_data["party_a"].value("user_id", -1);
		party_a_username = game_data["party_a"].value("display_name", "Not provided");
		party_a_online = game_data["party_a"].value("logged_in", false);\

		if (game_data["party_a"].contains("profile_picture") && !game_data["party_a"]["profile_picture"].is_null())
			party_a_pfp = game_data["party_a"].value("profile_picture", "");
	}

	int party_b_id = -1;
	std::string party_b_username = "Not provided";
	std::string party_b_pfp = "";
	bool party_b_online = false;
	if (game_data.contains("party_b") && game_data["party_b"].is_object())
	{
		party_b_id = game_data["party_b"].value("user_id", -1);
		party_b_username = game_data["party_b"].value("display_name", "Not provided");
		party_b_online = game_data["party_b"].value("logged_in", false);

		if (game_data["party_b"].contains("profile_picture") && !game_data["party_b"]["profile_picture"].is_null())
			party_b_pfp = game_data["party_b"].value("profile_picture", "");
	}

	bool local_player_a = party_a_id == Centralized.GetAccount().user_id;
	game_id = id;
	players[0].username = local_player_a ? party_a_username : party_b_username;
	players[1].username = local_player_a ? party_b_username : party_a_username;
	players[0].profile_picture = nullptr;
	players[1].profile_picture = nullptr;
	players[0].profile_picture_url = local_player_a ? party_a_pfp : party_b_pfp;
	players[1].profile_picture_url = local_player_a ? party_b_pfp : party_a_pfp;
//	players[0].profile_picture = Assets::Get()->GetTexture(Strings::FString("profile_pictures.cat%d", 1 + rand() % 12));
//	players[1].profile_picture = Assets::Get()->GetTexture(Strings::FString("profile_pictures.cat%d", 1 + rand() % 12));
	players[0].online = local_player_a ? party_a_online : party_b_online;
	players[1].online = local_player_a ? party_b_online : party_a_online;
	game_state = ((current_turn == "PARTY_A" && party_a_id == Centralized.GetAccount().user_id) ||
		(current_turn == "PARTY_B" && party_b_id == Centralized.GetAccount().user_id)) ? GameInfo::GameState::YOUR_MOVE
																					   : GameInfo::GameState::THEIR_MOVE;
	turn_stage = (stage == "PICKING" ? GameInfo::TurnStage::PICKING :
				  (stage == "DRAWING" ? GameInfo::TurnStage::DRAWING :
				   GameInfo::TurnStage::GUESSING));
	populated_with_data = true;

//					game_info_example.game_statistics.player_stats[0].longest_streak = 7;
//					game_info_example.game_statistics.player_stats[1].longest_streak = 52;
//					game_info_example.game_statistics.average_guess_speed_seconds = 76.74f;
//					game_info_example.game_statistics.average_draw_speed_seconds = 116.22f;
//					game_info_example.game_statistics.top_color_usage[0].color_id = 1;
//					game_info_example.game_statistics.top_color_usage[0].usage = 50.0f;
//					game_info_example.game_statistics.top_color_usage[1].color_id = 3;
//					game_info_example.game_statistics.top_color_usage[1].usage = 20.0f;
//					game_info_example.game_statistics.top_color_usage[2].color_id = 5;
//					game_info_example.game_statistics.top_color_usage[2].usage = 18.0f;
//					game_info_example.game_statistics.word_difficulty_distribution[0] = 3;
//					game_info_example.game_statistics.word_difficulty_distribution[1] = 12;
//					game_info_example.game_statistics.word_difficulty_distribution[2] = 57;
	game_statistics.populated_with_data = true;

	game_statistics.player_stats[0].coin_distribution = coins_a;
	game_statistics.player_stats[1].coin_distribution = coins_b;
	game_turn = turn_number;

	FetchProfilePictures();
	return this;
}

void GameInfo::FetchProfilePictures()
{
	if (players[1].profile_picture_url.empty())
		return;

	auto file_packet = new FilePacket(players[1].profile_picture_url);
	file_packet->SetResponseCallback([file_packet, this](const std::vector<unsigned char>& data)
									 {
										 // Create a temporary file
										 const char *temp_filename = "temp_image.png";
										 std::ofstream temp_file(temp_filename, std::ios::binary);
										 if (!temp_file)
										 {
											 dbg_msg("Failed to create temporary file: %s\n", SDL_GetError());
											 return;
										 }

										 // Write the image data to the temporary file
										 temp_file.write(reinterpret_cast<const char *>(data.data()), data.size());
										 temp_file.close();

										 // Load the image from the temporary file
										 SDL_Texture *texture = IMG_LoadTexture(Assets::Get()->GetDrawing()->Renderer(), temp_filename);
										 if (!texture)
										 {
											 dbg_msg("IMG_LoadTexture failed: %s\n", SDL_GetError());
											 std::remove(temp_filename); // Clean up the temporary file
											 return;
										 }

										 players[1].profile_picture = (new Texture(texture))->FlagForAutomaticDeletion();

										 std::remove(temp_filename);
										 delete file_packet;

										 if (pfp_update_callback)
											 pfp_update_callback();
									 });
	file_packet->Send();
}