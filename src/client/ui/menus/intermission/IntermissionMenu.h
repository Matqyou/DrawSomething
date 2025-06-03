//
// Created by Matq on 22/01/2025.
//

#pragma once

#include "../FullscreenMenu.h"
#include "components/MiniProfile.h"
#include "components/Scribbles.h"

class IntermissionMenu : public FullscreenMenu
{
private:
//    TextureData* texture_turn_number;
	TextElement *text_turn_number;
	Frame *menu_a;

	Intermission::Scribbles *scribbles;
	Intermission::MiniProfile *mini_profile1;
	Intermission::MiniProfile *mini_profile2;
	Frame *menu_b;

	TextElement *coins_earned_text;
	Frame *coins_frame;

	using Callback = std::function<void()>;
	Callback end_animation_callback;
	std::chrono::steady_clock::time_point end_animation_countdown;
	bool countdown_started;

public:
	IntermissionMenu();
	~IntermissionMenu() override;

	// Manipulation
	void ShowMenuA(bool show)
	{
		menu_a->SetEnabled(show);
		menu_b->SetEnabled(!show);
		RefreshMenu();
	}
	void SetTurnNumber(int turn_number);
	void ShowIntermission();
	void BeginAnimation();
	void SetProfile1(const std::string& username, Texture *profile_picture);
	void SetProfile2(const std::string& username, Texture *profile_picture);
	void RefreshData();
	void SetEndAnimationCallback(Callback end_callback);

	// Ticking
	void Tick(double elapsed_seconds) override;

};
