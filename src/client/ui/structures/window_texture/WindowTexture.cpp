//
// Created by Matq on 19/01/2025.
//

#include "WindowTexture.h"
#include "../../../core/app/Drawing.h"

WindowTexture::WindowTexture(Sheet* sheet, float left_scaling, float right_scaling, float top_scaling, float bottom_scaling) {
    this->sheet = sheet;
    this->generated = nullptr;
    this->generate_size = Vec2f(0.0f, 0.0f);

    this->left_scaling = left_scaling;
    this->right_scaling = right_scaling;
    this->top_scaling = top_scaling;
    this->bottom_scaling = bottom_scaling;
}

WindowTexture::~WindowTexture() {
    delete generated;
}

WindowTexture* WindowTexture::Generate(const Vec2f& new_size) {
    auto assets = Assets::Get();
    auto drawing = assets->GetDrawing();

    this->generate_size = new_size;

    delete generated;
    generated = assets->CreateTexture(SDL_PIXELFORMAT_RGBA8888,
                                      SDL_TEXTUREACCESS_TARGET,
                                      (int)generate_size.x,
                                      (int)generate_size.y);

    drawing->SetRenderTarget(generated);
    if (top_scaling > 0.0f) {
        if (left_scaling > 0.0f) {
            SDL_FRect top_left_rect = { 0, 0, left_scaling, top_scaling };
            drawing->RenderTexture(sheet->top_left_sdl, nullptr, top_left_rect);
        }
        SDL_FRect top_rect = { left_scaling, 0, generate_size.x - left_scaling - right_scaling, top_scaling };
        drawing->RenderTexture(sheet->top_sdl, nullptr, top_rect);
        if (right_scaling > 0.0f) {
            SDL_FRect top_right_rect = { generate_size.x - right_scaling, 0, right_scaling, top_scaling };
            drawing->RenderTexture(sheet->top_right_sdl, nullptr, top_right_rect);
        }
    }

    if (left_scaling > 0.0f) {
        SDL_FRect left_rect = { 0, top_scaling, left_scaling, generate_size.y - top_scaling - bottom_scaling };
        drawing->RenderTexture(sheet->left_sdl, nullptr, left_rect);
    }

    SDL_FRect middle_rect = { left_scaling, top_scaling, generate_size.x - left_scaling - right_scaling,
                              generate_size.y - top_scaling - bottom_scaling };
    drawing->RenderTexture(sheet->middle_sdl, nullptr, middle_rect);

    if (right_scaling > 0.0f) {
        SDL_FRect right_rect =
            { generate_size.x - right_scaling, top_scaling, right_scaling,
              generate_size.y - top_scaling - bottom_scaling };
        drawing->RenderTexture(sheet->right_sdl, nullptr, right_rect);
    }

    if (bottom_scaling > 0.0f) {
        if (left_scaling > 0.0f) {
            SDL_FRect bottom_left_rect = { 0, generate_size.y - bottom_scaling, left_scaling, bottom_scaling };
            drawing->RenderTexture(sheet->bottom_left_sdl, nullptr, bottom_left_rect);
        }
        SDL_FRect bottom_rect =
            { left_scaling, generate_size.y - bottom_scaling, generate_size.x - left_scaling - right_scaling,
              bottom_scaling };
        drawing->RenderTexture(sheet->bottom_sdl, nullptr, bottom_rect);
        if (right_scaling > 0.0f) {
            SDL_FRect bottom_right_rect =
                { generate_size.x - right_scaling, generate_size.y - bottom_scaling, right_scaling, bottom_scaling };
            drawing->RenderTexture(sheet->bottom_right_sdl, nullptr, bottom_right_rect);
        }
    }
    drawing->SetRenderTarget(nullptr);

    return this;
}
