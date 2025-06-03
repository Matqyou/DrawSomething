//
// Created by Matq on 30/01/2025.
//

#pragma once

#include <array>
#include <algorithm>

struct GameStats {
public:
    struct Seperate {
        int longest_streak;
        int coin_distribution;
    };

    struct ColorUsage {
        int color_id;
        float usage;
    };

    Seperate player_stats[2];
    float average_guess_speed_seconds;
    float average_draw_speed_seconds;
    std::array<ColorUsage, 3> top_color_usage;
    std::array<int, 3> word_difficulty_distribution;
    bool populated_with_data;

public:
    GameStats();
    ~GameStats();

};
