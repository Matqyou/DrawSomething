//
// Created by Matq on 30/01/2025.
//

#pragma once

#include "core/Strings.h"
#include "core/app/Assets.h"
#include "GameStats.h"
#include "network/NetworkClient.h"

class GameInfo {
public:
    enum class GameState {
        YOUR_MOVE,
        THEIR_MOVE,
    };
	enum class TurnStage {
		PICKING,
		DRAWING,
		GUESSING,
	};

    struct PlayerInfo {
		int user_id;
        std::string username;
		std::string profile_picture_url;
        Texture* profile_picture;
		bool online;
    };

	int game_id;
	PlayerInfo players[2];
	int game_turn;
	GameState game_state;
	TurnStage turn_stage;
    GameStats game_statistics;
	std::string word;

    bool populated_with_data;
	Callback pfp_update_callback;

public:
    GameInfo();
    ~GameInfo();

	// Options
	GameInfo* ParseFromJson(const json& game_data, bool full_data);

	// Manipulating
	void FetchProfilePictures();

};
