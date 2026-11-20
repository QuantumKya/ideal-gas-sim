#include "point_graph.h"
#include "../include/raylib.h"

#pragma once

void drawPoints(PointGraph* g, float lineWidth, Color clr);
void drawVector(Vector2 origin, Vector2 direction, float lineWidth, Color clr);

void drawNormals(PointGraph* g, float length, float lineWidth, Color clr);

void printVec(Vector2 v);