//
// Created by Matq on 26/01/2025.
//

#include "Statistics.h"
#include "ui/CommonUI.h"

namespace Main {
static LinkTexture sTextureStatsBody("main_menu.games.game"); // todo: change

void Statistics::SetUsername(const char* username) {
    name_right->UpdateText(CommonUI::sFontSmall.GetTTFFont(),
                           username, { 104, 195, 235, 255 });
}

void Statistics::SetLeftStreak(int longest_streak) {
    streak_left->UpdateText(CommonUI::sFontBigger.GetTTFFont(),
                            Strings::FString("%d", longest_streak).c_str(),
                            { 104, 195, 235, 255 });
}

void Statistics::SetRightStreak(int longest_streak) {
    streak_right->UpdateText(CommonUI::sFontBigger.GetTTFFont(),
                             Strings::FString("%d", longest_streak).c_str(),
                             { 104, 195, 235, 255 });
}

void Statistics::SetLeftDistribution(int distribution) {
    distribution_left->UpdateText(CommonUI::sFontSmall.GetTTFFont(),
                                  Strings::FString("%d%%", distribution).c_str(),
                                  { 255, 255, 255, 255 });

}

void Statistics::SetRightDistribution(int distribution) {
    distribution_right->UpdateText(CommonUI::sFontSmall.GetTTFFont(),
                                   Strings::FString("%d%%", distribution).c_str(),
                                   { 255, 255, 255, 255 });
}

void Statistics::SetAvgGuessSpeed(float guess_speed) {
    seconds_left->UpdateText(CommonUI::sFontBiggest.GetTTFFont(),
                             Strings::FString("%.2f", guess_speed).c_str(),
                             { 104, 195, 235, 255 });
}

void Statistics::SetAvgDrawSpeed(float draw_speed) {
    seconds_right->UpdateText(CommonUI::sFontBiggest.GetTTFFont(),
                              Strings::FString("%.2f", draw_speed).c_str(),
                              { 104, 195, 235, 255 });
}

Statistics::Statistics()
    : Frame() {

    streak_left = (TextElement*)(new TextElement())
        ->UpdateText(CommonUI::sFontBigger.GetTTFFont(), "NaN", { 104, 195, 235, 255 })
        ->SetName("Streak1");

    streak_right = (TextElement*)(new TextElement())
        ->UpdateText(CommonUI::sFontBigger.GetTTFFont(), "NaN", { 104, 195, 235, 255 })
        ->SetName("Streak2");

    auto streak_frame = (new Frame())
        ->SetAlign(Align::CENTER, Align::DONT)
        ->SetFlex(Flex::WIDTH, 100)
        ->SetAdaptive(true, true)
        ->SetName("StreakFrame")
        ->AddChildren({ streak_left, streak_right });

    auto distribution_text = (new TextElement())
        ->UpdateText(CommonUI::sFontSmaller.GetTTFFont(), "Coin Distribution", { 104, 195, 235, 255 })
        ->SetAlign(Align::CENTER, Align::DONT)
        ->SetName("DistributionText");

    auto distribution_frame = (new Frame())
        ->SetAdaptive(false, true)
        ->SetFullyOccupy(true, false)
        ->SetColor(0, 0, 255, 10)
        ->SetDraw(DRAW_RECT)
        ->SetName("DistributionFrame")
        ->AddChildren({ distribution_text });

    distribution_left = (TextElement*)(new TextElement())
        ->UpdateText(CommonUI::sFontSmall.GetTTFFont(), "NaN%", { 255, 255, 255, 255 })
        ->SetAlign(Align::LEFT, Align::CENTER)
        ->SetName("Distribution1");

    distribution_right = (TextElement*)(new TextElement())
        ->UpdateText(CommonUI::sFontSmall.GetTTFFont(), "NaN%", { 255, 255, 255, 255 })
        ->SetAlign(Align::RIGHT, Align::CENTER)
        ->SetName("Distribution1");

    auto distribution_bar = (new Frame())
        ->SetFullyOccupy(true, false)
        ->SetAdaptive(false, true)
        ->SetColor(104, 195, 235, 255)
        ->SetDraw(DRAW_RECT)
        ->SetName("DistributionBar")
        ->AddChildren({ distribution_left, distribution_right });

    auto name_left = (new TextElement())
        ->UpdateText(CommonUI::sFontSmall.GetTTFFont(), "you", { 104, 195, 235, 255 })
        ->SetAlign(Align::LEFT, Align::CENTER)
        ->SetName("Username1");

    name_right = (TextElement*)(new TextElement())
        ->UpdateText(CommonUI::sFontSmall.GetTTFFont(), "NaN", { 104, 195, 235, 255 })
        ->SetAlign(Align::RIGHT, Align::CENTER)
        ->SetName("Username2");

    auto names_frame = (new Frame())
        ->SetFullyOccupy(true, false)
        ->SetAdaptive(false, true)
        ->SetName("NamesFrame")
        ->AddChildren({ name_left, name_right });

    auto guess_text = (new TextElement())
        ->UpdateText(CommonUI::sFontSmaller.GetTTFFont(), "Avg. Guess Speed", { 104, 195, 235, 255 })
        ->SetName("GuessSpeedText");

    auto draw_text = (new TextElement())
        ->UpdateText(CommonUI::sFontSmaller.GetTTFFont(), "Avg. Draw Speed", { 104, 195, 235, 255 })
        ->SetAlign(Align::RIGHT, Align::DONT)
        ->SetName("DrawSpeedText");

    auto speed_frame = (new Frame())
        ->SetFullyOccupy(true, false)
        ->SetAdaptive(false, true)
        ->SetColor(0, 0, 255, 10)
        ->SetDraw(DRAW_RECT)
        ->SetName("SpeedBar")
        ->AddChildren({ guess_text, draw_text });

    seconds_left = (TextElement*)(new TextElement())
        ->UpdateText(CommonUI::sFontBiggest.GetTTFFont(), "NaN", { 104, 195, 235, 255 })
        ->SetName("Seconds1");

    auto seconds_left_description = (new TextElement())
        ->UpdateText(CommonUI::sFontSmaller.GetTTFFont(), "seconds", { 104, 195, 235, 255 })
        ->SetName("SecondsDescription1");

    auto seconds_left_frame = (new Frame())
        ->SetAlign(Align::LEFT, Align::DONT)
        ->SetFlex(Flex::HEIGHT)
        ->SetAdaptive(true, true)
        ->SetName("SecondsFrame1")
        ->AddChildren({ seconds_left, seconds_left_description });

    seconds_right = (TextElement*)(new TextElement())
        ->UpdateText(CommonUI::sFontBiggest.GetTTFFont(), "NaN", { 104, 195, 235, 255 })
        ->SetName("Seconds2");

    auto seconds_right_description = (new TextElement())
        ->UpdateText(CommonUI::sFontSmaller.GetTTFFont(), "seconds", { 104, 195, 235, 255 })
        ->SetName("SecondsDescription2");

    auto seconds_right_frame = (new Frame())
        ->SetAlign(Align::RIGHT, Align::DONT)
        ->SetFlex(Flex::HEIGHT)
        ->SetAdaptive(true, true)
        ->SetName("SecondsFrame2")
        ->AddChildren({ seconds_right, seconds_right_description });

    auto seconds_frame = (new Frame())
        ->SetFullyOccupy(true, false)
        ->SetAdaptive(false, true)
        ->SetName("SpeedBar")
        ->AddChildren({ seconds_left_frame, seconds_right_frame });

    auto colors_text = (new TextElement())
        ->UpdateText(CommonUI::sFontSmaller.GetTTFFont(), "Top Three Colors", { 104, 195, 235, 255 })
        ->SetName("TopColorsText");

    auto difficulty_text = (new TextElement())
        ->UpdateText(CommonUI::sFontSmaller.GetTTFFont(), "Word Difficulty", { 104, 195, 235, 255 })
        ->SetAlign(Align::RIGHT, Align::DONT)
        ->SetName("DifficultyText");

    auto chart_frame = (new Frame())
        ->SetFullyOccupy(true, false)
        ->SetAdaptive(false, true)
        ->SetColor(0, 0, 255, 10)
        ->SetDraw(DRAW_RECT)
        ->SetName("SpeedBar")
        ->AddChildren({ colors_text, difficulty_text });

    auto loading_text = (Frame*)(new Frame())
        ->SetTexture(Assets::Get()->GetTexture("loading.text1"))
        ->SetDraw(DRAW_TEXTURE)
        ->SetAlign(Align::CENTER, Align::DONT)
        ->ResizeToTexture()
        ->SetName("LoadingText");

    auto caption = (new TextElement())
        ->UpdateText(CommonUI::sFontSmaller.GetTTFFont(),
                     "Fetching game stats",
                     { 255, 255, 255, 255 })
        ->SetAlign(Align::CENTER, Align::DONT)
        ->SetName("LoadingCaption");

    auto loading_frame = (Frame*)(new Frame())
        ->SetAlign(Align::CENTER, Align::CENTER)
        ->SetFlex(Flex::HEIGHT, 10)
        ->SetAdaptive(true, true)
        ->SetName("LoadingFrame")
        ->AddChildren({ loading_text, caption });

    auto loading_screen = (Frame*)(new Frame())
        ->SetColor(0, 0, 0, 255)
        ->SetFullyOccupy(true, true)
        ->SetFlexInvolved(false, false)
        ->SetName("LoadingScreen")
        ->AddChildren({ loading_frame });

    this->SetSize(Vec2i(560, 0));
    this->SetTexture(sTextureStatsBody.GetTexture());
    this->SetDraw(DRAW_TEXTURE);
//                          Vec2d(1.0516473731077471, 1.0), Vec2d(-0.024555461473327687, 0.0)

    this->SetEnabled(false);
    this->SetFlex(Flex::HEIGHT);
    this->SetAdaptive(false, true);
    this->SetName("Statistics");
    this->AddChildren({ streak_frame, distribution_frame, distribution_bar, names_frame, speed_frame, seconds_frame,
                        chart_frame, loading_screen });
}

Statistics::~Statistics() {

}

void Statistics::UpdateInfo(GameInfo* game_info) {
    if (game_info == nullptr)
        return;

    GameStats* game_stats = &game_info->game_statistics;
    SetUsername(game_info->players[1].username.c_str());
    SetLeftStreak(game_stats->player_stats[0].longest_streak);
    SetRightStreak(game_stats->player_stats[1].longest_streak);
    SetLeftDistribution(game_stats->player_stats[0].coin_distribution);
    SetRightDistribution(game_stats->player_stats[1].coin_distribution);
    SetAvgGuessSpeed(game_stats->average_guess_speed_seconds);
    SetAvgDrawSpeed(game_stats->average_draw_speed_seconds);
}
}
