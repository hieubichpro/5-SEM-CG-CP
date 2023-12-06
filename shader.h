#ifndef SHADER_H
#define SHADER_H
#include "model.h"
#include "matrix.h"
#include "vector.h"
#include "camera.h"
#include "light.h"

class PixelShaderInterface{
public:
    virtual Vec3f shade(const Vertex& a, const Vertex& b, const Vertex&c, const Vec3f& bary) = 0;
    virtual ~PixelShaderInterface(){}
};

class ColorShader: public PixelShaderInterface{
public:
    Vec3f shade(const Vertex &a, const Vertex &b, const Vertex &c, const Vec3f& bary) override;

    ~ColorShader() override{}
};

class VertexShaderInterface{
public:
    virtual Vertex shade(const Vertex &a,
                         const Mat4x4f& objToWorld,
                         const Mat4x4f& rotation,
                         const Mat4x4f& projection,
                         const Mat4x4f& camView,
                         Light& light) = 0;
    virtual ~VertexShaderInterface(){}

};

class VertexShader: public VertexShaderInterface{
public:
    VertexShader(){}
    Vertex shade(const Vertex &a,
                 const Mat4x4f& objToWorld,
                 const Mat4x4f& rotation,
                 const Mat4x4f& projection,
                 const Mat4x4f& camView,
                 Light& light) override;
    ~VertexShader() override{}

};



#endif // SHADER_H
