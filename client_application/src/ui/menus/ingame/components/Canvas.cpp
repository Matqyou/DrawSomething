//
// Created by Matq on 10/01/2025.
//

#include "Canvas.h"
#include "core/Application.h"
#include "ui/cursors/Cursors.h"
#include "ui/CommonUI.h"

namespace Ingame
{
LinkTexture Canvas::sTextureGuess("game.canvas.guess", CommonUI::sCallbackScaleNearest);
LinkTexture Canvas::sTextureWatch("game.canvas.watch", CommonUI::sCallbackScaleNearest);
LinkTexture Canvas::sTextureDraw("game.canvas.draw", CommonUI::sCallbackScaleNearest);
LinkTexture sTextureDrawing("drawings.example", CommonUI::sCallbackScaleNearest);

void Canvas::EmplaceRecordingData(const Vec2f& segment_start, const Vec2f& segment_end)
{
	auto draw_offset = Vec2f(canvas_rect.x - canvas_source.x, canvas_rect.y - canvas_source.y);
	if (recording_data.empty())
		this->playback_start = std::chrono::steady_clock::now();

	auto duration = duration_cast<std::chrono::milliseconds>(
		std::chrono::steady_clock::now() - playback_start);
	auto milliseconds = duration.count();
	auto segment = std::pair(segment_start - draw_offset, segment_end - draw_offset);
	auto paint_color = tool == TOOL_PENCIL ? this->draw_color : this->eraser_color;
	auto line_thickness = tool == TOOL_PENCIL ? this->brush_size : this->eraser_size;
	this->recording_data.emplace_back(milliseconds, segment, paint_color.GetIColor(), line_thickness);
}

void Canvas::CopyRecordingToReplay(long long max_idle_milliseconds)
{
	replay_data.clear();
	if (recording_data.empty())
		return;

	long long current_time = 0;
	long long last_real_time = 0;
	for (auto data : recording_data)
	{
		auto real_time = std::get<0>(data);
		auto real_duration = real_time - last_real_time;

		// Advance time without weird spaces of nothing happening // todo: fix in the future
		current_time += real_duration < max_idle_milliseconds ? real_duration : max_idle_milliseconds;
		std::get<0>(data) = current_time;

		replay_data.push_back(data);
		last_real_time = real_time;
	}
}

Canvas::Canvas()
	: Element(),
	  draw_color(SDL_Color(0, 0, 0, 255)),
	  eraser_color(SDL_Color(233, 233, 233, 255))
{
	this->name = L"Canvas";

	this->instructions_intro = true;
	this->canvas_mode = CANVAS_GUESS;
	this->canvas = nullptr;

	auto assets = Assets::Get();
	auto drawing = Application::Get()->GetDrawing();

	// Instruction texts
	auto instructions_font = CommonUI::sFontInstructions.GetFont()->TTFFont();
	SDL_Color instructions_font_color = { 136, 136, 136, 255 };
	const char *guess_text_ = "TAP TO GUESS.";
	const char *watch_text_ = "TAP TO WATCH!";
	const char *draw_text_ = "TAP TO DRAW!";
	text_guess = assets->RenderTextBlended(instructions_font, guess_text_, instructions_font_color);
	text_watch = assets->RenderTextBlended(instructions_font, watch_text_, instructions_font_color);
	text_draw = assets->RenderTextBlended(instructions_font, draw_text_, instructions_font_color);

	// Instructions image scale
	scale_guess =
		Vec2i(Rectangles::ScaleToBounds(sTextureGuess.GetTexture()->GetOriginalSize(), Vec2f(400.0f, 400.0f)));
	scale_watch =
		Vec2i(Rectangles::ScaleToBounds(sTextureWatch.GetTexture()->GetOriginalSize(), Vec2f(300.0f, 300.0f)));
	scale_draw = Vec2i(Rectangles::ScaleToBounds(sTextureDraw.GetTexture()->GetOriginalSize(), Vec2f(400.0f, 400.0f)));

	after_intro_callback = []
	{ };
	resolution = Vec2i(950, 550);
	canvas_source = { 0, 0, (float)resolution.x, (float)resolution.y };
	canvas_rect = { (float)pos.x, (float)pos.y, (float)resolution.x, (float)resolution.y };
	canvas = new Texture(SDL_CreateTexture(drawing->Renderer(),
										   SDL_PIXELFORMAT_RGBA8888,
										   SDL_TEXTUREACCESS_TARGET,
										   resolution.x,
										   resolution.y));
	canvas->SetScaleMode(SDL_SCALEMODE_NEAREST);
	drawing->SetRenderTarget(canvas);
	drawing->SetColor(233, 233, 233, 255);
	drawing->Clear();
	drawing->SetRenderTarget(nullptr);

	this->painting = false;
	this->last_paint_position = Vec2f(0, 0);
	this->tool = TOOL_PENCIL;
	this->brush_size = 5.0f;
	this->eraser_size = 5.0f;
	this->custom_cursor = nullptr;

	this->canvas_playback = PlaybackMode::DO_NOTHING;
	last_recording_size = 0;
}

std::string Canvas::SerializeReplay()
{
	CopyRecordingToReplay(300);//

	std::string serialized;
	this->replay_iterator = replay_data.begin();
	for (; replay_iterator != replay_data.end(); replay_iterator++)
	{
		auto [milliseconds, segment, color, thickness] = *replay_iterator;
		auto [start, end] = segment;

		serialized += Strings::FString("%lld/%g/%g/%g/%g/%d/%d/%d/%g,",
									   milliseconds, start.x, start.y, end.x, end.y,
									   color.r, color.g, color.b, thickness);
	}
	return serialized;
}

void Canvas::LoadExample()
{
	auto drawing = Application::Get()->GetDrawing();
	drawing->SetRenderTarget(canvas);
	drawing->RenderTextureFullscreen(sTextureDrawing.GetTexture()->SDLTexture(), nullptr);
	drawing->SetRenderTarget(nullptr);
}

void Canvas::ClearCanvas()
{
	auto drawing = Application::Get()->GetDrawing();
	drawing->SetRenderTarget(canvas);
	drawing->SetColor(233, 233, 233, 255);
	drawing->Clear();
	drawing->SetRenderTarget(nullptr);

	paint_segments.clear();
}

void Canvas::SetTool(DrawTool tool)
{
	this->tool = tool;
}

void Canvas::SetDrawColor(SDL_Color color)
{
	this->draw_color.SetColor(color);
}

void Canvas::SetCurrentToolSize(float new_size)
{
	if (tool == TOOL_PENCIL) this->brush_size = new_size;
	else this->eraser_size = new_size;
}

void Canvas::LoadReplayData(const std::string& replay_load)
{
	recording_data.clear();
	std::stringstream ss(replay_load);
	std::string chunk;

	while (std::getline(ss, chunk, ',')) // each chunk ends with a comma
	{
		int64_t milliseconds;
		float start_x, start_y, end_x, end_y;
		int r, g, b;
		float thickness;

		int parsed = sscanf(chunk.c_str(), "%lld/%f/%f/%f/%f/%d/%d/%d/%f",
							&milliseconds, &start_x, &start_y, &end_x, &end_y,
							&r, &g, &b, &thickness);

		if (parsed != 9)
		{
			dbg_msg("Replay Failed to parse a chunk correctly (%d/9 fields)\n", parsed);
			continue; // or return/error depending on your needs
		}

		Vec2f start{ start_x, start_y };
		Vec2f end{ end_x, end_y };
		SDL_Color color{ static_cast<Uint8>(r), static_cast<Uint8>(g), static_cast<Uint8>(b), 255 };

		recording_data.emplace_back(milliseconds, std::make_pair(start, end), color, thickness);
	}
}

void Canvas::Tick(double elapsed_seconds)
{

}

void Canvas::HandleEvent(const SDL_Event& sdl_event, EventContext& event_summary)
{
	switch (sdl_event.type)
	{
		case SDL_EVENT_MOUSE_BUTTON_DOWN:
		{
			if (sdl_event.button.button == SDL_BUTTON_LEFT
				&& PointCollides((int)sdl_event.button.x, (int)sdl_event.button.y) &&
				!event_summary.rapid_context.event_captured)
			{
				event_summary.rapid_context.event_captured = true;

				if (instructions_intro)
				{
					instructions_intro = false;
					after_intro_callback();
				}
				else
				{
					painting = true;
					last_paint_position = Vec2f(sdl_event.button.x, sdl_event.button.y);
					paint_segments.emplace_back(last_paint_position, last_paint_position);

					// Recording
					if (canvas_playback == PlaybackMode::RECORD)
						EmplaceRecordingData(last_paint_position, last_paint_position);
				}
			}
			break;
		}
		case SDL_EVENT_MOUSE_MOTION:
		{
			if (custom_cursor != nullptr && event_summary.cursor_changed == CursorChange::NO_CHANGE &&
				Rectangles::PointCollides(sdl_event.motion.x, sdl_event.motion.y,
										  canvas_rect.x, canvas_rect.y,
										  canvas_rect.x + canvas_rect.w,
										  canvas_rect.y + canvas_rect.h))
			{
				if (canvas_mode == CANVAS_DRAW && !instructions_intro)
				{
					event_summary.cursor_changed = CursorChange::TO_CUSTOM;
					SDL_SetCursor(custom_cursor);
				}
			}

			if (painting)
			{
				auto mouse_position = Vec2f(sdl_event.motion.x, sdl_event.motion.y);
				paint_segments.emplace_back(last_paint_position, mouse_position);

				if (canvas_playback == PlaybackMode::RECORD)
					EmplaceRecordingData(last_paint_position, mouse_position);

				last_paint_position = mouse_position;
			}
			break;
		}
		case SDL_EVENT_MOUSE_BUTTON_UP:
		{
			if (sdl_event.button.button == SDL_BUTTON_LEFT)
				painting = false;
			break;
		}
	}
}

void Canvas::Render()
{
	auto drawing = Application::Get()->GetDrawing();

	drawing->RenderTexture(canvas->SDLTexture(), &canvas_source, canvas_rect);

	if (canvas_mode == CANVAS_DRAW && tool != TOOL_NONE)
	{
		drawing->SetRenderTarget(canvas);
		auto draw_offset = Vec2f(canvas_rect.x - canvas_source.x, canvas_rect.y - canvas_source.y);
		auto paint_color = tool == TOOL_PENCIL ? this->draw_color : this->eraser_color;
		auto line_thickness = tool == TOOL_PENCIL ? this->brush_size : this->eraser_size;
		for (const auto& segment : paint_segments)
		{
			auto [start, end] = segment;
			drawing->DrawLine((start - draw_offset),
							  (end - draw_offset),
							  line_thickness,
							  paint_color.GetFColor());
		}
		paint_segments.clear();
		drawing->SetRenderTarget(nullptr);
	}
	else if (canvas_mode == CANVAS_GUESS && canvas_playback == PlaybackMode::REPLAY)
	{
		auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(
			std::chrono::steady_clock::now() - playback_start).count();
		drawing->SetRenderTarget(canvas);
		for (; replay_iterator != replay_data.end(); replay_iterator++)
		{
			auto [milliseconds, segment, color, thickness] = *replay_iterator;
			if (elapsed * 2 < milliseconds)
				break;

			auto [start, end] = segment;
			drawing->DrawLine(start, end, thickness, Colors::ColorToFloat(color));
			// todo: pre Colors::ColorToFloat(color)
		}
		if (replay_iterator == replay_data.end())
			canvas_playback = PlaybackMode::DO_NOTHING;
		drawing->SetRenderTarget(nullptr);
	}

	// Draw instructions
	if (!instructions_intro)
		return;

	Vec2i intro_texture_scale;
	Texture *intro_texture = nullptr;
	Texture *text_texture = nullptr;
	switch (canvas_mode)
	{
		case CANVAS_GUESS:
		{
			intro_texture_scale = scale_guess;
			intro_texture = sTextureGuess.GetTexture();
			text_texture = text_guess;
			break;
		}
		case CANVAS_WATCH:
		{
			intro_texture_scale = scale_watch;
			intro_texture = sTextureWatch.GetTexture();
			text_texture = text_watch;
			break;
		}
		case CANVAS_DRAW:
		{
			intro_texture_scale = scale_draw;
			intro_texture = sTextureDraw.GetTexture();
			text_texture = text_draw;
			break;
		}
		default: throw std::runtime_error("Canvas.cpp Render()");
	}

	const float gap = 5.0f;

	float canvas_midpoint = (float)pos.x + (float)size.x / 2.0f;
	float centered_text_x = canvas_midpoint - text_texture->GetOriginalSize().x / 2.0f;
	float centered_instructions_x = canvas_midpoint - (float)intro_texture_scale.x / 2.0f;

	float total_height = text_texture->GetOriginalSize().y + gap + (float)intro_texture_scale.y;
	float text_y = (float)pos.y + (float)size.y / 2.0f - total_height / 2.0f;
	float instructions_y = text_y + text_texture->GetOriginalSize().y + gap;

	SDL_FRect text_rect = {
		centered_text_x,
		text_y,
		text_texture->GetOriginalSize().x,
		text_texture->GetOriginalSize().y,
	};
	drawing->RenderTexture(text_texture->SDLTexture(), nullptr, text_rect);

	SDL_FRect instructions_rect = {
		centered_instructions_x,
		instructions_y,
		(float)intro_texture_scale.x,
		(float)intro_texture_scale.y,
	};
	drawing->RenderTexture(intro_texture->SDLTexture(), nullptr, instructions_rect);
}

void Canvas::PostRefresh()
{
	auto visible_area = ClampMax(size, resolution);
	auto centered = Rectangles::CenterRelative(visible_area, size);
	canvas_source = Rectangles::CenterRelative(visible_area, resolution);

	canvas_rect = {
		(float)pos.x + centered.x,
		(float)pos.y + centered.y,
		(float)visible_area.x,
		(float)visible_area.y,
	};
}
}
