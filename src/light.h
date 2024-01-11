#ifndef LIGHT_H
#define LIGHT_H
#include "vector.h"

class Light
{
public:
    Vec3f pos;
    Vec3f color;
    float intensity = 1.f;
public:
    void shiftX(const float& offs);
    void shiftY(const float& offs);
    void shiftZ(const float& offs);
    Light(const Vec3f& pos_ = {0.f, 0.f, -50.f}, const Vec3f &color = {1.0f, 1.0f, 1.0f});
};

#endif // LIGHT_H
