//
// Created by Matq on 26/03/2023.
//

#include "Drawing.h"

LoadedFont Drawing::sAngleFont("computer_modern.cmunrm", 16);

Drawing::Drawing(SDL_Renderer* renderer) {
    m_Renderer = renderer;
    m_Zoom = 0.5;
}

Drawing::~Drawing() = default;

void Drawing::SetRenderTarget(Texture* target) {
    if (target) SDL_SetRenderTarget(m_Renderer, target->SDLTexture());
    else SDL_SetRenderTarget(m_Renderer, nullptr);
}

void Drawing::DrawRect(const SDL_Rect& rect) {
    SDL_RenderDrawRect(m_Renderer, &rect);
}

void Drawing::FillRect(const SDL_Rect& rect) {
    SDL_RenderFillRect(m_Renderer, &rect);
}

void Drawing::FillCircle(const Vec2d& center, double radius, SDL_Color color) {
    const int segments = 100; // Number of segments for the circle
    float angle_step = 2.0f * M_PI / segments;

    for (int i = 0; i < segments; i++) {
        float theta1 = i * angle_step;
        float theta2 = (i + 1) * angle_step;

        // Points on the circle
        float x1 = center.x + radius * cos(theta1);
        float y1 = center.y + radius * sin(theta1);
        float x2 = center.x + radius * cos(theta2);
        float y2 = center.y + radius * sin(theta2);

        // Draw triangles for the circle
        SDL_Vertex vertices[3] = {
            { (float)center.x, (float)center.y, color, { 0, 0 }},
            { (float)x1, (float)y1, color, { 0, 0 }},
            { (float)x2, (float)y2, color, { 0, 0 }}
        };

        SDL_RenderGeometry(m_Renderer, nullptr, vertices, 3, nullptr, 0);
    }
}

void Drawing::DrawLine(const Vec2i& start, const Vec2i& end) {
    SDL_RenderDrawLine(m_Renderer, start.x, start.y, end.x, end.y);
}

void Drawing::DrawLine(const Vec2d& start, const Vec2d& end) {
    SDL_RenderDrawLineF(m_Renderer, (float)start.x, (float)start.y, (float)end.x, (float)end.y);
}

void Drawing::DrawLine(const Vec2d& start, const Vec2d& end, double size, SDL_Color color) {
    float half_thickness = size / 2.0f;

    // Vector between start and end
    Vec2d direction = (end - start).Normalize();

    // Perpendicular vector to the direction
    Vec2d perp_dir(-direction.y, direction.x);

    // Calculate the vertices of the thick line rectangle
    Vec2d p1 = start + perp_dir * half_thickness;
    Vec2d p2 = start - perp_dir * half_thickness;
    Vec2d p3 = end - perp_dir * half_thickness;
    Vec2d p4 = end + perp_dir * half_thickness;

    SDL_Vertex vertices[4] = {
        { (float)p1.x, (float)p1.y, color, { 0, 0 }},  // Vertex 0
        { (float)p2.x, (float)p2.y, color, { 0, 0 }},  // Vertex 1
        { (float)p3.x, (float)p3.y, color, { 0, 0 }},  // Vertex 2
        { (float)p4.x, (float)p4.y, color, { 0, 0 }}   // Vertex 3
    };

    int indices[6] = {
        0, 1, 2,  // First triangle (p1 -> p2 -> p3)
        0, 2, 3   // Second triangle (p1 -> p3 -> p4)
    };

    SDL_RenderGeometry(m_Renderer, nullptr, vertices, 4, indices, 6);


    // Draw the rounded caps
    FillCircle(start, half_thickness, color);
    FillCircle(end, half_thickness, color);
}

void Drawing::DrawDegrees(const Vec2d& point_a, const Vec2d& point_b, SDL_Color text_color) {
    double radians = (point_b - point_a).Atan2();

    auto angle_text = Strings::FString("%.1f°", round(radians / M_PI * 1800.0) / 10.0);
    auto text_surface = TTF_RenderUTF8_Blended(sAngleFont.GetFont()->TTFFont(),
                                               angle_text.c_str(),
                                               text_color);
    auto texture = SDL_CreateTextureFromSurface(m_Renderer, text_surface);
    int width, height;
    SDL_QueryTexture(texture, nullptr, nullptr, &width, &height);
    SDL_FreeSurface(text_surface);

    auto text_position = point_a + (point_a - point_b).Normalize() * 10;
    SDL_FRect text_rect = {
        (float)(text_position.x - (double)width / 2.0),
        (float)(text_position.y - (double)height / 2.0),
        (float)width,
        (float)height
    };
    RenderTextureF(texture, nullptr, text_rect);
    SDL_DestroyTexture(texture);
}

void Drawing::DrawAnglemark(const Vec2d& point_a,
                            const Vec2d& point_b,
                            const Vec2d& point_c,
                            double radius,
                            SDL_Color text_color) {
    double radians_a = (point_a - point_b).Atan2();
    double radians_c = (point_c - point_b).Atan2();
    double radians = radians_a - radians_c;

    if (radians > M_PI)
        radians -= M_PI * 2;
    else if (radians < -M_PI)
        radians += M_PI * 2;

    auto direction = std::copysign(1.0, -radians);
    auto abs_radians = std::abs(radians);
    for (auto current = 0.0; current <= abs_radians; current += 1.0 / 180.0 * M_PI) {
        double x = std::cos(radians_a + current * direction) * radius;
        double y = std::sin(radians_a + current * direction) * radius;

        auto point = point_b + Vec2d(x, y);
        SDL_RenderDrawPoint(m_Renderer, (int)point.x, (int)point.y);
    }

    auto angle_text = Strings::FString("%.1f°", round(radians / M_PI * 1800.0) / 10.0);
    auto text_surface = TTF_RenderUTF8_Blended(sAngleFont.GetFont()->TTFFont(),
                                               angle_text.c_str(),
                                               text_color);
    auto texture = SDL_CreateTextureFromSurface(m_Renderer, text_surface);
    int width, height;
    SDL_QueryTexture(texture, nullptr, nullptr, &width, &height);
    SDL_FreeSurface(text_surface);

    auto text_position = point_b + (((point_a - point_b) + (point_c - point_b)) / 2.0).Normalize() * (radius + 10.0);
    SDL_FRect text_rect = {
        (float)(text_position.x - (double)width / 2.0),
        (float)(text_position.y - (double)height / 2.0),
        (float)width,
        (float)height
    };
    RenderTextureF(texture, nullptr, text_rect);
    SDL_DestroyTexture(texture);
}

void Drawing::RenderTexture(SDL_Texture* texture, SDL_Rect* srcrect, const SDL_Rect& dstrect) {
    SDL_RenderCopy(m_Renderer, texture, srcrect, &dstrect);
}

void Drawing::RenderTextureF(SDL_Texture* texture, SDL_Rect* srcrect, const SDL_FRect& dstrect) {
    SDL_RenderCopyF(m_Renderer, texture, srcrect, &dstrect);
}

void Drawing::RenderTextureFullscreen(SDL_Texture* texture, SDL_Rect* srcrect) {
    SDL_RenderCopy(m_Renderer, texture, srcrect, nullptr);
}

void Drawing::RenderTextureEx(SDL_Texture* texture,
                              SDL_Rect* srcrect,
                              const SDL_Rect& dstrect,
                              double angle,
                              SDL_Point* center,
                              SDL_RendererFlip flip) {
    SDL_RenderCopyEx(m_Renderer, texture, srcrect, &dstrect, angle, center, flip);
}

void Drawing::Clear() {
    SDL_RenderClear(m_Renderer);
}

void Drawing::FillAll() {
    SDL_RenderFillRect(m_Renderer, nullptr);
}

void Drawing::UpdateWindow() {
    SDL_RenderPresent(m_Renderer);
}