//
// Created by Matq on 26/03/2023.
//

#pragma once

#include "SDL_image.h"
#include <cmath>
#include "Assets.h"
#include "../../../shared/core/Vec2.h"

class Drawing {
private:
    SDL_Renderer* m_Renderer;
    double m_Zoom;

    static LoadedFont sAngleFont;

public:
    explicit Drawing(SDL_Renderer* renderer);
    ~Drawing();

    // Getting
    [[nodiscard]] SDL_Renderer* Renderer() const { return m_Renderer; }
    [[nodiscard]] double GetZoom() const { return m_Zoom; }

    // Setting
    void SetRenderTarget(Texture* target);
    void SetZoom(double zoom) { m_Zoom = zoom; }
    void SetDrawBlendMode(SDL_BlendMode blend_mode) { SDL_SetRenderDrawBlendMode(m_Renderer, blend_mode); }
    void SetColor(Uint8 r, Uint8 g, Uint8 b, Uint8 a) { SDL_SetRenderDrawColor(m_Renderer, r, g, b, a); }
    void SetColor(SDL_Color color) { SetColor(color.r, color.g, color.b, color.a); }

    // Drawing
    void DrawRect(const SDL_Rect& rect);
    void FillRect(const SDL_Rect& rect);
    void FillCircle(const Vec2d& center, double radius, SDL_Color color);
    void DrawLine(const Vec2i& start, const Vec2i& end);
    void DrawLine(const Vec2d& start, const Vec2d& end);
    void DrawLine(const Vec2d& start, const Vec2d& end, double size, SDL_Color color);
    void DrawDegrees(const Vec2d& point_a, const Vec2d& point_b, SDL_Color text_color = { 255, 255, 255, 255 });
    void DrawAnglemark(const Vec2d& point_a,
                       const Vec2d& point_b,
                       const Vec2d& point_c,
                       double radius,
                       SDL_Color text_color = { 255, 255, 255, 255 });
    void RenderTexture(SDL_Texture* texture, SDL_Rect* srcrect, const SDL_Rect& dstrect);
    void RenderTextureF(SDL_Texture* texture, SDL_Rect* srcrect, const SDL_FRect& dstrect);
    void RenderTextureFullscreen(SDL_Texture*, SDL_Rect* srcrect);
    void RenderTextureEx(SDL_Texture* texture,
                         SDL_Rect* srcrect,
                         const SDL_Rect& dstrect,
                         double angle,
                         SDL_Point* center,
                         SDL_RendererFlip flip);
    void Clear();  // Resets all pixels to a color value
    void FillAll();  // If color blending is enabled, fill the window with a semi-transparent tone
    void UpdateWindow();  // Pushes the latest state of the pixels to window

};
