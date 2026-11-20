#include "particle.h"
#include "../include/raymath.h"
#include "vecmath.h"

const float particleRadius = 5;

bool checkCollisionWall(Particle* p, Vector2 endpoint1, Vector2 endpoint2);



void updateParticle(Particle* p, PointGraph* g) {
    const float dt = GetFrameTime();
    const Vector2 newPos = Vector2Add(p->position, Vector2Scale(p->momentum, 1 / p->mass * dt));
    p->position = newPos;


    // check for collisions
    Vector2 e1; Vector2 e2;
    int edgeId = -1;

    for (int i = 0; i < g->pointCount; i++) {
        e1 = g->points[i];
        AdjList* adj = &g->nodes[i];

        for (int k = 0; k < adj->count; k++) {
            e2 = g->points[ adj->neighbors[k] ];

            if (checkCollisionWall(p, e1, e2)) {
                edgeId = i;
                break;
            }
        }
        if (edgeId >= 0) break;
    }

    if (edgeId >= 0) { // do the bounce
        const Vector2 norm = getInwardNormal(e1, e2, getCentroid(g));
        p->momentum = getReflection(p->momentum, norm);
    }
}



bool checkCollisionWall(Particle* p, Vector2 endpoint1, Vector2 endpoint2) {
    const Vector2 normal = getInwardNormal(endpoint1, endpoint2, p->position);
    const Vector2 diff = Vector2Subtract(p->position, endpoint1);

    return Vector2DotProduct(normal, diff) <= particleRadius;
}