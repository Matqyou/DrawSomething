//
// Created by Matq on 03/02/2025.
//

#pragma once

#include "Frame.h"

class ScrollFrame : public Frame {
private:
    Orientation scroll_direction;
    int scroll_amount;

public:
    explicit ScrollFrame(Orientation scroll_direction);
    ~ScrollFrame();

};
