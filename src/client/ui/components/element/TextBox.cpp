//
// Created by Matq on 16/12/2024.
//

#include "TextBox.h"
#include "../../../core/Application.h"

LoadedFont TextBox::sDefaultFont("minecraft", 24);

std::vector<std::string> TextBox::WrapText(const std::string& text, TTF_Font* font, int max_width) {
    std::vector<std::string> lines;
    std::string current_line;
    std::string current_word;
    int word_width, word_height;

    for (char ch : text) {
        if (ch == ' ' || ch == '\n') {
            // Measure the current line with the new word appended
            TTF_SizeText(font, (current_line + current_word).c_str(), &word_width, &word_height);

            if (word_width > max_width && !current_line.empty()) {
                lines.push_back(current_line);
                current_line.clear();
            }

            // Check if the word itself exceeds max_width
            TTF_SizeText(font, current_word.c_str(), &word_width, &word_height);
            if (word_width > max_width) {
                // Split the word into smaller chunks
                for (size_t i = 0; i < current_word.size(); ++i) {
                    std::string part = current_word.substr(0, i + 1);
                    TTF_SizeText(font, part.c_str(), &word_width, &word_height);

                    if (word_width > max_width) {
                        lines.push_back(current_line);
                        current_line.clear();
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
        TTF_SizeText(font, (current_line + current_word).c_str(), &word_width, &word_height);

        if (word_width > max_width) {
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
//    SDL_FreeSurface(TempSurface);

    for (auto line_render : text_lines)
        delete line_render;
    text_lines.clear();

    auto font = sDefaultFont.GetFont()->TTFFont();
    auto lines = WrapText(text, font, 200);

    for (size_t i = 0; i < lines.size(); ++i) {
        SDL_Surface* surface = TTF_RenderText_Blended(font, lines[i].c_str(), text_color);
        Texture* line_render = Assets::Get()->TextureFromSurface(surface);
        SDL_FreeSurface(surface);
        text_lines.push_back(line_render);
    }
}

// Ticking
void TextBox::HandleEvent(SDL_Event& event, EventContext& event_summary) {
    switch (event.type) {
        case SDL_MOUSEMOTION: {
            if (event_summary.cursor_changed == CursorChange::NO_CHANGE &&
                PointCollides(event.motion.x, event.motion.y)) {
                event_summary.cursor_changed = CursorChange::TO_IBEAM;
                SDL_SetCursor(SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_IBEAM));
            }

            break;
        }
        case SDL_MOUSEBUTTONDOWN: {
            if (PointCollides(event.button.x, event.button.y)) {
                SetFocus(this);
                break;
            }

            break;
        }
        case SDL_TEXTINPUT: {
            if (!has_focus) break;

            AppendText(event.text.text);
            update_render = true;
            break;
        }
        case SDL_KEYDOWN: {
            if (!has_focus) break;

            if (event.key.keysym.sym == SDLK_BACKSPACE) {
                Backspace();
                update_render = true;
            }
            else if (event.key.keysym.sym == SDLK_RETURN) { enter_callback(text); }

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

    int line_height = TTF_FontHeight(sDefaultFont.GetFont()->TTFFont());
    auto render_rect = GetRect();
    for (int i = 0; i < text_lines.size(); i++) {
        auto line_render = text_lines[i];
        render_rect.w = line_render->GetWidth();
        render_rect.h = line_render->GetHeight();
        drawing->RenderTexture(line_render->SDLTexture(), nullptr, render_rect);

        render_rect.y += line_height;
    }
}
