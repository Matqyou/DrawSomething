//
// Created by Matq on 17/02/2025.
//

#include "CheckBox.h"
#include "ui/cursors/Cursors.h"

CheckBox::CheckBox()
	: Element(),
	  activated_visual_texture(nullptr)
{
	this->name = L"CheckBox";
	this->activated_callback = { };
	this->deactivated_callback = { };
	this->clickable = true;
	this->activated = false;

	this->SetDraw(DRAW_RECT);
}

CheckBox::CheckBox(Texture *texture,
				   Texture *activated_texture)
	: Element(),
	  activated_visual_texture(activated_texture)
{
	this->name = L"CheckBox";
	this->activated_callback = { };
	this->deactivated_callback = { };
	this->clickable = true;
	this->activated = false;

	this->SetDraw(DRAW_TEXTURE);
	this->SetTexture(texture);
}

CheckBox::~CheckBox()
{

}

void CheckBox::SetActivated(bool activated)
{
	this->activated = activated;
}

void CheckBox::RunActivatedCallback() const
{
	if (activated_callback != nullptr)
		activated_callback();
}

void CheckBox::RunDeactivatedCallback() const
{
	if (deactivated_callback != nullptr)
		deactivated_callback();
}

void CheckBox::PostRefresh()
{
	if (draw == DRAW_TEXTURE)
	{
		UpdateTexturePlacement();
		UpdateActivatedTexture();
	}
}

void CheckBox::HandleEvent(const SDL_Event& sdl_event, EventContext& event_summary)
{
	HandleEventChildren(sdl_event, event_summary);

	if (!event_summary.rapid_context.event_captured && clickable)
	{ // For the buttons it's pretty easy
		switch (sdl_event.type)
		{
			case SDL_EVENT_MOUSE_MOTION:
			{
				if (event_summary.cursor_changed == CursorChange::NO_CHANGE &&
					PointCollides((int)sdl_event.motion.x, (int)sdl_event.motion.y))
				{
					event_summary.cursor_changed = CursorChange::TO_POINTER;
					SDL_SetCursor(Cursors::sCursorSystemPointer);
					break;
				}

				break;
			}
			case SDL_EVENT_MOUSE_BUTTON_DOWN:
			{
				if (sdl_event.button.button == SDL_BUTTON_LEFT &&
					!event_summary.rapid_context.event_captured
					&& PointCollides((int)sdl_event.button.x, (int)sdl_event.button.y))
				{
					event_summary.rapid_context.event_captured = true;

					activated = !activated;
					if (activated) RunActivatedCallback();
					else RunDeactivatedCallback();
				}

				break;
			}
//            case SDL_EVENT_MOUSE_BUTTON_UP: { // aka holding animation
//                if (sdl_event.button.button == SDL_BUTTON_LEFT)
//                    pressed_down = false;
//                break;
//            }
		}
	}
}

void CheckBox::Render()
{
	if (draw != ElementDraw::DONT_DRAW)
	{
		auto drawing = Assets::Get()->GetDrawing();

		if (draw == ElementDraw::DRAW_RECT)
		{
			auto& fill_color = has_focus || activated ? focus_color : color;
			drawing->SetColor(fill_color);
			drawing->FillRect(GetRect());
		}
		else if (draw == ElementDraw::DRAW_TEXTURE)
		{
			SDL_Texture *activated_sdl_texture = activated_visual_texture.GetTexture()->SDLTexture();
			if (activated && activated_sdl_texture != nullptr)
			{
				drawing->RenderTexture(activated_sdl_texture,
									   nullptr,
									   activated_visual_texture.GetResultingFRect());
			}
			else
			{
				drawing->RenderTexture(texture_instance.GetTexture()->SDLTexture(),
									   nullptr, texture_instance.GetResultingFRect());
			}
		}
	}

	RenderChildren();
}

void CheckBox::UpdateActivatedTexture()
{
	activated_visual_texture.UpdateWithNewPlacement(GetRect());
}
