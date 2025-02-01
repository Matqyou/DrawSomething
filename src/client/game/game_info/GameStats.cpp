//
// Created by Matq on 30/01/2025.
//

#include "GameStats.h"

GameStats::GameStats()
    : player_stats{{0, 0}, {0, 0}},
      average_guess_speed_seconds(0.0f),
      average_draw_speed_seconds(0.0f),
      top_color_usage{{{0, 0.0f}, {0, 0.0f}, {0, 0.0f}}},
      word_difficulty_distribution{{0, 0, 0}},
      populated_with_data(false) {

}

GameStats::~GameStats() {

}
