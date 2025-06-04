//
// Created by Matq on 12/01/2025.
//

#include "IngameMenu.h"
#include "core/Application.h"
#include "game/GameData.h"

IngameMenu::IngameMenu()
    : FullscreenMenu() {
    name = L"IngameMenu";

    // Header
    this->header = new Ingame::Header();

    // Canvas
    this->canvas = (Ingame::Canvas*)(new Ingame::Canvas())
        ->SetFullyOccupy(true, false)
        ->SetOccupy(false, true)
        ->SetColor(255, 255, 255, 255)
        ->SetName("Canvas");

    // Buttons and stuff
    this->panel = new Ingame::Panel();
    this->tool_selector = new Ingame::ToolSelector(canvas);
    this->color_selector = new Ingame::ColorSelector(canvas, tool_selector);

    this->SetColor(190, 190, 190, 255);
    this->SetFlex(Flex::HEIGHT);
    this->AddChildren({ header, canvas });
    this->SetName("GuessingMenu");
    this->RefreshMenu();
}

void IngameMenu::PrepareGuess() {
	auto current_game = Centralized.GetCurrentGame();
	if (current_game != nullptr)
	{
		auto opponent_id = current_game->players[1].user_id;
		auto opponent = Centralized.GetKnownUser(opponent_id);

		header->SetTitle("You are guessing");
		header->SetDescription(Strings::FString("%s's drawing.", opponent->nickname.c_str()));
		header->SetTurnNumber(current_game->game_turn);
		header->SetProfilePicture(opponent->profile_picture);
		panel->RandomizeWord(current_game->word);
	}

    SetChildren({ header, canvas });
    Refresh();
    canvas->ClearCanvas();
    canvas->SetPlaybackMode(Ingame::PlaybackMode::DO_NOTHING);
    canvas->SetMode(Ingame::CANVAS_GUESS);
    canvas->SetCallback([this]() {
        canvas->SetPlaybackMode(Ingame::PlaybackMode::REPLAY);
        this->SetChildren({ header, canvas, panel });
        this->Refresh();
    });
}

void IngameMenu::PrepareWatch() {
//    header->SetTitle("You are watching Matiss B.");
//    header->SetDescription(Strings::FString("guess the word %s.", random_word.c_str()));
//    header->SetTurnNumber(4);
//    panel->RandomizeWord(random_word);
//
//    SetChildren({ header, canvas });
//    Refresh();
//    canvas->ClearCanvas();
//    canvas->SetPlaybackMode(Ingame::PlaybackMode::DO_NOTHING);
//    canvas->SetMode(Ingame::CANVAS_WATCH);
//    canvas->SetCallback([this]() {
//        canvas->LoadExample();
//        this->SetChildren({ header, canvas, panel });
//        this->Refresh();
//    });
}

void IngameMenu::PrepareDraw() {
	auto current_game = Centralized.GetCurrentGame();
	if (current_game != nullptr)
	{
		auto opponent_id = current_game->players[1].user_id;
		auto opponent = Centralized.GetKnownUser(opponent_id);

		header->SetTitle(Strings::FString("You are drawing %s", current_game->word.c_str()));
		header->SetDescription(Strings::FString("for %s's.", opponent->nickname.c_str()));
		header->SetProfilePicture(opponent->profile_picture);
		header->SetTurnNumber(current_game->game_turn);
	}

    canvas->ClearCanvas();
    canvas->SetPlaybackMode(Ingame::PlaybackMode::DO_NOTHING);

    SetChildren({ header, canvas });
    Refresh();
    canvas->SetMode(Ingame::CANVAS_DRAW);
    canvas->SetCallback([this]() {
        canvas->SetPlaybackMode(Ingame::PlaybackMode::RECORD);
        this->SetChildren({ header, color_selector, canvas, tool_selector });
        this->Refresh();
    });
}

void IngameMenu::SetDoneDrawingCallback(Callback new_callback)
{
	tool_selector->SetDoneCallback(std::move(new_callback));
}