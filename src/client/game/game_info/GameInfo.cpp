//
// Created by Matq on 30/01/2025.
//

#include "GameInfo.h"

GameInfo GameInfo::GetExample1() {
    GameInfo game_info_example;
    game_info_example.players[0].username = "Player 1.";
    game_info_example.players[0].profile_picture = nullptr;
    game_info_example.players[1].username = "Player 2.";
    game_info_example.players[1].profile_picture = nullptr;
    game_info_example.game_state = GameInfo::GameState::YOUR_MOVE;
    game_info_example.game_turn = 7;
    game_info_example.populated_with_data = true;

    game_info_example.game_statistics.player_stats[0].longest_streak = 7;
    game_info_example.game_statistics.player_stats[0].coin_distribution = 9; // 14
    game_info_example.game_statistics.player_stats[1].longest_streak = 52;
    game_info_example.game_statistics.player_stats[1].coin_distribution = 91; // 143
    game_info_example.game_statistics.average_guess_speed_seconds = 76.74f;
    game_info_example.game_statistics.average_draw_speed_seconds = 116.22f;
    game_info_example.game_statistics.top_color_usage[0].color_id = 1;
    game_info_example.game_statistics.top_color_usage[0].usage = 50.0f;
    game_info_example.game_statistics.top_color_usage[1].color_id = 3;
    game_info_example.game_statistics.top_color_usage[1].usage = 20.0f;
    game_info_example.game_statistics.top_color_usage[2].color_id = 5;
    game_info_example.game_statistics.top_color_usage[2].usage = 18.0f;
    game_info_example.game_statistics.word_difficulty_distribution[0] = 3;
    game_info_example.game_statistics.word_difficulty_distribution[1] = 12;
    game_info_example.game_statistics.word_difficulty_distribution[2] = 57;
    game_info_example.game_statistics.populated_with_data = true;

    return game_info_example;
}

GameInfo GameInfo::GetExample2() {
    GameInfo game_info_example;
    game_info_example.players[0].username = "Player 1.";
    game_info_example.players[0].profile_picture = nullptr;
    game_info_example.players[1].username = "Player 3.";
    game_info_example.players[1].profile_picture = nullptr;
    game_info_example.game_state = GameInfo::GameState::THEIR_MOVE;
    game_info_example.game_turn = 12;
    game_info_example.populated_with_data = true;

    game_info_example.game_statistics.player_stats[0].longest_streak = 7;
    game_info_example.game_statistics.player_stats[0].coin_distribution = 9; // 14
    game_info_example.game_statistics.player_stats[1].longest_streak = 52;
    game_info_example.game_statistics.player_stats[1].coin_distribution = 91; // 143
    game_info_example.game_statistics.average_guess_speed_seconds = 76.74f;
    game_info_example.game_statistics.average_draw_speed_seconds = 116.22f;
    game_info_example.game_statistics.top_color_usage[0].color_id = 1;
    game_info_example.game_statistics.top_color_usage[0].usage = 50.0f;
    game_info_example.game_statistics.top_color_usage[1].color_id = 3;
    game_info_example.game_statistics.top_color_usage[1].usage = 20.0f;
    game_info_example.game_statistics.top_color_usage[2].color_id = 5;
    game_info_example.game_statistics.top_color_usage[2].usage = 18.0f;
    game_info_example.game_statistics.word_difficulty_distribution[0] = 3;
    game_info_example.game_statistics.word_difficulty_distribution[1] = 12;
    game_info_example.game_statistics.word_difficulty_distribution[2] = 57;
    game_info_example.game_statistics.populated_with_data = true;

    return game_info_example;
}

GameInfo::GameInfo()
    : players{{ "undefined", nullptr }, { "undefined", nullptr }},
      game_turn(0),
      game_state(GameState::THEIR_MOVE),
      populated_with_data(false) {

}

GameInfo::~GameInfo() {

}
