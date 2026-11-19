#include "draw.h"

void drawPoints(PointGraph* g, float lineWidth, Color clr) {

    for (int i = 0; i < g->pointCount; i++) {
        Vector2 p = g->points[i];
        AdjList* adj = &g->nodes[i];

        for (int k = 0; k < adj->count; k++) {
            Vector2 p2 = g->points[adj->neighbors[k]];
            DrawLineEx(p, p2, lineWidth, clr);
        }
    }
    
    for (int i = 0; i < g->pointCount; i++) {
        Vector2 p = g->points[i];
        DrawCircle(p.x, p.y, lineWidth / 2, RED); // change back to clr!
    }
}