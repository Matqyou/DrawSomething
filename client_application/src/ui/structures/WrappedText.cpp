//
// Created by Matq on 19/01/2025.
//

#include "WrappedText.h"

std::vector<std::string> WrappedText::WrapText(const std::string& text,
                                           TTF_Font* font,
                                           int max_width,
                                           Vec2i* out_size) {
    int measured_width = 0;
    size_t measured_length = 0;
    int line_height = TTF_GetFontHeight(font);

    auto measure_text = [&](const std::string& str) -> int {
        if (TTF_MeasureString(font, str.c_str(), str.length(), max_width, &measured_width, &measured_length)) {
            return measured_width;
        } else {
            throw std::runtime_error("Failed to measure text: " + std::string(SDL_GetError()));
        }
    };

    int result_width = 0; // Tracks the longest line width
    std::vector<std::string> lines;
    std::string current_line;
    std::string current_word;

    for (char ch : text) {
        if (ch == ' ' || ch == '\n') {
            // Measure the current line with the new word appended
            int new_line_width = measure_text(current_line + current_word);

            if (new_line_width > max_width && !current_line.empty()) {
                result_width = std::max(result_width, measure_text(current_line));
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
                            result_width = std::max(result_width, measure_text(current_line));
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
                result_width = std::max(result_width, measure_text(current_line));
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
            result_width = std::max(result_width, measure_text(current_line));
            lines.push_back(current_line);
            current_line = current_word;
        } else {
            current_line += current_word;
        }
    }
    if (!current_line.empty()) {
        result_width = std::max(result_width, measure_text(current_line));
        lines.push_back(current_line);
    }

    // Calculate the total height
    int result_height = lines.size() * line_height;

    // Update out_size if provided
    if (out_size != nullptr)
        *out_size = Vec2i(result_width, result_height);

    return lines;
}

WrappedText::WrappedText(TTF_Font* font, SDL_Color text_color) {
    this->text = "";
    this->font = font;
    this->text_lines = { };
    this->text_lines_size = Vec2i(0, 0);
    this->text_color = text_color;
    this->filter = nullptr;
}

WrappedText::~WrappedText() {
    for (auto line_render : text_lines)
        delete line_render;
}

void WrappedText::UpdateRender() {
    for (auto line_render : text_lines)
        delete line_render;
    text_lines.clear();

    std::string draw_text = text;
    if (filter != nullptr) draw_text = filter(text);

    auto lines = WrapText(draw_text, font, 200, &text_lines_size);
    for (const auto & line : lines) {
        SDL_Surface* surface = TTF_RenderText_Blended(font, line.c_str(), line.size(), text_color);
        Texture* line_render = Assets::Get()->TextureFromSurface(surface);
        SDL_DestroySurface(surface);
        text_lines.push_back(line_render);
    }
}

void WrappedText::AppendText(const char* input) {
    text += input;
}

void WrappedText::Backspace() {
    if (!text.empty())
        text.pop_back();
}
