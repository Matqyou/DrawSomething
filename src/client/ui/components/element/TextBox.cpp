//
// Created by Matq on 16/12/2024.
//

#include "TextBox.h"
#include "../../../core/Application.h"

#include <vector>
#include <string>
#include <stdexcept>
#include <SDL3_ttf/SDL_ttf.h>

LinkFont TextBox::sFontDefault("default");

std::vector<std::string> TextBox::WrapText(const std::string& text, TTF_Font* font, int max_width) {
    std::vector<std::string> lines;
    std::string current_line;
    std::string current_word;
    int measured_width = 0;
    size_t measured_length = 0;

    auto measure_text = [&](const std::string& str) -> int {
        if (TTF_MeasureString(font, str.c_str(), str.length(), max_width, &measured_width, &measured_length)) {
            return measured_width;
        } else {
            throw std::runtime_error("Failed to measure text: " + std::string(SDL_GetError()));
        }
    };

    for (char ch : text) {
        if (ch == ' ' || ch == '\n') {
            // Measure the current line with the new word appended
            int new_line_width = measure_text(current_line + current_word);

            if (new_line_width > max_width && !current_line.empty()) {
                lines.push_back(current_line);
                current_line.clear();
            }

            // Check if the word itself exceeds max_width
            int word_width = measure_text(current_word);
            if (word_width > max_width) {
                // Split the word into smaller chunks
                std::string split_word;
                for (size_t i = 0; i < current_word.size(); ++i) {
                    split_word += current_word[i];
                    int part_width = measure_text(split_word);

                    if (part_width > max_width) {
                        if (!current_line.empty()) {
                            lines.push_back(current_line);
                            current_line.clear();
                        }
                        current_line = current_word.substr(i); // Remaining part
                        break;
                    }
                }
            } else {
                // Append word to current line
                current_line += current_word + (ch == ' ' ? " " : "");
            }

            current_word.clear();
            if (ch == '\n') {
                lines.push_back(current_line);
                current_line.clear();
            }
        } else {
            current_word += ch;
        }
    }

    // Handle any remaining word or line
    if (!current_word.empty()) {
        int final_line_width = measure_text(current_line + current_word);

        if (final_line_width > max_width) {
            lines.push_back(current_line);
            current_line = current_word;
        } else {
            current_line += current_word;
        }
    }
    if (!current_line.empty()) {
        lines.push_back(current_line);
    }

    return lines;
}

TextBox::TextBox(const Vec2i& pos, const Vec2i& size, void (* enter_callback)(const std::string&))
    : Element(ELEMENT_TEXTBOX, pos, size, DRAW_RECT),
      enter_callback(enter_callback) {

    text_lines = {};
    update_render = true;
    text_color = { 0, 0, 0, 255 };
}

TextBox::~TextBox() {
    for (auto line_render : text_lines)
        delete line_render;
}

void TextBox::AppendText(const char* input) {
    text += input; // Append input string to the current text
}

void TextBox::Backspace() {
    if (!text.empty()) {
        text.pop_back(); // Remove the last character
    }
}

void TextBox::UpdateRender() {
//    delete text_render;
//    SDL_Surface* TempSurface = TTF_RenderText_Blended(sDefaultFont.GetFont()->TTFFont(), text.c_str(), text_color);
//    text_render = Assets::Get()->TextureFromSurface(TempSurface);
//    SDL_DestroySurface(TempSurface);

    for (auto line_render : text_lines)
        delete line_render;
    text_lines.clear();

    auto font = sFontDefault.GetFont()->TTFFont();
    auto lines = WrapText(text, font, 200);

    for (size_t i = 0; i < lines.size(); ++i) {
        SDL_Surface* surface = TTF_RenderText_Blended(font, lines[i].c_str(), lines[i].size(), text_color);
        Texture* line_render = Assets::Get()->TextureFromSurface(surface);
        SDL_DestroySurface(surface);
        text_lines.push_back(line_render);
    }
}

// Ticking
void TextBox::HandleEvent(SDL_Event& event, EventContext& event_summary) {
    switch (event.type) {
        case SDL_EVENT_MOUSE_MOTION: {
            if (event_summary.cursor_changed == CursorChange::NO_CHANGE &&
                PointCollides(event.motion.x, event.motion.y)) {
                event_summary.cursor_changed = CursorChange::TO_IBEAM;
                SDL_SetCursor(SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_TEXT));
            }

            break;
        }
        case SDL_EVENT_MOUSE_BUTTON_DOWN: {
            if (PointCollides(event.button.x, event.button.y)) {
                SetFocus(this);
                break;
            }

            break;
        }
        case SDL_EVENT_TEXT_INPUT: {
            if (!has_focus) break;

            AppendText(event.text.text);
            update_render = true;
            break;
        }
        case SDL_EVENT_KEY_DOWN: {
            if (!has_focus) break;

            if (event.key.key == SDLK_BACKSPACE) {
                Backspace();
                update_render = true;
            }
            else if (event.key.key == SDLK_RETURN) { enter_callback(text); }

            break;
        }
    }
}

void TextBox::PostEvent() {
    if (update_render) {
        update_render = false;
        UpdateRender();
    }
}

void TextBox::Render() const {
    auto drawing = Application::Get()->GetDrawing();
    auto& fill_color = has_focus ? focus_color : color;

    drawing->SetColor(fill_color);
    drawing->FillRect(GetRect());

    // Retrieve the font height
    int line_height = TTF_GetFontHeight(sFontDefault.GetFont()->TTFFont());

    auto render_rect = GetRect();
    for (const auto& line_render : text_lines) {
        render_rect.w = line_render->GetWidth();
        render_rect.h = line_render->GetHeight();
        drawing->RenderTexture(line_render->SDLTexture(), nullptr, render_rect);

        render_rect.y += line_height;
    }
}
