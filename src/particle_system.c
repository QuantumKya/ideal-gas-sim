#include "particle_system.h"
#include <stdlib.h>

#include "../include/raylib.h"

float pressureThisFrame = 0.0f;

/*
const Quadrant screenBox = (Quadrant){ 0, SCREEN_WIDTH, 0, SCREEN_HEIGHT };



bool isInQuadrant(Vector2 pnt, Quadrant quad) {
    return (pnt.x >= quad.minX)
        && (pnt.y >= quad.minY)
        && (pnt.x < quad.maxX)
        && (pnt.y < quad.maxY);
}

void codeToQuad(QuadCode quadCode, Quadrant* dest, Quadrant world) {
    dest->minX = world.minX; dest->maxX = world.maxX;
    dest->minY = world.minY; dest->maxY = world.maxY;

    for (int d = quadCode.depth; d > 0; d--) {

        int xChoice = (quadCode.code >> ((d-1)*2)) & 1;
        int yChoice = (quadCode.code >> ((d-1)*2 + 1)) & 1;

        const Vector2 distance = (Vector2){ dest->maxX - dest->minX, dest->maxY - dest->minY };
        const Vector2 demiDistance = Vector2Scale(distance, 0.5);



        dest->minX += xChoice * demiDistance.x;
        dest->maxX = dest->minX + demiDistance.x;

        dest->minY += yChoice * demiDistance.y;
        dest->maxY = dest->minY + demiDistance.y;
    }
}

void drawQuad(Quadrant q, float lineThick, Color clr) {
    DrawLineEx((Vector2){ q.minX, q.minY }, (Vector2){ q.maxX, q.minY }, lineThick, clr);
    DrawLineEx((Vector2){ q.minX, q.minY }, (Vector2){ q.minX, q.maxY }, lineThick, clr);
    DrawLineEx((Vector2){ q.maxX, q.maxY }, (Vector2){ q.minX, q.maxY }, lineThick, clr);
    DrawLineEx((Vector2){ q.maxX, q.maxY }, (Vector2){ q.maxX, q.minY }, lineThick, clr);
}
void drawQuadFromCode(QuadCode code, float lineThick, Color clr) {
    Quadrant q;
    codeToQuad(code, &q, screenBox);
    drawQuad(q, lineThick, clr);
}


void makePartitionSet(PartitionSets* box, ParticleSystem* ps) {
    QuadCode currentQuad = { 0, 0 };
    int particlesCounted = 0;
    box->quadCount = 0;

    int iter = 0;
    do {
        Quadrant quad;
        codeToQuad(currentQuad, &quad, screenBox);
        
        int list[MAX_PER_QUAD] = {};
        int listSize = 0;
        for (int i = 0; i < ps->count; i++) {
            Particle* p = &ps->particles[i];

            // overfull leaves can be a problem,
            // but they are so unlikely that I won't worry about it
            if (isInQuadrant(p->position, quad)) {
                if (listSize >= MAX_PER_QUAD) {
                    listSize++;
                    break;
                }
                particlesCounted++;
                list[listSize++] = i;
            }
        }

        if (listSize > MAX_PER_QUAD && currentQuad.depth < MAX_DEPTH) { // gotta split the quad again!
            currentQuad.code <<= 2;
            currentQuad.depth++;
            continue;
        }

        // if the code gets here, this is a leaf node; add the stuff in
        box->quads[iter] = currentQuad;
        box->quadCount++;
        for (int i = 0; i < listSize; i++) box->particles[iter][i] = list[i];
        box->particleCounts[iter] = listSize;


        int depthCut = currentQuad.code & 0b11;
        if (depthCut == 0b11 && currentQuad.depth > 0) { // done with this depth, pop back up
            currentQuad.code >>= 2;
            currentQuad.depth--;
        }
        
        currentQuad.code++;
        iter++;
    } while (currentQuad.depth > 0);
}
*/


void doCollision(Particle* p1, Particle* p2) {
    const Vector2 gap = Vector2Subtract(p2->position, p1->position);
    const float distance = Vector2Length(gap);
    if (distance > 2 * particleRadius) return;
    
    const float m1 = p1->mass, m2 = p2->mass;
    const float M = m1 + m2;

    const Vector2 v1 = p1->velocity;
    const Vector2 v2 = p2->velocity;


    // I got this all from this page: https://scipython.com/blog/two-dimensional-collisions/

    const Vector2 mgap = Vector2Negate(gap); // p1 - p2
    const Vector2 vrel1 = Vector2Subtract(v1, v2); // v1 - v2
    const Vector2 vrel2 = Vector2Subtract(v2, v1); // v2 - v1

    const float factor1 = 2*m2 / M * Vector2DotProduct(vrel1, mgap) / powf(distance, 2);
    const float factor2 = 2*m1 / M * Vector2DotProduct(vrel2,  gap) / powf(distance, 2);
    p1->velocity = Vector2Subtract(v1, Vector2Scale(mgap, factor1));
    p2->velocity = Vector2Subtract(v2, Vector2Scale( gap, factor2));
}

void checkParticleCollisions(ParticleSystem* ps) {
    /*
    PartitionSets* part = ps->partition;
    makePartitionSet(part, ps);

    for (int i = 0; i < part->quadCount; i++) {
        Quadrant quad = {};
        codeToQuad(part->quads[i], &quad, screenBox);

        for (int k = 0; k < part->particleCounts[i]; k++) {
            const int pId1 = part->particles[i][k];
            Particle* p1 = &ps->particles[pId1];

            for (int kk = k + 1; kk < part->particleCounts[i]; kk++) {
                const int pId2 = part->particles[i][kk];
                Particle* p2 = &ps->particles[pId2];

                // check for collision... somehow
                doCollision(p1, p2);
            }
        }
    }
    */

    for (int i = 0; i < ps->count; i++) {
        for (int ii = i+1; ii < ps->count; ii++) {
            doCollision(&ps->particles[i], &ps->particles[ii]);
        }
    }
}



ParticleSystem* initParticleSystem(int numParticles) {
    ParticleSystem* ps = calloc(1, sizeof(ParticleSystem));
    ps->particles = calloc(numParticles, sizeof(Particle));
    ps->count = numParticles;
    //ps->partition = initPartitionSet();

    return ps;
}
void freeParticleSystem(ParticleSystem* ps) {
    //freePartitionSet(ps->partition);
    free(ps->particles);
    free(ps);
}

void updateSystem(ParticleSystem* ps, PointGraph* g) {
    for (int i = 0; i < ps->count; i++) {
        Particle* p = &ps->particles[i];

        float wallHit = updateParticle(p, g);
        pressureThisFrame += wallHit;
    }
    checkParticleCollisions(ps);
}
void drawSystem(ParticleSystem* ps, Color clr) {
    for (int i = 0; i < ps->count; i++) DrawCircleV(ps->particles[i].position, particleRadius, clr);
}


float getPressure(float wallLength, float dt) {
    const float delta_t = dt * PRESSURE_WAIT_TIME; // average over many timesteps to get good results
    return pressureThisFrame / (wallLength * delta_t);
}


/*
PartitionSets* initPartitionSet() {
    PartitionSets* partition = calloc(1, sizeof(PartitionSets));
    

    const int maxQuads = (pow(4, MAX_DEPTH + 1) - 1) / 3;

    partition->particles = calloc(maxQuads, sizeof(int*));
    for (int i = 0; i < maxQuads; i++) partition->particles[i] = calloc(MAX_PER_QUAD, sizeof(int));
    partition->particleCounts = calloc(maxQuads, sizeof(int));
    
    partition->quads = calloc(maxQuads, sizeof(QuadCode));
    partition->quadCount = 0;

    return partition;
}
void freePartitionSet(PartitionSets* partition) {
    for (int i = 0; i < partition->quadCount; i++)
        free(partition->particles[i]);
    
    free(partition->particles);
    free(partition->particleCounts);
    free(partition->quads);
    free(partition);
}

void drawPartitionSet(PartitionSets* partition) {
    for (int i = 0; i < partition->quadCount; i++) { // fix this memory leak
        const QuadCode qCode = partition->quads[i];
        drawQuadFromCode(qCode, 5, RED);
    }
}
*/