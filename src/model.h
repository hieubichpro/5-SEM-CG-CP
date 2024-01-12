#ifndef MODEL_H
#define MODEL_H
#include "vector.h"
#include "matrix.h"
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <QImage>
#include <set>
using namespace std;

const float rot_step = 15;

class Vertex
{
public:
    Vec3f position;
    Vec3f normal;
    Vec2f texture;
    Vec3f color = {0.5f, 0.5f, 0.5f};
    float invW = 1.f;
    int idx;

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
    Mat4x4f rotation_matrix = Mat4x4f::Identity();
    float ambient_coef = 0.3f;
    float diffuse_coef = 0.7f;
    float specular_coef = 0.8f;
    float alpha_coef = 128;

private:
    float alpha_x = 0.f, alpha_y = 0.f, alpha_z = 0.f;
    float shift_x = 0.f, shift_y = 0.f, shift_z = 0.f;
    float scale_x = 1.f, scale_y = 1.f, scale_z = 1.f;
};

class Cube
{
public:
    Cube();
    void addFace(Vertex a, Vertex b, Vertex c);
public:
    vector<Vec3f> vertexCoords;
    vector<Vertex> vertices;
    vector<Face> faces;
    map<int, set<int>> faceNeighbor;
};


class Cylinder
{
public:
    Cylinder();
    Vec3f avgNormals(set<int> idxs);
    void addFace(int idx_a, int idx_b, int idx_c);
public:
    vector<Vec3f> vertexCoords;
    vector<Vertex> vertices;
    vector<Face> faces;
    map<int, set<int>> faceNeighbor;
};

class Sphere
{
public:
    Sphere();
    Vec3f avgNormals(set<int> idxs);
    void addFace(int idx_a, int idx_b, int idx_c);
public:
    vector<Vec3f> vertexCoords;
    vector<Vertex> vertices;
    vector<Face> faces;
    map<int, set<int>> faceNeighbor;
};

#endif // MODEL_H
