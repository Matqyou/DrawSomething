//
// Created by Matq on 26/01/2025.
//

#pragma once

#include "../../../confirmation/ConfirmationScreen.h"
#include "../../../../../game/game_info/GameInfo.h"
#include "../../../../../network/NetworkClient.h"
#include "../../../../components/element/Frame.h"
#include "../../../loading/LoadingScreen.h"

namespace Main
{
class CurrentGames : public Frame
{
private:
	friend class Game;
	Frame *games;
	ConfirmationScreen *confirmation_screen_;
	LoadingScreen *loading_screen_;

	Texture *title_texture;
	Texture *footer_enabled_texture;
	Texture *footer_enabled_pressed_texture;
	Texture *footer_disabled_texture;

	Button *create_game;

public:
	CurrentGames(ConfirmationScreen *confirmation_screen, LoadingScreen *loading_screen);
	~CurrentGames() override;

	// Manipulation
	void RefreshData();

};
}
