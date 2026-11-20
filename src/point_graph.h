#include "../include/raymath.h"
#include <stdint.h>

#pragma once

typedef struct {
    uint8_t* neighbors;
    size_t count;
    size_t memCap;
} AdjList;

typedef struct {
    Vector2* points; // the points and nodes arrays are parallel;
    AdjList* nodes; // each point has a respective node at the same index.
    size_t pointCount;
} PointGraph;



void initPointGraph(PointGraph* g, int numPoints);
void freePointGraph(PointGraph* g);

void addPoint(AdjList* adj, int i);
void addEdge(PointGraph* g, int a, int b);

PointGraph* makePolygon(Vector2* pnts, size_t numPts);
PointGraph* makeTriangle(Vector2 corner, Vector2 edge1, Vector2 edge2);
PointGraph* makeParallelogram(Vector2 corner, Vector2 edge1, Vector2 edge2);
PointGraph* makeRegularPolygon(int sides, Vector2 origin, Vector2 v1);

void translatePoints(PointGraph* g, Vector2 by);
Vector2 getCentroid(PointGraph* g);
Vector2 correctNormal(PointGraph* g, int edgeId, Vector2 norm);