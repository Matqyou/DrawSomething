//
// Created by Matq on 26/01/2025.
//

#include "Statistics.h"
#include "../../../../../components/element/TextElement.h"
#include "../../../../../CommonUI.h"

namespace Main {
static LinkTexture sTextureStatsBody("main_menu.games.game"); // todo: change

Statistics::Statistics()
    : Frame(Vec2i(0, 0),
            Vec2i(560, 0),
            VisualTexture(sTextureStatsBody.GetSDLTexture(),
                          Vec2d(1.0516473731077471, 1.0),
                          Vec2d(-0.024555461473327687, 0.0))) {

    auto streak_left = (new TextElement(Vec2i(0, 0)))
        ->UpdateText(CommonUI::sFontBigger.GetTTFFont(), "1", { 104, 195, 235, 255 })
        ->SetName("Streak1");

    auto streak_right = (new TextElement(Vec2i(0, 0)))
        ->UpdateText(CommonUI::sFontBigger.GetTTFFont(), "2", { 104, 195, 235, 255 })
        ->SetName("Streak2");

    auto streak_frame = (new Frame(Vec2i(0, 0),
                                   Vec2i(0, 0),
                                   DONT_DRAW))
        ->SetAlign(Align::CENTER, Align::DONT)
        ->SetFlex(Flex::WIDTH, 100)
        ->SetAdaptive(true, true)
        ->SetName("StreakFrame")
        ->AddChildren({ streak_left, streak_right });

    auto distribution_text = (new TextElement(Vec2i(0, 0)))
        ->UpdateText(CommonUI::sFontSmaller.GetTTFFont(), "Coin Distribution", { 104, 195, 235, 255 })
        ->SetAlign(Align::CENTER, Align::DONT)
        ->SetName("DistributionText");

    auto distribution_frame = (new Frame(Vec2i(0, 0),
                                         Vec2i(0, 0),
                                         DRAW_RECT))
        ->SetAdaptive(false, true)
        ->SetFullyOccupy(true, false)
        ->SetColor(0, 0, 255, 10)
        ->SetName("DistributionFrame")
        ->AddChildren({ distribution_text });

    auto distribution_left = (new TextElement(Vec2i(0, 0)))
        ->UpdateText(CommonUI::sFontSmall.GetTTFFont(), "48%", { 255, 255, 255, 255 })
        ->SetAlign(Align::LEFT, Align::CENTER)
        ->SetName("Distribution1");

    auto distribution_right = (new TextElement(Vec2i(0, 0)))
        ->UpdateText(CommonUI::sFontSmall.GetTTFFont(), "52%", { 255, 255, 255, 255 })
        ->SetAlign(Align::RIGHT, Align::CENTER)
        ->SetName("Distribution1");

    auto distribution_bar = (new Frame(Vec2i(0, 0),
                                       Vec2i(0, 0),
                                       DRAW_RECT))
        ->SetFullyOccupy(true, false)
        ->SetAdaptive(false, true)
        ->SetColor(104, 195, 235, 255)
        ->SetName("DistributionBar")
        ->AddChildren({ distribution_left, distribution_right });

    auto name_left = (new TextElement(Vec2i(0, 0)))
        ->UpdateText(CommonUI::sFontSmall.GetTTFFont(), "you", { 104, 195, 235, 255 })
        ->SetAlign(Align::LEFT, Align::CENTER)
        ->SetName("Username1");

    auto name_right = (new TextElement(Vec2i(0, 0)))
        ->UpdateText(CommonUI::sFontSmall.GetTTFFont(), "Username S.", { 104, 195, 235, 255 })
        ->SetAlign(Align::RIGHT, Align::CENTER)
        ->SetName("Username2");

    auto names_frame = (new Frame(Vec2i(0, 0),
                                  Vec2i(0, 0),
                                  DONT_DRAW))
        ->SetFullyOccupy(true, false)
        ->SetAdaptive(false, true)
        ->SetName("NamesFrame")
        ->AddChildren({ name_left, name_right });

    auto guess_text = (new TextElement(Vec2i(0, 0)))
        ->UpdateText(CommonUI::sFontSmaller.GetTTFFont(), "Avg. Guess Speed", { 104, 195, 235, 255 })
        ->SetName("GuessSpeedText");

    auto draw_text = (new TextElement(Vec2i(0, 0)))
        ->UpdateText(CommonUI::sFontSmaller.GetTTFFont(), "Avg. Draw Speed", { 104, 195, 235, 255 })
        ->SetAlign(Align::RIGHT, Align::DONT)
        ->SetName("DrawSpeedText");

    auto speed_frame = (new Frame(Vec2i(0, 0),
                                  Vec2i(0, 0),
                                  DRAW_RECT))
        ->SetFullyOccupy(true, false)
        ->SetAdaptive(false, true)
        ->SetColor(0, 0, 255, 10)
        ->SetName("SpeedBar")
        ->AddChildren({ guess_text, draw_text });

    auto seconds_left = (new TextElement(Vec2i(0, 0)))
        ->UpdateText(CommonUI::sFontBiggest.GetTTFFont(), "76.74", { 104, 195, 235, 255 })
        ->SetName("Seconds1");

    auto seconds_left_description = (new TextElement(Vec2i(0, 0)))
        ->UpdateText(CommonUI::sFontSmaller.GetTTFFont(), "seconds", { 104, 195, 235, 255 })
        ->SetName("SecondsDescription1");

    auto seconds_left_frame = (new Frame(Vec2i(0, 0),
                                   Vec2i(0, 0),
                                   DONT_DRAW))
        ->SetAlign(Align::LEFT, Align::DONT)
        ->SetFlex(Flex::HEIGHT)
        ->SetAdaptive(true, true)
        ->SetName("SecondsFrame1")
        ->AddChildren({ seconds_left, seconds_left_description });

    auto seconds_right= (new TextElement(Vec2i(0, 0)))
        ->UpdateText(CommonUI::sFontBiggest.GetTTFFont(), "116.22", { 104, 195, 235, 255 })
        ->SetName("Seconds2");

    auto seconds_right_description = (new TextElement(Vec2i(0, 0)))
        ->UpdateText(CommonUI::sFontSmaller.GetTTFFont(), "seconds", { 104, 195, 235, 255 })
        ->SetName("SecondsDescription2");

    auto seconds_right_frame = (new Frame(Vec2i(0, 0),
                                         Vec2i(0, 0),
                                         DONT_DRAW))
        ->SetAlign(Align::RIGHT, Align::DONT)
        ->SetFlex(Flex::HEIGHT)
        ->SetAdaptive(true, true)
        ->SetName("SecondsFrame2")
        ->AddChildren({ seconds_right, seconds_right_description });

    auto seconds_frame = (new Frame(Vec2i(0, 0),
                                    Vec2i(0, 0),
                                    DONT_DRAW))
        ->SetFullyOccupy(true, false)
        ->SetAdaptive(false, true)
        ->SetName("SpeedBar")
        ->AddChildren({ seconds_left_frame, seconds_right_frame });

    auto colors_text = (new TextElement(Vec2i(0, 0)))
        ->UpdateText(CommonUI::sFontSmaller.GetTTFFont(), "Top Three Colors", { 104, 195, 235, 255 })
        ->SetName("TopColorsText");

    auto difficulty_text = (new TextElement(Vec2i(0, 0)))
        ->UpdateText(CommonUI::sFontSmaller.GetTTFFont(), "Word Difficulty", { 104, 195, 235, 255 })
        ->SetAlign(Align::RIGHT, Align::DONT)
        ->SetName("DifficultyText");

    auto chart_frame = (new Frame(Vec2i(0, 0),
                                  Vec2i(0, 0),
                                  DRAW_RECT))
        ->SetFullyOccupy(true, false)
        ->SetAdaptive(false, true)
        ->SetColor(0, 0, 255, 10)
        ->SetName("SpeedBar")
        ->AddChildren({ colors_text, difficulty_text });

    SetEnabled(false);
    SetFlex(Flex::HEIGHT);
    SetAdaptive(false, true);
    SetName("Statistics");
    AddChildren({ streak_frame, distribution_frame, distribution_bar, names_frame, speed_frame, seconds_frame, chart_frame });
}

Statistics::~Statistics() {

}
}
