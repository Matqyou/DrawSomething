//
// Created by Matq on 03/02/2025.
//

#include "ScrollFrame.h"

ScrollFrame::ScrollFrame(Orientation scroll_direction)
 : Frame() {
    this->name = L"ScrollFrame";

    this->scroll_direction = scroll_direction;
    this->scroll_amount = 0;
}

ScrollFrame::~ScrollFrame() {

}
