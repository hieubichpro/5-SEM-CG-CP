#ifndef MODEL_H
#define MODEL_H
#include "vector.h"
#include "matrix.h"
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <QImage>
using namespace std;

const float rot_step = 15;

class Vertex
{
public:
    Vec3f position;
    Vec3f normal;
    Vec2f texture;
    Vec3f color = {0.5, 0.5, 0.5};
    float invW = 1.f;

    Vertex(Vec3f _position):
        position{_position} {}
};

Vec3f get_normal(const Vertex& a, const Vertex& b, const Vertex &c);

class Face
{
public:
    Vertex a, b, c;
    Vec3f normal;
public:
    Face(Vertex a_, Vertex b_, Vertex c_):
        a{a_}, b{b_}, c{c_}
    {
        normal = get_normal(a, b, c);
        a.normal = this->normal;
        b.normal = this->normal;
        c.normal = this->normal;
    }
};

class Model
{
    Model() = default;
public:
    Model(const string& name_object, int id);
    Mat4x4f objToWorld() const;
    void moveX(float offs);
    void moveY(float offs);
    void moveZ(float offs);
    void scaleX(float kx);
    void scaleY(float ky);
    void scaleZ(float kz);
    void rotateX(float angle);
    void rotateY(float angle);
    void rotateZ(float angle);

public:
    vector<Face> faces;
    int id_model;
    QImage texture;
    bool isTextured = false;
private:
    float alpha_x = 0.f, alpha_y = 0.f, alpha_z = 0.f;
    float shift_x = 0.f, shift_y = 0.f, shift_z = 0.f;
    float scale_x = 1.f, scale_y = 1.f, scale_z = 1.f;
};

class Cube
{
public:
    Cube();
public:
    vector<Face> faces;
};


class Cylinder
{
public:
    Cylinder();
public:
    vector<Face> faces;
};

class Sphere
{
public:
    Sphere();
public:
    vector<Face> faces;
};

class Plane
{
public:
    Plane();
public:
    vector<Face> faces;
};

#endif // MODEL_H
