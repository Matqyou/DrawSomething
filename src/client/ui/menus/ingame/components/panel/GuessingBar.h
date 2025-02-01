//
// Created by Matq on 21/01/2025.
//

#pragma once

#include "../../../../components/element/Frame.h"
#include "LetterSlot.h"

namespace Ingame {
class Letter;
class GuessingBar : public Frame {
private:
    Frame* complete_text;
    Frame* guessing_frame;
    std::vector<LetterSlot*> letter_slots;
    std::string guess_word;

    using Callback = std::function<void()>;
    Callback callback;

public:
    GuessingBar();
    ~GuessingBar();

    // Options
    GuessingBar* SetCallback(Callback callback) {
        this->callback = callback;
        return this;
    }

    // Generating
    [[nodiscard]] bool IsAnsweredCorrectly();

    // Manipulation
    void GenerateForWord(const std::string& word);
    bool AddLetter(Letter* new_letter);
    void RemoveLastLetter();
    void RunCallback() const;

    // Ticking
    void HandleEvent(SDL_Event& event, EventContext& event_summary) override;

};
}
