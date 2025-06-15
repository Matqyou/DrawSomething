//
// Created by Matq on 03/02/2025.
//

#include "ScrollFrame.h"
#include "core/Application.h"

void ScrollFrame::UpdateCaptureArea()
{
	delete capture_area;
	auto assets = Assets::Get();
	auto area = Application::Get()->GetResolution();
	capture_area = assets->CreateTexture(SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, area.x, area.y);
}

ScrollFrame::ScrollFrame()
	: Frame()
{
	this->name = L"ScrollFrame";

	scroll_direction = Orientation::VERTICAL;
	this->scroll = 0.0;
	this->capture_area = nullptr;

	UpdateCaptureArea();
}

ScrollFrame::~ScrollFrame()
{
	delete capture_area;
}

void ScrollFrame::HandleEvent(const SDL_Event& sdl_event, EventContext& event_summary)
{
	switch (sdl_event.type)
	{
		case SDL_EVENT_WINDOW_RESIZED:
		{
			this->UpdateCaptureArea();
			break;
		}
	}

	HandleEventChildren(sdl_event, event_summary);

	switch (sdl_event.type) {
		case SDL_EVENT_MOUSE_WHEEL: {
			if (!event_summary.rapid_context.event_captured &&
				PointCollides((int)sdl_event.wheel.mouse_x, (int)sdl_event.wheel.mouse_y)) {
				event_summary.rapid_context.event_captured = true;

				scroll += sdl_event.wheel.y * 25;
				this->parent->Refresh();
			}

			break;
		}
	}
}

void ScrollFrame::Render()
{
	auto drawing = Assets::Get()->GetDrawing();

	SDL_FRect source_destination = {
		(float)pos.x,
		(float)pos.y,
		(float)size.x,
		(float)size.y
	};
	auto area = capture_area->GetOriginalSize();
	source_destination = Rectangles::ClipBounds(source_destination, SDL_FRect(0, 0, area.x, area.y));

	drawing->SetRenderTarget(capture_area);
	drawing->SetColor(0, 0, 0, 0);
	drawing->Clear();
	RenderChildren();
	drawing->SetRenderTarget(nullptr);
	drawing->RenderTexture(capture_area->SDLTexture(), &source_destination, source_destination);
}

void ScrollFrame::PreComposition() {
	for (auto child : children) {
		child->composition_pos += scroll_direction == Orientation::VERTICAL ?
								  Vec2i(0, (int)scroll) : Vec2i((int)scroll, 0);
	}
}