#include "../include/raylib.h"
#include "point_graph.h"

#pragma once

extern const float particleRadius;

typedef struct {
    float mass;
    Vector2 position;
    Vector2 velocity;
} Particle;


// returns momentum transfer at wall
float updateParticle(Particle* p, PointGraph* g);