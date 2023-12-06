#include "barycenter.h"

float interPolateCord(float val1, float val2, float val3, const Vec3f& bary){
    return bary.x * val1 + bary.y * val2 + bary.z * val3;
}

Vec3f baryCentricInterpolation(const Vec3f& a, const Vec3f& b, const Vec3f& c, const Vec3f& bary)
{
    float x = interPolateCord(a.x, b.x, c.x, bary);
    float y = interPolateCord(a.y, b.y, c.y, bary);
    float z = interPolateCord(a.z, b.z, c.z, bary);
    return {x, y, z};
}

float getAreaTriangle(Vec3f a, Vec3f b, Vec3f p){
    return (a.y - b.y) * p.x + (b.x - a.x) * p.y + a.x * b.y - b.x * a.y;
}

Vec3f getBarycentric(Vec3f a, Vec3f b, Vec3f c, Vec3f p){


    float S_PBC = getAreaTriangle(c, b, p);
    float S_PAC = getAreaTriangle(a, c, p);

    float S_ABC = getAreaTriangle(c, b, a);

    auto lamda1 = S_PBC / S_ABC;
    auto lamda2 = S_PAC / S_ABC;

    return {lamda1, lamda2, 1 - lamda1 - lamda2};
}
