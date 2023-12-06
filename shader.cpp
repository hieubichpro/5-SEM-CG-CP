#include "shader.h"
#include "barycenter.h"
const float eps = 1e-6f;

Vertex VertexShader::shade(const Vertex &a, const Mat4x4f& objToWorld, const Mat4x4f &projection, const Mat4x4f& camView, Light& light)
{
    Vec4f new_pos(a.position);
    new_pos = new_pos * objToWorld * camView * projection;
//    new_pos.print_v();
    Vec4f new_normal(a.normal);
    new_normal = new_normal * objToWorld;

    Vertex output = a;
    output.position = Vec3f(new_pos.x, new_pos.y, new_pos.z);
    output.normal = Vec3f(new_normal.x, new_normal.y, new_normal.z).normalize();
    if (fabs(new_pos.w) < eps)
        new_pos.w = 1;
    output.invW = 1 / new_pos.w;
    output.texture.x *= output.invW;
    output.texture.y *= output.invW;
    auto diffuse = light.diff * std::max(0.f, Vec3f::dot(output.normal.normalize(), (light.pos - output.position).normalize())) * light.intensity;
    auto c = (diffuse + light.ambient).saturate();
    output.color = output.color.hadamard(c).saturate();
    return output;
}

Vec3f ColorShader::shade(const Vertex &a, const Vertex &b, const Vertex &c, const Vec3f& bary)
{
    auto pixel_color = baryCentricInterpolation(a.color, b.color, c.color, bary);
    return pixel_color;
}
