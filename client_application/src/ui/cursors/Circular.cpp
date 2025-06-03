//
// Created by Matq on 17/01/2025.
//

#include "Circular.h"
#include "SDL3/SDL_surface.h"
#include "core/app/Assets.h"
#include <cmath>


void SetPixel(SDL_Surface* surface, int x, int y, Uint32 color) {
    if (x >= 0 && x < surface->w && y >= 0 && y < surface->h) {
        Uint32* pixels = (Uint32*)surface->pixels;
        pixels[(y * surface->pitch / 4) + x] = color;
    }
}

void DrawCircle(SDL_Surface* surface, int centerX, int centerY, int radius, int thickness, Uint32 color) {
    // Ensure that the minimum radius is at least the thickness
    int adjustedRadius = std::max(radius, thickness);

    // Loop through the points within the bounding box of the circle
    for (int y = -adjustedRadius; y <= adjustedRadius; ++y) {
        for (int x = -adjustedRadius; x <= adjustedRadius; ++x) {
            // Calculate the distance from the center using the Euclidean distance formula
            float distance = std::sqrt(x * x + y * y);

            // Check if the point lies within the ring of the circle
            if (distance >= adjustedRadius - thickness && distance <= adjustedRadius) {
                SetPixel(surface, centerX + x, centerY + y, color);
            }
        }
    }
}

namespace Cursors {
int Circular::sCurrentCursorID = 0;

Circular::Circular() {
    this->custom_custor_id = sCurrentCursorID;
    this->diameter = 0;
    this->cursor = nullptr;

    sCurrentCursorID++;
}

Circular::~Circular() {
    SDL_DestroyCursor(this->cursor);
}

void Circular::Generate(float diameter, SDL_Color sdl_color) {
    this->diameter = diameter;
    auto assets = Assets::Get();

    int cursor_size = (int)diameter + 12;
    SDL_Surface* sdl_surface = assets->CreateSDLSurface(cursor_size,
                                                        cursor_size,
                                                        SDL_PIXELFORMAT_RGBA8888);
    auto details = SDL_GetPixelFormatDetails(sdl_surface->format);
    SDL_FillSurfaceRect(sdl_surface, nullptr, SDL_MapRGBA(details, nullptr, 0, 0, 0, 0));

    Uint32 white = SDL_MapRGBA(details, nullptr, 255, 255, 255, 255);
    Uint32 black = SDL_MapRGBA(details, nullptr, sdl_color.r, sdl_color.g, sdl_color.b, sdl_color.a);
    DrawCircle(sdl_surface, cursor_size / 2, cursor_size / 2, (int)(diameter / 2.0f), 3, black);
    DrawCircle(sdl_surface, cursor_size / 2, cursor_size / 2, (int)(diameter / 2.0f), 1, white);

    SDL_Cursor* custom_cursor = SDL_CreateColorCursor(sdl_surface, cursor_size / 2, cursor_size / 2);
    if (custom_cursor == nullptr) {
        std::wcout << Strings::FStringColorsW(L"[Cursors] &cCircular cursor could not be generated\n");
        std::wcout << Strings::FStringColorsW(L"[Cursors] &cReason: %s\n", SDL_GetError());
    }
    SDL_DestroySurface(sdl_surface);

    SDL_DestroyCursor(this->cursor);
    this->cursor = custom_cursor;
}
}
