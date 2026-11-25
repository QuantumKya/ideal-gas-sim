#include "../include/raylib.h"
#include "point_graph.h"

#pragma once

extern const float particleRadius;

typedef struct {
    float mass; // in daltons, I guess (for convenience)
    Vector2 position; // one pixel is 10 picometers
    Vector2 velocity; // pixels per second
} Particle;


// returns momentum transfer at wall
float updateParticle(Particle* p, PointGraph* g);

float getVelInMS(Particle* p);
float getParticleKE(Particle* p);