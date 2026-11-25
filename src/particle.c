#include "particle.h"
#include "../include/raymath.h"
#include "vecmath.h"
#include "const.h"

const float particleRadius = 5;

bool checkCollisionWall(Particle* p, Vector2 getCentroid, Vector2 endpoint1, Vector2 endpoint2, float* moment);



float updateParticle(Particle* p, PointGraph* g) {
    const float dt = GetFrameTime();
    const Vector2 displacement = Vector2Scale(p->velocity, dt);
    const Vector2 newPos = Vector2Add(p->position, displacement);

    const Vector2 gCentroid = getCentroid(g);


    // check for collisions
    Vector2 e1; Vector2 e2;
    bool collided = false;
    float momentumTransfer = 0.0f;

    for (int i = 0; i < g->pointCount; i++) {
        e1 = g->points[i];
        AdjList* adj = &g->nodes[i];

        for (int k = 0; k < adj->count; k++) {
            e2 = g->points[ adj->neighbors[k] ];

            collided = checkCollisionWall(p, gCentroid, e1, e2, &momentumTransfer);
            if (collided) return momentumTransfer;
        }
    }

    if (!collided) {
        p->position = newPos;
    }

    return 0.0f;
}



bool checkCollisionWall(Particle* p, Vector2 gCentroid, Vector2 endpoint1, Vector2 endpoint2, float* moment) {
    const Vector2 midpoint = Vector2Lerp(endpoint1, endpoint2, 0.5f);
    const Vector2 normal = getInwardNormal(endpoint1, endpoint2, gCentroid);
    
    if (Vector2DotProduct(p->velocity, normal) >= 0) return false; // not moving into the wall
    
    
    const float lengthSqr = Vector2DistanceSqr(endpoint1, endpoint2);
    const Vector2 diff = Vector2Subtract(p->position, midpoint);

    const Vector2 parallelProj = Vector2Scale(
        normal, Vector2DotProduct(diff, normal) / Vector2DotProduct(normal, normal)
    );
    const float perp = Vector2DistanceSqr(diff, parallelProj);

    if (perp > lengthSqr / 4) return false; // projection is not within segment


    // calculations for clamping and bouncing ------------------------------------------------
    
    const float s = Vector2DotProduct(diff, normal);
    const float penetration = particleRadius - s;
    if (penetration <= 0) return false; // particle hasn't actually touched the wall

    // add to total impulse
    *moment = 
        2.0f * (p->mass * DA_TO_KG)
        * fabs(Vector2DotProduct(p->velocity, normal)) * METERS_PER_PIXEL;

    // clamp and bounce
    p->position = Vector2Add(p->position, Vector2Scale(normal, penetration));
    p->velocity = getReflection(p->velocity, normal);


    return true;
}



float getVelInMS(Particle* p) {
    return Vector2Length(p->velocity) * METERS_PER_PIXEL;
}
float getParticleKE(Particle* p) {
    const float massKg = p->mass * DA_TO_KG;
    const float speedMS = getVelInMS(p);

    return 0.5f * massKg * speedMS * speedMS;
}