#include "light.h"

Light::Light(const Vec3f& pos_, const Vec3f& diff_, const Vec3f& ambient_):
    pos{pos_}, diff{diff_}, ambient{ambient_}{}

void Light::shiftX(const float& offs)
{
    pos.x += offs;
}
void Light::shiftY(const float& offs)
{
    pos.y += offs;
}
void Light::shiftZ(const float& offs)
{
    pos.z += offs;
}
