#ifndef BARYCENTER_H
#define BARYCENTER_H

#include "vector.h"

float interPolateCord(float val1, float val2, float val3, const Vec3f& bary);

Vec3f baryCentricInterpolation(const Vec3f& a, const Vec3f& b, const Vec3f& c, const Vec3f& bary);

float getAreaTriangle(Vec3f a, Vec3f b, Vec3f p);

Vec3f getBarycentric(Vec3f a, Vec3f b, Vec3f c, Vec3f p);

#endif // BARYCENTER_H
