//
// Created by Matq on 26/01/2025.
//

#pragma once

#include "../../../../../components/element/TextElement.h"
#include "../../../../../components/element/Frame.h"
#include "../../../../../../game/game_info/GameInfo.h"

namespace Main {
class Statistics : public Frame {
private:
    TextElement* streak_left;
    TextElement* streak_right;
    TextElement* distribution_left;
    TextElement* distribution_right;
    TextElement* name_right;
    TextElement* seconds_left;
    TextElement* seconds_right;

    void SetUsername(const char* username);
    void SetLeftStreak(int longest_streak);
    void SetRightStreak(int longest_streak);
    void SetLeftDistribution(int distribution);
    void SetRightDistribution(int distribution);
    void SetAvgGuessSpeed(float guess_speed);
    void SetAvgDrawSpeed(float draw_speed);

public:
    Statistics();
    ~Statistics();

    // Manipulating
    void UpdateInfo(GameInfo* game_info);

};
}
