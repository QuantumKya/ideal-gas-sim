#include "vecmath.h"
#include "../include/raymath.h"

#include <stdlib.h>

Vector2 getReflection(Vector2 dir, Vector2 norm) { // formula is r = d - 2 (d * n) n
    const float dot = Vector2DotProduct(dir, norm);
    const Vector2 scaled = Vector2Scale(norm, 2 * dot);

    return Vector2Subtract(dir, scaled);
}

Vector2 getLineNormal(Vector2 endpoint1, Vector2 endpoint2) {
    const Vector2 delta = Vector2Subtract(endpoint2, endpoint1);
    const Vector2 perp = (Vector2){ -delta.y, delta.x };

    return Vector2Normalize(perp);
}

Vector2 getInwardNormal(Vector2 endpoint1, Vector2 endpoint2, Vector2 center) {
    const Vector2 naturalNorm = getLineNormal(endpoint1, endpoint2);
    const Vector2 diff = Vector2Subtract(endpoint1, center);

    if (Vector2DotProduct(naturalNorm, diff) > 0) return Vector2Negate(naturalNorm);
    else return naturalNorm;
}
Vector2 getOutwardNormal(Vector2 endpoint1, Vector2 endpoint2, Vector2 center) {
    const Vector2 naturalNorm = getLineNormal(endpoint1, endpoint2);
    const Vector2 diff = Vector2Subtract(endpoint1, center);

    if (Vector2DotProduct(naturalNorm, diff) < 0) return Vector2Negate(naturalNorm);
    else return naturalNorm;
}

float randfNormal() {
    return (float)rand() / (float)RAND_MAX;
}
float randfRange(float min, float max) {
    return min + randfNormal() * (max - min);
}
float randiRange(int min, int max) {
    return (rand() % (max - min + 1)) + min;
}