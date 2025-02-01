//
// Created by Matq on 21/01/2025.
//

#include "Letters.h"
#include "Letter.h"
#include "GuessingBar.h"
#include <algorithm>
#include <random>

namespace Ingame {
Letters::Letters(GuessingBar* guessing_bar)
    : Frame(Vec2i(0, 0),
            Vec2i(0, 0),
            DONT_DRAW) {
    guess_word = "not set";
    guessing_bar_ = guessing_bar;

    for (int i = 0; i < 12; i++) {
        auto new_letter = new Letter();
        new_letter->UpdateRender('#', Letter::sBlueColorBackground, Letter::sBlueColor);
        new_letter->SetCallback([this, new_letter]() {
            if (guessing_bar_->AddLetter(new_letter)) {
                new_letter->SetDraw(ElementDraw::DONT_DRAW);
                new_letter->SetClickable(false);
            }
        });
        all_letters.push_back(new_letter);
    }

    size_t letters_midpoint = all_letters.size() / 2;
    std::vector<Element*> bottom_row_letters(all_letters.begin(), all_letters.begin() + letters_midpoint);
    std::vector<Element*> top_row_letters(all_letters.begin() + letters_midpoint, all_letters.end());

    // Top row
    top_row = (Frame*)(new Frame(Vec2i(0, 0),
                                 Vec2i(66, 66),
                                 DONT_DRAW))
        ->SetAlign(Align::CENTER, Align::DONT)
        ->SetFlex(Flex::WIDTH, 5)
        ->SetAdaptive(true, true)
        ->SetName("TopRow")
        ->AddChildren(top_row_letters);

    // Bottom row
    bottom_row = (Frame*)(new Frame(Vec2i(0, 0),
                                    Vec2i(66, 66),
                                    DONT_DRAW))
        ->SetAlign(Align::CENTER, Align::DONT)
        ->SetFlex(Flex::WIDTH, 5)
        ->SetAdaptive(true, true)
        ->SetName("BottomRow")
        ->AddChildren(bottom_row_letters);

//    RandomizeWord("TESTING");

    SetAlign(Align::CENTER, Align::CENTER);
    SetAdaptive(true, true);
    SetFlex(Flex::HEIGHT, 5);
    SetName("AllRows");
    AddChildren({ top_row, bottom_row });
}

Letters::~Letters() {

}

bool Letters::RandomizeWord(const std::string& word) {
    guess_word = word;

    auto word_length = word.length();
    auto capacity = all_letters.size();
    if (capacity < word_length) {
        std::wcout << Strings::FStringColorsW(L"[Letters] &cThe word '%s' is longer than there are allocated slots\n",
                                              word.c_str());
        return false;
    }

    size_t i = 0;
    for (; i < word_length; i++) {
        auto character = word[i];
        auto generate_for_letter = all_letters[i];
        generate_for_letter->ResetLetter();
        generate_for_letter->UpdateRender(character, Letter::sBlueColorBackground, Letter::sBlueColor);
    }
    for (; i < capacity; i++) {
        auto generate_for_letter = all_letters[i];
        generate_for_letter->ResetLetter();
        generate_for_letter->UpdateRender(Strings::RandomUppercaseLetter(),
                                          Letter::sBlueColorBackground,
                                          Letter::sBlueColor);
    }

    std::random_device rd;
    std::mt19937 generator(rd());
    std::shuffle(all_letters.begin(), all_letters.end(), generator);

    size_t letters_midpoint = all_letters.size() / 2;
    std::vector<Element*> top_row_letters(all_letters.begin(), all_letters.begin() + letters_midpoint);
    std::vector<Element*> bottom_row_letters(all_letters.begin() + letters_midpoint, all_letters.end());
    top_row->SetChildren(top_row_letters);
    bottom_row->SetChildren(bottom_row_letters);

    return true;
}

void Letters::BlowUp() {
    std::vector<Letter*> excess = all_letters;
    for (char letter : guess_word) {
        auto it = std::find_if(excess.begin(),
                               excess.end(),
                               [letter](Letter* object) { return object->GetLetter() == letter; });

        if (it != excess.end())
            excess.erase(it);
    }

    auto random_index = std::rand() % excess.size();
    excess.erase(excess.begin() + random_index);

    for (auto letter : excess) {
        LetterSlot* occupied_slot = letter->GetOccupySlot();
        if (occupied_slot != nullptr) {
            occupied_slot->OccupyLetter(nullptr);
            occupied_slot->SetClickable(false);
        } else {
            letter->SetDraw(ElementDraw::DONT_DRAW);
            letter->SetClickable(false);
        }
    }
}

void Letters::HandleEvent(SDL_Event& event, EventContext& event_summary) {
    HandleEventChildren(event, event_summary);

    switch (event.type) {
        case SDL_EVENT_TEXT_INPUT: {
            char pressed_character = (char)std::toupper((int)event.text.text[0]);
            for (auto letter : all_letters) {
                if (letter->GetOccupySlot() != nullptr || letter->draw == ElementDraw::DONT_DRAW)
                    continue;

                if (letter->GetLetter() == pressed_character) {
                    if (guessing_bar_->AddLetter(letter)) {
                        letter->SetDraw(ElementDraw::DONT_DRAW);
                        letter->SetClickable(false);
                    }
                    break;
                }
            }
            break;
        }
    }
}
}
