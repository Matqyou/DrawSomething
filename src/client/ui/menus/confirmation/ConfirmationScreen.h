//
// Created by Matq on 31/01/2025.
//

#pragma once

#include "../FullscreenMenu.h"
#include "../../components/element/TextElement.h"

class ConfirmationScreen : public FullscreenMenu {
private:
    TextElement* question;

public:
    ConfirmationScreen();
    ~ConfirmationScreen() override;

    // Manipulating
    void Prompt(const char* prompt_question);

};
