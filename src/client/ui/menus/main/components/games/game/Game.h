//
// Created by Matq on 26/01/2025.
//

#pragma once

#include "../../../../../components/element/TextElement.h"
#include "../../../../../components/element/Frame.h"
#include "Statistics.h"

namespace Main {
class Game : public Frame {
private:
    Statistics* statistics;
    TextElement* turn_number;
    int turn_number_;

public:
    Game();
    ~Game() override;

    // Getting
    [[nodiscard]] int GetTurnNumber() const { return turn_number_; }

    // Manipulation
    void SetTurnNumber(int number);

};
}
