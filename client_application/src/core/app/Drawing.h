//
// Created by Matq on 26/03/2023.
//

#pragma once

#include "core/Vec2.h"
#include "SDL3_image/SDL_image.h"
#include "Assets.h"
#include <cmath>

class Drawing {
private:
    SDL_Renderer* m_Renderer;

    static LinkFont sFontMath;

public:
    explicit Drawing(SDL_Renderer* renderer);
    ~Drawing();

    // Getting
    [[nodiscard]] SDL_Renderer* Renderer() const { return m_Renderer; }

    // Setting
    void SetRenderTarget(Texture* target);
    void SetRenderTargetSDL(SDL_Texture* target);
    void SetDrawBlendMode(SDL_BlendMode blend_mode) { SDL_SetRenderDrawBlendMode(m_Renderer, blend_mode); }
    void SetColor(Uint8 r, Uint8 g, Uint8 b, Uint8 a) { SDL_SetRenderDrawColor(m_Renderer, r, g, b, a); }
    void SetColor(SDL_Color color) { SetColor(color.r, color.g, color.b, color.a); }

    // Drawing
//    void DrawRect(const SDL_Rect& rect);
//    void FillRect(const SDL_Rect& rect);
    void DrawRect(const SDL_FRect& rect);
    void FillRect(const SDL_FRect& rect);
    void FillCircle(const Vec2f& center, float radius, SDL_FColor color);
    void DrawLine(const Vec2f& start, const Vec2f& end);
    void DrawLine(const Vec2f& start, const Vec2f& end, double size, SDL_FColor color);
    void DrawDegrees(const Vec2d& point_a, const Vec2d& point_b, SDL_Color text_color = { 255, 255, 255, 255 });
    void DrawAnglemark(const Vec2d& point_a,
                       const Vec2d& point_b,
                       const Vec2d& point_c,
                       double radius,
                       SDL_Color text_color = { 255, 255, 255, 255 });
    void RenderTexture(SDL_Texture* texture, SDL_FRect* srcrect, const SDL_FRect& dstrect);
    void RenderTextureFullscreen(SDL_Texture*, SDL_FRect* srcrect = nullptr);
    void RenderTextureEx(SDL_Texture* texture,
                         SDL_FRect* srcrect,
                         const SDL_FRect& dstrect,
                         double angle,
                         SDL_FPoint* center,
                         SDL_FlipMode flip);
    void Clear();  // Resets all pixels to a color value
    void FillAll();  // If color blending is enabled, fill the window with a semi-transparent tone
    void UpdateWindow();  // Pushes the latest state of the pixels to window

};
