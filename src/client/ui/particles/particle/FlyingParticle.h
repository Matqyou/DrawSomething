//
// Created by Matq on 31/01/2025.
//

#pragma once

#include <SDL3/SDL_render.h>
#include "../../../../shared/core/Vec2.h"
#include "../../../core/app/Assets.h"

struct FlyingParticle {
public:
    Vec2f pos;
    Vec2f size;
    float size_decay;
    Vec2f velocity;
    Vec2f gravity;
    float velocity_decay;
    float angle;
    float angular_momentum;
    float angular_decay;
    TextureData* texture_data;

    bool active;

public:
    FlyingParticle();
    FlyingParticle(const Vec2f& pos,
                   const Vec2f& size,
                   float size_decay,
                   const Vec2f& velocity,
                   const Vec2f& gravity,
                   float velocity_decay,
                   float angle,
                   float angular_momentum,
                   float angular_decay,
                   TextureData* texture_data);
    ~FlyingParticle();

    // Ticking
    void Tick();
    void Render();

};
