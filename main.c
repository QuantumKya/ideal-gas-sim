#include <stdio.h>
#include "include/raylib.h"

#include "src/const.h"
#include "src/point_graph.h"
#include "src/draw.h"
#include "src/utility.h"


int main() {

    InitWindow(screenWidth, screenHeight, "Check This Out, Noah!");
    SetTargetFPS(60);

    // Font defFont = LoadFont("cmuntb.ttf");

    PointGraph* testTriangle = makeTriangle((Vector2){ 0, 0 }, (Vector2){ 50, 0 }, (Vector2){ 0, 50 });
    translatePoints(testTriangle, (Vector2){ 400, 400 });

    //for (int i = 0; i < testTriangle->pointCount; i++) printVec(testTriangle->points[i]);

    while (!WindowShouldClose()) {
        
        BeginDrawing();
        ClearBackground(BLACK);

        drawPoints(testTriangle, 5, WHITE);

        EndDrawing();
    }
    CloseWindow();

    freePointGraph(testTriangle);

    return 0;
}