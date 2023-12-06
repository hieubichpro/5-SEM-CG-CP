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
                         const Mat4x4f& projection,
                         const Mat4x4f& camView,
                         Light& light) = 0;
    virtual ~VertexShaderInterface(){}

};

class VertexShader: public VertexShaderInterface{
public:
    VertexShader(const Vec3f& dir_ = {0.f, 0.f, 1.f},
                 const Vec3f& diff_ = {1.0f, 1.0f, 1.0f},
                 const Vec3f& ambient_ = {0.3f, 0.3f, 0.3f}):
        dir{dir_}, light_color{diff_}, ambient{ambient_}{}
    Vertex shade(const Vertex &a,
                 const Mat4x4f& objToWorld,
                 const Mat4x4f& projection,
                 const Mat4x4f& camView,
                 Light& light) override;
    ~VertexShader() override{}

    void set_light_color(const Vec3f &color)
    {
        this->light_color = color;
    }
private:
    Vec3f dir;
    Vec3f light_color;
    Vec3f ambient;
    float intensity = 1.f;
};


//class GeometryShaderInterface{
//public:
//    virtual Vertex shade(const Vertex &a,
//                         const Mat4x4f& projectMatrix, const Mat4x4f& camView) = 0;
//    virtual ~GeometryShaderInterface(){}
//};

//class GeometryShader: public GeometryShaderInterface{
//public:
//    Vertex shade(const Vertex &a, const Mat4x4f &projection, const Mat4x4f& camView) override;
//    ~GeometryShader() override{}
//};


#endif // SHADER_H
