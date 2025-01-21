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

    for (int i = 0; i < 12; i++) {
        auto new_letter = new Letter();
        new_letter->UpdateRender("", Letter::sBlueColorBackground, Letter::sBlueColor);
        new_letter->SetCallback([guessing_bar, new_letter]() {
            if (guessing_bar->AddLetter(new_letter)) {
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

        generate_for_letter->UpdateRender(std::string(1, character), Letter::sBlueColorBackground, Letter::sBlueColor);
    }
    for (; i < capacity; i++) {
        auto generate_for_letter = all_letters[i];

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
}
