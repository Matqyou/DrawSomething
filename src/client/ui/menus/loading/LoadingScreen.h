//
// Created by Matq on 29/01/2025.
//

#pragma once

#include "../FullscreenMenu.h"
#include "../../components/element/TextElement.h"
#include "../intermission/components/Scribbles.h"

class LoadingScreen : public FullscreenMenu
{
private:
	Frame *loading_text;
	Intermission::Scribbles* scribbles;
	Frame *loading_frame;
	TextElement *caption;
	std::chrono::steady_clock::time_point last_updated_text;
	size_t current_text;
	Uint8 screen_alpha;

public:
	LoadingScreen();
	~LoadingScreen() override;

	// Manipulating
	void StartShowing(const std::string& message);
	void Hide();

	// Ticking
	void HandleEvent(const SDL_Event& sdl_event, EventContext& event_summary) override;
	void Tick(double elapsed_seconds) override;

};
