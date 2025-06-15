//
// Created by Matq on 03/02/2025.
//

#pragma once

#include "Frame.h"

class ScrollFrame : public Frame {
private:
    Orientation scroll_direction;
    double scroll;

	Texture* capture_area;

	void UpdateCaptureArea();

public:
    ScrollFrame();
    ~ScrollFrame();

	// Options
	ScrollFrame* SetScrollDirection(Orientation direction) {
		scroll_direction = direction;
		return this;
	}

	// Ticking
	void HandleEvent(const SDL_Event& sdl_event, EventContext& event_summary) override;
	void Render() override;
	void PreComposition() override;

};
