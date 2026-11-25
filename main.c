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
        (Vector2){ 0, -400 } // 4 meters
    );
    const float totalWallLength = getTotalLength(testTriangle);

    ParticleSystem* particleSystem = initParticleSystem(500);
    float currentPressure = 0.0f;

    for (int i = 0; i < 500; i++) {
        Particle* p = &particleSystem->particles[i];
        p->position = (Vector2){ SCREEN_WIDTH/2 + randfRange(-200, 200), SCREEN_HEIGHT/2 + randfRange(-200, 200) };
        p->mass = 39.95f; // argon!
        
        const float angle = randfRange(-PI, PI);
        p->velocity = Vector2Scale((Vector2){ cosf(angle), sinf(angle) }, 200);
    }
    setUpParticles(particleSystem, 39.95f, 1.0f); // 1.0 K

    //for (int i = 0; i < testTriangle->pointCount; i++) printVec(testTriangle->points[i]);

    int frame = 0;
    while (!WindowShouldClose()) {
        const float dt = GetFrameTime();

        updateSystem(particleSystem, testTriangle);

        if (IsKeyDown(KEY_UP)) raiseTemperature(particleSystem, 20.0f * dt);
        if (IsKeyDown(KEY_DOWN)) raiseTemperature(particleSystem, -20.0f * dt);

        if (frame % PRESSURE_WAIT_TIME == 0) {
            currentPressure = getPressure(totalWallLength, dt);
            pressureThisFrame = 0.0f;
        }
        
        BeginDrawing();
        ClearBackground(BLACK);

            drawPoints(testTriangle, 5, WHITE);
            //drawNormals(testTriangle, 20, 5, BLUE);
            
            drawSystem(particleSystem, GREEN);

            DrawText(TextFormat("Current Pressure: %.03e Pa", currentPressure), 30, 30, 30, WHITE);
            DrawText(TextFormat("Total KE: %.03e J", getTotalKE(particleSystem)), 30, 70, 30, WHITE);
            DrawText(TextFormat("Temperature: %.03e K", getTemperature(particleSystem)), 30, 110, 30, WHITE);

            //drawPartitionSet(particleSystem->partition);

        EndDrawing();
        frame++;
    }
    CloseWindow();

    freePointGraph(testTriangle);
    freeParticleSystem(particleSystem);

    return 0;
}