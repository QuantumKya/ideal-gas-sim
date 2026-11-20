#include <stdio.h>
#include <stdlib.h>
#include "include/raylib.h"

#include "src/const.h"
#include "src/point_graph.h"
#include "src/draw.h"
#include "src/particle.h"
#include "src/vecmath.h"


int main() {

    InitWindow(screenWidth, screenHeight, "Espy Orbital");
    SetTargetFPS(60);

    // Font defFont = LoadFont("cmuntb.ttf");

    PointGraph* testTriangle = makeRegularPolygon(5, (Vector2){ 500, 500 }, (Vector2){ 0, -400 });

    Particle* testParticles = calloc(50, sizeof(Particle));
    for (int i = 0; i < 50; i++) {
        testParticles[i].position = (Vector2){ 500, 800 };
        testParticles[i].mass = 10;

        const float angle = randfRange(-PI, 0);
        testParticles[i].momentum = Vector2Scale((Vector2){ cosf(angle), sinf(angle) }, 3000);
    }

    //for (int i = 0; i < testTriangle->pointCount; i++) printVec(testTriangle->points[i]);

    while (!WindowShouldClose()) {

        for (int i = 0; i < 50; i++) updateParticle(&testParticles[i], testTriangle);
        
        BeginDrawing();
        ClearBackground(BLACK);

        drawPoints(testTriangle, 5, WHITE);
        drawNormals(testTriangle, 20, 5, BLUE);

        for (int i = 0; i < 50; i++) DrawCircleV(testParticles[i].position, particleRadius, GREEN);

        EndDrawing();
    }
    CloseWindow();

    freePointGraph(testTriangle);
    free(testParticles);

    return 0;
}