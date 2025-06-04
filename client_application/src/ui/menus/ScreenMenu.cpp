//
// Created by Matq on 24/05/2025.
//

#include "ScreenMenu.h"

ScreenMenu::ScreenMenu()
	: Frame()
{
	this->SetEnabled(false);
	this->SetColor(0, 0, 0, 200);
	this->SetDraw(DRAW_RECT);
	this->SetFullyOccupy(true, true);
	this->SetName("UnnamedScreen");
}

ScreenMenu::~ScreenMenu()
{

}

void ScreenMenu::ScreenEvent(const SDL_Event& sdl_event, EventContext& event_summary) {
	// When multiple menus are stacked, the oldest one will prevent events from passing onto lower menus
	if ((sdl_event.type == SDL_EVENT_KEY_DOWN || sdl_event.type == SDL_EVENT_KEY_UP) ||
		(sdl_event.type == SDL_EVENT_MOUSE_MOTION && PointCollides((int)sdl_event.motion.x, (int)sdl_event.motion.y)) ||
		(sdl_event.type == SDL_EVENT_MOUSE_BUTTON_DOWN || sdl_event.type == SDL_EVENT_MOUSE_BUTTON_UP) && PointCollides((int)sdl_event.button.x, (int)sdl_event.button.y))
		event_summary.rapid_context.event_captured = true;

	switch (sdl_event.type)
	{
		case SDL_EVENT_KEY_DOWN:
		{
			if (sdl_event.key.scancode == SDL_SCANCODE_ESCAPE)
				this->SetEnabled(false);
			break;
		}
	}
}

void ScreenMenu::HandleEvent(const SDL_Event& sdl_event, EventContext& event_summary)
{
	HandleEventChildren(sdl_event, event_summary);
	ScreenEvent(sdl_event, event_summary);
}
