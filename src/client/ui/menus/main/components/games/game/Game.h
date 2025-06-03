//
// Created by Matq on 26/01/2025.
//

#pragma once

#include "../../../../../../game/game_info/GameInfo.h"
#include "../../../../../components/element/TextElement.h"
#include "../../../../../components/element/Frame.h"
#include "../../../../../components/element/Button.h"
#include "Statistics.h"
#include "../../../../../CommonUI.h"

namespace Main
{
class CurrentGames;
class Game : public Frame
{
private:
	Frame *game_frame;
	Statistics *statistics;
	TextElement *turn_number;
	Frame *online;
	Frame *profile_picture;
	TextElement *picture_text;
	TextElement *state_text;
	TextElement *state_description;
	Button *play_button;
	Button *delete_button;
	GameInfo *game_info_;
	CurrentGames *games_;

	Texture *game_background;

	Texture *turn_texture;

	Texture *play_button_texture;
	Texture *play_grayed_button_texture;
	Texture *delete_button_texture;

	Texture *profile_picture_texture;

	static LinkTexture sTextureDefaultProfilePicture;

	void SetTurnNumber(int turn);
	void SetProfilePicture(Texture *texture_data);
	void SetUsername(const char *username);
	void SetState(GameInfo::GameState game_state);

public:
	explicit Game(CurrentGames *games);
	~Game() override;

	// Getting
	[[nodiscard]] GameInfo *GetGameInfo() const { return game_info_; }

	// Options
	Game *UpdateInfo(GameInfo *game_info);

};
}
