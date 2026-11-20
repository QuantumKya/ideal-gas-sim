#include "draw.h"
#include <stdio.h>

#include "vecmath.h"

void drawPoints(PointGraph* g, float lineWidth, Color clr) {

    for (int i = 0; i < g->pointCount; i++) {
        Vector2 p1 = g->points[i];
        AdjList* adj = &g->nodes[i];

        for (int k = 0; k < adj->count; k++) {
            Vector2 p2 = g->points[adj->neighbors[k]];
            DrawLineEx(p1, p2, lineWidth, clr);
        }
    }
    
    for (int i = 0; i < g->pointCount; i++) {
        Vector2 p = g->points[i];
        DrawCircle(p.x, p.y, lineWidth / 2, RED); // change back to clr!
    }
}

void drawVector(Vector2 origin, Vector2 direction, float lineWidth, Color clr) {
    const Vector2 tip = Vector2Add(origin, direction);
    DrawLineEx(origin, tip, lineWidth, clr);

    // draw pointy end lines
    // const Vector2 arrowDir1 = Vector2Scale(Vector2Normalize(Vector2Rotate(direction,  135*DEG2RAD)), 10);
    // const Vector2 arrowDir2 = Vector2Scale(Vector2Normalize(Vector2Rotate(direction, -135*DEG2RAD)), 10);

    // DrawTriangle(tip, Vector2Add(tip, arrowDir2), Vector2Add(tip, arrowDir1), clr);
}



void printVec(Vector2 v) {
    printf("(%f, %f)\n", v.x, v.y);
}



void drawNormals(PointGraph* g, float length, float lineWidth, Color clr) {
    for (int i = 0; i < g->pointCount; i++) {
        Vector2 p1 = g->points[i];
        AdjList* adj = &g->nodes[i];

        for (int k = 0; k < adj->count; k++) {
            Vector2 p2 = g->points[ adj->neighbors[k] ];

            drawVector(
                Vector2Lerp(p1, p2, 0.5),
                Vector2Scale(getLineNormal(p1, p2), length),
                lineWidth, clr
            );
        }
    }
}