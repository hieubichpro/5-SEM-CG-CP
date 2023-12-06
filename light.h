#ifndef LIGHT_H
#define LIGHT_H
#include "vector.h"

class Light
{
public:
    Vec3f pos;
    Vec3f diff;
    Vec3f ambient;
    float intensity = 1.f;
public:
    void shiftX(const float& offs);
    void shiftY(const float& offs);
    void shiftZ(const float& offs);
    Light(const Vec3f& pos_ = {0.f, 0.f, -100.f},
          const Vec3f& diff_ = {1.0f, 1.0f, 1.0f},
          const Vec3f& ambient_ = {0.3f, 0.3f, 0.3f});
};

#endif // LIGHT_H
