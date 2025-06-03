//
// Created by Matq on 31/01/2025.
//

#include "FlyingParticle.h"
#include "core/app/Drawing.h"
#include "core/Application.h"

FlyingParticle::FlyingParticle() {
    this->active = false;
}

FlyingParticle::FlyingParticle(const Vec2f& pos,
                               const Vec2f& size,
                               float size_decay,
                               const Vec2f& velocity,
                               const Vec2f& gravity,
                               float velocity_decay,
                               float angle,
                               float angular_momentum,
                               float angular_decay,
                               Texture* texture_data) {
    this->pos = pos;
    this->size = size;
    this->size_decay = size_decay;
    this->velocity = velocity;
    this->gravity = gravity;
    this->velocity_decay = velocity_decay;
    this->angle = angle;
    this->angular_momentum = angular_momentum;
    this->angular_decay = angular_decay;
    this->texture = texture_data;
    this->active = true;
}

FlyingParticle::~FlyingParticle() {

}

void FlyingParticle::Tick() {
    size -= size_decay;

    if (size.x <= 0 || size.y <= 0 ||
        pos.x < 0 || pos.y < 0 ||
        pos.x > (float)Application::Get()->GetWidth() ||
        pos.y > (float)Application::Get()->GetHeight()) {
        active = false;
    } else {
        velocity += gravity;
        velocity *= velocity_decay;
        angular_momentum *= angular_decay;

        pos += velocity;
        angle += angular_momentum;
    }
}

void FlyingParticle::Render() {
#ifndef NDEBUG
    if (texture->SDLTexture() == nullptr)
        throw std::runtime_error("sdl_texture of Particle was nullptr");
#endif

    auto drawing = Assets::Get()->GetDrawing();

    SDL_FRect particle_rect = {
        pos.x - size.x / 2.0f,
        pos.y - size.y / 2.0f,
        size.x,
        size.y,
    };
    drawing->RenderTextureEx(texture->SDLTexture(), nullptr, particle_rect, angle, nullptr, SDL_FLIP_NONE);
}
