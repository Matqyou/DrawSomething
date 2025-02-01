//
// Created by Matq on 31/01/2025.
//

#pragma once

#include "particle/FlyingParticle.h"
#include "../../core/app/Assets.h"

#define NUM_PARTICLES 1000

class ParticlesClass {
private:
    FlyingParticle particles[NUM_PARTICLES];
    size_t active_begin, active_end;

    bool initialized;

public:
    ParticlesClass();
    ~ParticlesClass();
    void Initialize(AssetsClass* assets);

    // Manipulating
    void PlayParticle(const FlyingParticle& flying_particle);

    // Ticking
    void Tick();
    void Render();

};

extern ParticlesClass Particles;
