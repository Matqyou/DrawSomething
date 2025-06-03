//
// Created by Matq on 17/01/2025.
//

#include "Cursors.h"

#include "core/Strings.h"

namespace Cursors {
SDL_Cursor* sCursorSystemText = nullptr;
SDL_Cursor* sCursorSystemPointer = nullptr;

void Initialize() {
    sCursorSystemText = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_TEXT);
    sCursorSystemPointer = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_POINTER);
    std::wcout << Strings::FStringColorsW(L"[Cursors] &5Loaded 2 cursors\n");
}

void Deinitialize() {
    SDL_DestroyCursor(sCursorSystemText);
    SDL_DestroyCursor(sCursorSystemPointer);
    std::wcout << Strings::FStringColorsW(L"[Cursors] &5Unloaded 2 cursors\n");
}
}