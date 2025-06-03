//
// Created by Matq on 20/01/2025.
//

#include "Sheet.h"
#include "../../../core/app/Drawing.h"

Sheet::Sheet() {
    this->sheet = nullptr;

    this->top_left = nullptr;
    this->top = nullptr;
    this->top_right = nullptr;
    this->left = nullptr;
    this->middle = nullptr;
    this->right = nullptr;
    this->bottom_left = nullptr;
    this->bottom = nullptr;
    this->bottom_right = nullptr;

    this->top_left_sdl = nullptr;
    this->top_sdl = nullptr;
    this->top_right_sdl = nullptr;
    this->left_sdl = nullptr;
    this->middle_sdl = nullptr;
    this->right_sdl = nullptr;
    this->bottom_left_sdl = nullptr;
    this->bottom_sdl = nullptr;
    this->bottom_right_sdl = nullptr;
}

Sheet::~Sheet() {
    delete top_left;
    delete top;
    delete top_right;
    delete left;
    delete middle;
    delete right;
    delete bottom_left;
    delete bottom;
    delete bottom_right;
}

void Sheet::Generate(AssetsClass* assets, Texture* sheet) {
    auto drawing = assets->GetDrawing();

    this->sheet = sheet;

    auto resolution = Vec2i(sheet->GetOriginalSize());
    if (resolution.x != resolution.y || resolution.x % 3 != 0)
        std::wcout << Strings::FStringColorsW(L"[Warning] &cSheet.h Sheet() resolution is not split equally\n");

    std::pair<Texture**, SDL_Texture**> sheet_order[9] = {
        std::pair(&top_left, &top_left_sdl),
        std::pair(&top, &top_sdl),
        std::pair(&top_right, &top_right_sdl),
        std::pair(&left, &left_sdl),
        std::pair(&middle, &middle_sdl),
        std::pair(&right, &right_sdl),
        std::pair(&bottom_left, &bottom_left_sdl),
        std::pair(&bottom, &bottom_sdl),
        std::pair(&bottom_right, &bottom_right_sdl),
    };

    auto tile_size = resolution / 3;
    for (int i = 0; i < 9; i++) {
        auto [tile, tile_sdl] = sheet_order[i];

        auto new_texture = assets->CreateTexture(SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, tile_size.x, tile_size.y);
        new_texture->SetScaleMode(SDL_SCALEMODE_NEAREST);

        int x = i % 3;
        int y = i / 3;

        SDL_FRect source_rect = {
            (float)(x * tile_size.x),
            (float)(y * tile_size.y),
            (float)(tile_size.x),
            (float)(tile_size.y),
        };

        drawing->SetRenderTarget(new_texture);
        drawing->RenderTextureFullscreen(this->sheet->SDLTexture(), &source_rect);

        *tile = new_texture;
        *tile_sdl = new_texture->SDLTexture();
    }
}
