#include "point_graph.h"
#include <stdlib.h>
#include <string.h>

#include "utility.h"

void initPointGraph(PointGraph* g, int numPoints) {
    g->points = calloc(numPoints, sizeof(Vector2));
    g->nodes = calloc(numPoints, sizeof(AdjList));
    for (int i = 0; i < numPoints; i++) {
        AdjList* const adj = &g->nodes[i];
        adj->neighbors = calloc(8, sizeof(uint8_t));
        adj->count = 0;
        adj->memCap = 8;
    }
}

void freePointGraph(PointGraph* g) {
    for (int i = 0; i < g->pointCount; i++) {
        free(g->nodes[i].neighbors);
    }
    free(g->nodes);
    free(g->points);
    free(g);
}



void addPoint(AdjList* adj, int i) {
    if (adj->count >= adj->memCap) {
        adj->memCap *= 2;
        adj->neighbors = realloc(adj->neighbors, adj->memCap * sizeof(uint8_t));
    }
    adj->neighbors[adj->count++] = i;
}

void addEdge(PointGraph* g, int a, int b) {
    AdjList* aEdges = &g->nodes[a];
    AdjList* bEdges = &g->nodes[b];

    addPoint(aEdges, b); addPoint(bEdges, a);
}



// clones array, leaves argument untouched
PointGraph* makePolygon(Vector2* pnts, size_t numPts) {
    if (numPts < 3 || numPts >= 256) return NULL;

    PointGraph* g = calloc(1, sizeof(*g));
    initPointGraph(g, numPts);
    

    // clone the array of points
    for (int i = 0; i < numPts; i++) g->points[i] = pnts[i];
    g->pointCount = numPts;

    // add all the edges
    for (int i = 0; i < g->pointCount - 1; i++) {
        addEdge(g, i, i + 1);
    }
    addEdge(g, g->pointCount - 1, 0); // catch the ends in the final edge

    return g;
}

PointGraph* makeTriangle(Vector2 corner, Vector2 edge1, Vector2 edge2) {
    Vector2 points[3];
    points[0] = corner;
    points[1] = Vector2Add(corner, edge1);
    points[2] = Vector2Add(corner, edge2);

    return makePolygon(points, 3);
}

PointGraph* makeParallelogram(Vector2 corner, Vector2 edge1, Vector2 edge2) {

}

PointGraph* makeRegularPolygon(int sides, Vector2 origin, Vector2 v1) {

}



void translatePoints(PointGraph* g, Vector2 by) {
    for (int i = 0; i < g->pointCount; i++) {
        g->points[i] = Vector2Add(g->points[i], by);
    }
}