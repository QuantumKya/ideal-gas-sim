#include "../include/raylib.h"

#pragma once

Vector2 getReflection(Vector2 dir, Vector2 norm);

Vector2 getLineNormal(Vector2 endpoint1, Vector2 endpoint2);
Vector2 getInwardNormal(Vector2 endpoint1, Vector2 endpoint2, Vector2 center);
Vector2 getOutwardNormal(Vector2 endpoint1, Vector2 endpoint2, Vector2 center);

float randfNormal();
float randfRange(float min, float max);
float randiRange(int min, int max);