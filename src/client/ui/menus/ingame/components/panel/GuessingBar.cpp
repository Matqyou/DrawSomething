//
// Created by Matq on 21/01/2025.
//

#include "GuessingBar.h"
#include "Letter.h"

auto static sCallbackGuessingBar = [](Texture* texture) {
    texture->SetColorMod(100, 190, 255);
    texture->SetAlphaMod(170);
    texture->SetBlendMode(SDL_BLENDMODE_BLEND);
};

namespace Ingame {
PreloadTexture GuessingBar::game_guessing_bar("game.ingame_panel.guessing_bar", sCallbackGuessingBar);

GuessingBar::GuessingBar()
    : Frame(Vec2i(0, 0),
            Vec2i(0, 90),
            game_guessing_bar.GetTexture()) {
    guess_word = "not set";

    // Guessing letter frame
    guessing_frame = (Frame*)(new Frame(Vec2i(0, 0),
                                        Vec2i(0, 0),
                                        DONT_DRAW))
        ->SetAlign(Align::CENTER, Align::CENTER)
        ->SetAdaptive(true, true)
        ->SetFlex(Flex::WIDTH, 10);

    SetFullyOccupy(true, false);
    SetFlexInvolved(true, false);
    SetAlign(Align::DONT, Align::ABOVE_TOP);
    SetName("Guessing");
    AddChildren({ guessing_frame });
}

GuessingBar::~GuessingBar() {

}

bool GuessingBar::IsAnsweredCorrectly() {
    std::string user_answer;
    for (auto slot : letter_slots) {
        auto occupy_letter = slot->GetOccupyLetter();
        if (occupy_letter != nullptr)
            user_answer += occupy_letter->GetLetter();
    }

    return user_answer == guess_word;
}

void GuessingBar::GenerateForWord(const std::string& word) {
    for (auto slot : letter_slots)
        delete slot;
    letter_slots.clear();

    guess_word = word;
    auto num_slots = word.length();
    for (int i = 0; i < num_slots; i++) {
        auto new_slot = new LetterSlot();
        new_slot->UpdateRender(LetterSlot::sColorBlue);
        new_slot->SetCallback([new_slot]() { new_slot->RemoveLetter(); });
        letter_slots.push_back(new_slot);
    }

    std::vector<Element*> letter_slots_as_elements(letter_slots.begin(), letter_slots.end());
    guessing_frame->SetChildren(letter_slots_as_elements);
}

bool GuessingBar::AddLetter(Letter* new_letter) {
    size_t used_slots = 0;
    bool find_available = true;
    for (auto slot : letter_slots) {
        if (slot->GetOccupyLetter() == nullptr) {
            if (find_available) {
                find_available = false;
                slot->OccupyLetter(new_letter);
                slot->SetClickable(true);
            }
        } else { used_slots++; }
    }

    if (used_slots == letter_slots.size() - 1 &&
        IsAnsweredCorrectly())
        RunCallback();

    return !find_available;
}

void GuessingBar::RunCallback() const {
    if (callback != nullptr)
        callback();
}
}
