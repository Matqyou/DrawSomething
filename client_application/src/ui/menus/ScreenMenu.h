//
// Created by Matq on 24/05/2025.
//

#pragma once

#include "ui/components/element/Frame.h"

class ScreenMenu : public Frame
{
private:

public:
	ScreenMenu();
	~ScreenMenu() override;

	// Ticking
	void ScreenEvent(const SDL_Event& sdl_event, EventContext& event_summary);
	void HandleEvent(const SDL_Event& sdl_event, EventContext& event_summary) override;

};
