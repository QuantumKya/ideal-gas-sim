#include "../include/raylib.h"
#include "point_graph.h"

#pragma once

extern const float particleRadius;

typedef struct {
    float mass;
    Vector2 position;
    Vector2 momentum;
} Particle;


void updateParticle(Particle* p, PointGraph* g);