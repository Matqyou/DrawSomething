//
// Created by Matq on 17/01/2025.
//

#pragma once

#include "SDL3/SDL_mouse.h"

namespace Cursors {
class Circular {
private:
    int custom_custor_id;
    float diameter;
    SDL_Cursor* cursor;

    static int sCurrentCursorID;

public:
    Circular();
    ~Circular();

    // Getting
    [[nodiscard]] int GetCustomCursorID() const { return custom_custor_id; }
    [[nodiscard]] SDL_Cursor* GetCursor() const { return cursor; }

    // Manipulating
    void Generate(float diameter);

};
}
