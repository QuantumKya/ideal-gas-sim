#include "point_graph.h"
#include <stdlib.h>

#include "draw.h"
#include "vecmath.h"

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
    addPoint(aEdges, b);
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
    Vector2 points[3];
    points[0] = corner;
    points[1] = Vector2Add(corner, edge1);
    points[2] = Vector2Add(corner, edge2);
    points[3] = Vector2Add(points[1], edge2);

    return makePolygon(points, 4);
}

PointGraph* makeRegularPolygon(int sides, Vector2 origin, Vector2 v1) {
    Vector2 pts[sides];

    for (int i = 0; i < sides; i++) {
        const Vector2 sidePoint = Vector2Rotate(v1, i * 360 / sides * DEG2RAD);
        pts[i] = Vector2Add(origin, sidePoint);
    }

    return makePolygon(pts, sides);
}



void translatePoints(PointGraph* g, Vector2 by) {
    for (int i = 0; i < g->pointCount; i++) {
        g->points[i] = Vector2Add(g->points[i], by);
    }
}

Vector2 getCentroid(PointGraph* g) {
    Vector2 sum;
    for (int i = 0; i < g->pointCount; i++) sum = Vector2Add(sum, g->points[i]);
    return Vector2Scale(sum, 1/g->pointCount);
}

Vector2 correctNormal(PointGraph* g, int pId, Vector2 norm) {
    const Vector2 c = getCentroid(g);
    const Vector2 e = g->points[pId];

    const Vector2 rel = Vector2Subtract(e, c);
    if (Vector2DotProduct(rel, norm) > 0) return Vector2Negate(norm);
    else return norm;
}