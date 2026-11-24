#include "include/raylib.h"

#include "src/const.h"
#include "src/point_graph.h"
#include "src/draw.h"
#include "src/particle.h"
#include "src/vecmath.h"
#include "src/particle_system.h"


int main() {

    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Espy Orbital");
    SetTargetFPS(60);

    // Font defFont = LoadFont("cmuntb.ttf");

    PointGraph* testTriangle = makeRegularPolygon(5,
        (Vector2){ SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 },
        (Vector2){ 0, -400 }
    );
    const float totalWallLength = getTotalLength(testTriangle);

    ParticleSystem* particleSystem = initParticleSystem(200);
    float currentPressure = 0.0f;

    for (int i = 0; i < 200; i++) {
        Particle* p = &particleSystem->particles[i];
        p->position = (Vector2){ SCREEN_WIDTH/2 + randfRange(-200, 200), SCREEN_HEIGHT/2 + randfRange(-200, 200) };
        p->mass = 10.0f;

        const float angle = randfRange(-PI, PI);
        p->velocity = Vector2Scale((Vector2){ cosf(angle), sinf(angle) }, 300);
    }

    //for (int i = 0; i < testTriangle->pointCount; i++) printVec(testTriangle->points[i]);

    int frame = 0;
    while (!WindowShouldClose()) {

        updateSystem(particleSystem, testTriangle);

        if (frame % PRESSURE_WAIT_TIME == 0) {
            currentPressure = getPressure(totalWallLength, GetFrameTime());
            pressureThisFrame = 0.0f;
        }
        
        BeginDrawing();
        ClearBackground(BLACK);

            drawPoints(testTriangle, 5, WHITE);
            drawNormals(testTriangle, 20, 5, BLUE);
            
            drawSystem(particleSystem, GREEN);

            DrawText(TextFormat("Current Pressure: %08.08f", currentPressure), 30, 30, 30, WHITE);

            //drawPartitionSet(particleSystem->partition);

        EndDrawing();
        frame++;
    }
    CloseWindow();

    freePointGraph(testTriangle);
    freeParticleSystem(particleSystem);

    return 0;
}