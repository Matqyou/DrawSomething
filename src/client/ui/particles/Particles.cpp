//
// Created by Matq on 31/01/2025.
//

#include "Particles.h"

ParticlesClass Particles;

ParticlesClass::ParticlesClass() {
    this->active_begin = 0;
    this->active_end = 0;
    this->initialized = false;
}

ParticlesClass::~ParticlesClass() {

}

void ParticlesClass::Initialize(AssetsClass* assets) {
    this->initialized = true; // unused at the moment
}

void ParticlesClass::PlayParticle(const FlyingParticle& flying_particle) {
    this->particles[active_end] = flying_particle;
    this->active_end = (active_end + 1) % NUM_PARTICLES;
}

void ParticlesClass::Tick() {
    size_t active_current = active_begin;
    bool inactive = true;
    while (active_current != active_end) {
        FlyingParticle& current_particle = particles[active_current];
        if (current_particle.active) {
            current_particle.Tick();
            inactive = false;
        } else if (inactive && !current_particle.active) {
            active_begin = (active_begin + 1) % NUM_PARTICLES;
        }

        active_current = (active_current + 1) % NUM_PARTICLES;
    }
}

void ParticlesClass::Render() {
    size_t active_current = active_begin;
    while (active_current != active_end) {
        FlyingParticle& current_particle = particles[active_current];
        if (current_particle.active)
            current_particle.Render();

        active_current = (active_current + 1) % NUM_PARTICLES;
    }
}