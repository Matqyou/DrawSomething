//
// Created by Matq on 26/01/2025.
//

#pragma once

#include "../../../../components/element/Frame.h"

namespace Main {
class Games : public Frame {
private:
    Frame* games;

public:
    Games();
    ~Games();

    // Manipulation
    void SortGames();

};
}
