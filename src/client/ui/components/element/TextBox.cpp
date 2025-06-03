//
// Created by Matq on 16/12/2024.
//

#include "TextBox.h"
#include "../../../core/Application.h"
#include "../../cursors/Cursors.h"

#include <string>
#include <SDL3_ttf/SDL_ttf.h>

TextBox::TextBox()
    : Element(),
      text(nullptr, { 0, 0, 0, 255 }),
      placeholder(nullptr, { 200, 200, 200, 255 }) {
    this->name = L"TextBox";
	this->max_length = 100;

    // Core
    this->callback = [](std::string&) { };

    // Visual
    this->text_pos = pos;
    this->placeholder_pos = pos;
    this->update_text = true;
    this->update_placeholder = true;
    this->text_align_horizontally = SimpleAlign::DONT;
    this->text_align_vertically = SimpleAlign::DONT;
}

TextBox::~TextBox() {

}

void TextBox::UpdateTextPosition(WrappedText& update_text, Vec2i* out_pos) {
    // Align text
    switch (text_align_horizontally) {
        case SimpleAlign::DONT:
        case SimpleAlign::LEFT: {
            out_pos->x = pos.x;
            break;
        }
        case SimpleAlign::CENTER: {
            out_pos->x = (int)((float)pos.x + (float)(size.x - update_text.GetWidth()) / 2.0f);
            break;
        }
        case SimpleAlign::RIGHT: {
            out_pos->x = pos.x + size.x - update_text.GetWidth();
            break;
        }
    }
    switch (text_align_vertically) {
        case SimpleAlign::DONT:
        case SimpleAlign::TOP: {
            out_pos->y = pos.y;
            break;
        }
        case SimpleAlign::CENTER: {
            out_pos->y = (int)((float)pos.y + (float)(size.y - update_text.GetLineHeight()) / 2.0f);
            break;
        }
        case SimpleAlign::BOTTOM: {
            out_pos->y = pos.y + size.y - update_text.GetLineHeight();
            break;
        }
    }
}

// Ticking
void TextBox::HandleEvent(const SDL_Event& sdl_event, EventContext& event_summary) {
    if (event_summary.rapid_context.event_captured)
        return;

    switch (sdl_event.type) {
        case SDL_EVENT_MOUSE_MOTION: {
            if (event_summary.cursor_changed == CursorChange::NO_CHANGE &&
                PointCollides(sdl_event.motion.x, sdl_event.motion.y)) {
                event_summary.cursor_changed = CursorChange::TO_IBEAM;
                SDL_SetCursor(Cursors::sCursorSystemText);
            }

            break;
        }
        case SDL_EVENT_MOUSE_BUTTON_DOWN: {
            if (PointCollides(sdl_event.button.x, sdl_event.button.y))
                parent->SetFocus(this);

            break;
        }
		case SDL_EVENT_TEXT_INPUT: {
			if (!has_focus) break;

			const char* input = sdl_event.text.text;
			size_t currentLength = text.GetText().length();
			size_t inputLength = strlen(input);

			if (currentLength < max_length) {
				size_t availableSpace = max_length - currentLength;

				if (inputLength <= availableSpace) {
					text.AppendText(input);
				} else {
					text.AppendText(std::string(input, availableSpace).c_str());
				}

				update_text = true;
			}

			break;
		}

        case SDL_EVENT_KEY_DOWN: {
            if (!has_focus) break;

            if (sdl_event.key.key == SDLK_BACKSPACE) {
                text.Backspace();
                update_text = true;
            } else if (sdl_event.key.key == SDLK_RETURN) { callback(text.GetText()); }

            break;
        }
    }
}

void TextBox::PostEvent() {
    if (update_text) {
        update_text = false;
        text.UpdateRender();
        UpdateTextPosition(text, &text_pos);
    }
    if (update_placeholder) {
        update_placeholder = false;
        placeholder.UpdateRender();
        UpdateTextPosition(placeholder, &placeholder_pos);
    }
}

void TextBox::Render() {
    auto drawing = Application::Get()->GetDrawing();
    BaseRender();

    if (!text.GetText().empty() || has_focus) {
        SDL_FRect render_rect = {
            (float)text_pos.x,
            (float)text_pos.y,
            0, 0
        };
        float render_width = 0;
        auto render_height = (float)text.GetLineHeight();
        auto& lines = text.GetRenderLines();
        for (const auto& line_render : lines) {
            Vec2f render_size = line_render->GetOriginalSize();
            render_width = render_size.x;
            render_height = render_size.y;

            render_rect.w = render_width;
            render_rect.h = render_height;

            drawing->RenderTexture(line_render->SDLTexture(), nullptr, render_rect);
            render_rect.y += render_height;
        }

        if (has_focus) {
            auto cursor_y = lines.empty() ? render_rect.y : render_rect.y - render_height;
            auto cursor_top = Vec2f(render_rect.x + render_width, cursor_y);
            auto cursor_bottom = cursor_top + Vec2f(0, render_height);
            drawing->SetColor(text.GetTextColor());
            drawing->DrawLine(cursor_top, cursor_bottom);
        }
    } else {
        SDL_FRect render_rect = {
            (float)placeholder_pos.x,
            (float)placeholder_pos.y,
            0, 0
        };
        for (const auto& line_render : placeholder.GetRenderLines()) {
            Vec2f render_size = line_render->GetOriginalSize();
            auto render_height = render_size.y;
            render_rect.w = render_size.x;
            render_rect.h = render_height;

            drawing->RenderTexture(line_render->SDLTexture(), nullptr, render_rect);
            render_rect.y += render_height;
        }
    }
}

void TextBox::PostRefresh() {
    if (draw == DRAW_TEXTURE) // todo: weird
        UpdateTexturePlacement();

    UpdateTextPosition(text, &text_pos);
    UpdateTextPosition(placeholder, &placeholder_pos);
}
