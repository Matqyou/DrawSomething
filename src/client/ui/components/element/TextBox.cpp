//
// Created by Matq on 16/12/2024.
//

#include "TextBox.h"
#include "../../../core/Application.h"
#include "../../cursors/Cursors.h"

#include <string>
#include <SDL3_ttf/SDL_ttf.h>

TextBox::TextBox(const Vec2i& pos, const Vec2i& size)
    : Element(ELEMENT_TEXTBOX, pos, size, DRAW_RECT),
      text(nullptr, { 0, 0, 0, 255 }),
      placeholder(nullptr, { 200, 200, 200, 255 }) {
    // Core
    callback = [](std::string&) { };

    // Visual
    text_pos = pos;
    placeholder_pos = pos;
    update_text = true;
    update_placeholder = true;
    text_align_horizontally = SimpleAlign::DONT;
    text_align_vertically = SimpleAlign::DONT;
}

TextBox::TextBox(const Vec2i& pos, const Vec2i& size, const Vec2i& visual, const Vec2i& offset, Texture* texture)
    : Element(ELEMENT_TEXTBOX, pos, size, visual, offset, texture),
      text(nullptr, { 0, 0, 0, 255 }),
      placeholder(nullptr, { 200, 200, 200, 255 }){
    // Core
    callback = [](std::string&) { };

    // Visual
    text_pos = pos;
    placeholder_pos = pos;
    update_text = true;
    update_placeholder = true;
    text_align_horizontally = SimpleAlign::DONT;
    text_align_vertically = SimpleAlign::DONT;
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
void TextBox::HandleEvent(SDL_Event& event, EventContext& event_summary) {
    switch (event.type) {
        case SDL_EVENT_MOUSE_MOTION: {
            if (event_summary.cursor_changed == CursorChange::NO_CHANGE &&
                PointCollides(event.motion.x, event.motion.y)) {
                event_summary.cursor_changed = CursorChange::TO_IBEAM;
                SDL_SetCursor(Cursors::sCursorSystemText);
            }

            break;
        }
        case SDL_EVENT_MOUSE_BUTTON_DOWN: {
            if (PointCollides(event.button.x, event.button.y))
                parent->SetFocus(this);

            break;
        }
        case SDL_EVENT_TEXT_INPUT: {
            if (!has_focus) break;

            text.AppendText(event.text.text);
            update_text = true;
            break;
        }
        case SDL_EVENT_KEY_DOWN: {
            if (!has_focus) break;

            if (event.key.key == SDLK_BACKSPACE) {
                text.Backspace();
                update_text = true;
            } else if (event.key.key == SDLK_RETURN) { callback(text.GetText()); }

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
    if (draw != ElementDraw::DONT_DRAW) {
        auto& fill_color = has_focus ? focus_color : color;

        if (draw == ElementDraw::DRAW_RECT) {
            drawing->SetColor(fill_color);
            drawing->FillRect(GetRect());
        } else if (draw == ElementDraw::DRAW_TEXTURE) {
            drawing->RenderTexture(visual_texture->SDLTexture(), nullptr, GetVisualRect());
        }
    }

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
            render_width = line_render->GetWidth();
            render_height = line_render->GetHeight();

            render_rect.w = render_width;
            render_rect.h = render_height;

            drawing->RenderTexture(line_render->SDLTexture(), nullptr, render_rect);
            render_rect.y += render_height;
        }

        if (has_focus) {
            auto cursor_y = lines.empty() ? render_rect.y : render_rect.y - render_height;
            auto cursor_top = Vec2f(render_rect.x + render_width, cursor_y);
            auto cursor_bottom = cursor_top + Vec2f(0, render_height);
            drawing->SetColor(255, 255, 255, 255);
            drawing->DrawLine(cursor_top, cursor_bottom);
        }
    } else {
        SDL_FRect render_rect = {
            (float)placeholder_pos.x,
            (float)placeholder_pos.y,
            0, 0
        };
        for (const auto& line_render : placeholder.GetRenderLines()) {
            auto render_height = line_render->GetHeight();
            render_rect.w = line_render->GetWidth();
            render_rect.h = render_height;

            drawing->RenderTexture(line_render->SDLTexture(), nullptr, render_rect);
            render_rect.y += render_height;
        }
    }
}

void TextBox::PostRefresh() {
    UpdateTextPosition(text, &text_pos);
    UpdateTextPosition(placeholder, &placeholder_pos);
}
