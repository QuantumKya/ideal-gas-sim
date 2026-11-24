#include "particle.h"

#define MAX_DEPTH 4
#define MAX_PER_QUAD 32

#define PRESSURE_WAIT_TIME 120

/*
typedef struct {
    int code;
    int depth;
} QuadCode;

typedef struct {
    float minX, maxX,
    minY, maxY;
} Quadrant;

typedef struct {
    int** particles; // one particle index array for each quad
    int* particleCounts; // counts for each quad particle array (sheesh, I know it's bad)
    QuadCode* quads;
    size_t quadCount;
} PartitionSets;
*/

typedef struct {
    Particle* particles;
    size_t count;
    //PartitionSets* partition;
} ParticleSystem;

extern float pressureThisFrame;



ParticleSystem* initParticleSystem(int numParticles);
void freeParticleSystem(ParticleSystem*);

void updateSystem(ParticleSystem* ps, PointGraph* g);
void drawSystem(ParticleSystem* ps, Color clr);

void checkParticleCollisions(ParticleSystem*);


float getPressure(float wallLength, float dt);


/*
PartitionSets* initPartitionSet();
void freePartitionSet(PartitionSets*);

void drawPartitionSet(PartitionSets*);
*/