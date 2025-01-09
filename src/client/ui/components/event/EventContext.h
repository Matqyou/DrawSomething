//
// Created by Matq on 16/12/2024.
//

#pragma once

enum CursorChange {
    NO_CHANGE,
    TO_ARROW,
    TO_HAND,
    TO_IBEAM,
    TO_POINTER
};

struct EventContext {
    bool had_mouse_motion;
    CursorChange cursor_changed;

    EventContext() {
        had_mouse_motion = false;
        cursor_changed = NO_CHANGE;
    }

};