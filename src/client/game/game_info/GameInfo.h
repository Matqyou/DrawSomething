//
// Created by Matq on 30/01/2025.
//

#pragma once

#include "../../../shared/core/Strings.h"
#include "../../core/app/Assets.h"
#include "GameStats.h"

class GameInfo {
public:
    enum class GameState {
        YOUR_MOVE,
        THEIR_MOVE,
    };

    struct PlayerInfo {
        std::string username;
        TextureData* profile_picture;
    };

    PlayerInfo players[2];
    int game_turn;
    GameState game_state;
    GameStats game_statistics;

    bool populated_with_data;

    static GameInfo GetExample1();
    static GameInfo GetExample2();

public:
    GameInfo();
    ~GameInfo();

};
