#include "model.h"
#include "loader.h"
Vec3f get_normal(const Vertex& a, const Vertex& b, const Vertex &c)
{
    auto res = Vec3f::cross(b.position - a.position, c.position - a.position);
    if (Vec3f::dot(res, a.position) < 0)
        res *= -1;
    return res;
}

Plane::Plane()
{

}

Cube::Cube()
{
    vector<Vertex> vertices;
    vertices.push_back(Vertex{Vec3f{1, 1, -1}});
    vertices.push_back(Vertex{Vec3f{-1, 1, -1}});
    vertices.push_back(Vertex{Vec3f{-1, -1, -1}});
    vertices.push_back(Vertex{Vec3f{1, -1, -1}});
    vertices.push_back(Vertex{Vec3f{1, 1, 1}});
    vertices.push_back(Vertex{Vec3f{-1, 1, 1}});
    vertices.push_back(Vertex{Vec3f{-1, -1, 1}});
    vertices.push_back(Vertex{Vec3f{1, -1, 1}});

    faces.push_back(Face(vertices[0], vertices[2], vertices[1]));
    faces.push_back(Face(vertices[0], vertices[2], vertices[3]));
    faces.push_back(Face(vertices[1], vertices[4], vertices[0]));
    faces.push_back(Face(vertices[1], vertices[4], vertices[5]));
    faces.push_back(Face(vertices[6], vertices[1], vertices[5]));
    faces.push_back(Face(vertices[6], vertices[1], vertices[2]));
    faces.push_back(Face(vertices[7], vertices[2], vertices[6]));
    faces.push_back(Face(vertices[7], vertices[2], vertices[3]));
    faces.push_back(Face(vertices[7], vertices[0], vertices[4]));
    faces.push_back(Face(vertices[7], vertices[0], vertices[3]));
    faces.push_back(Face(vertices[7], vertices[5], vertices[6]));
    faces.push_back(Face(vertices[7], vertices[5], vertices[4]));

    for (int i = 0; i < faces.size(); i += 2)
    {
        faces[i].a.texture = Vec2f{0.0, 0.0};
        faces[i].b.texture = Vec2f{1.0, 1.0};
        faces[i].c.texture = Vec2f{0.0, 1.0};

        faces[i + 1].a.texture = Vec2f{0.0, 0.0};
        faces[i + 1].b.texture = Vec2f{1.0, 1.0};
        faces[i + 1].c.texture = Vec2f{1.0, 0.0};
    }
}


Cylinder::Cylinder()
{
    int r = 1;
    int h = 2;
    vector<Vertex> vertices_1;
    vector<Vertex> vertices_2;
    for (int phi = 0; phi < 360; phi += 10)
    {
        float x = r * cos(phi * M_PI / 180.f), y = r * sin(phi * M_PI / 180.f), z1 = -h / 2.f, z2 = h / 2.f;
        vertices_1.push_back(Vertex{Vec3f{x, y, z1}});
        vertices_2.push_back(Vertex{Vec3f{x, y, z2}});
    }

    for (int i = 0; i < 35; i++)
    {
        faces.push_back(Face(vertices_1[i], vertices_1[i + 1], vertices_2[i]));
        faces.push_back(Face(vertices_2[i], vertices_2[i + 1], vertices_1[i + 1]));
    }
    faces.push_back(Face(vertices_1[35], vertices_1[0], vertices_2[35]));
    faces.push_back(Face(vertices_2[35], vertices_2[0], vertices_1[0]));
    for (int i = 0; i < 35; i++)
    {
        faces.push_back(Face(vertices_1[i], vertices_1[i + 1], Vec3f{0, 0, -h / 2.f}));
        faces.push_back(Face(vertices_2[i], vertices_2[i + 1], Vec3f{0, 0, h / 2.f}));
    }
    faces.push_back(Face(vertices_1[35], vertices_1[0], Vec3f{0, 0, -h / 2.f}));
    faces.push_back(Face(vertices_2[35], vertices_2[0], Vec3f{0, 0, h / 2.f}));

    for (int i = 0; i < 36; i += 1)
    {
        faces[2 * i].a.texture = Vec2f{i / 36.f, 0};
        faces[2 * i].b.texture = Vec2f{(i + 1) / 36.f, 0};
        faces[2 * i].c.texture = Vec2f{i / 36.f, 1};

        faces[2 * i + 1].a.texture = Vec2f{i / 36.f, 1};
        faces[2 * i + 1].b.texture = Vec2f{(i + 1) / 36.f, 1};
        faces[2 * i + 1].c.texture = Vec2f{(i + 1) / 36.f, 0};
    }
}

Sphere::Sphere()
{
    float r = 1;
    int step_phi = 3;
    int step_theta = 5                                                                                                                             ;
    for (float phi = 0.f; phi < 180.f; phi += step_phi)
    {
        vector<Vertex> top;
        vector<Vertex> down;
        for (int theta = 0; theta < 360; theta += step_theta)
        {
            float x1 = r * sin(phi * M_PI / 180.f) * cos(theta * M_PI / 180.f);
            float y1 = r * sin(phi * M_PI / 180.f) * sin(theta * M_PI / 180.f);
            float z1 = r * cos(phi * M_PI / 180.f);
            float x2 = r * sin((phi + step_phi) * M_PI / 180.f) * cos(theta * M_PI / 180.f);
            float y2 = r * sin((phi + step_phi) * M_PI / 180.f) * sin(theta * M_PI / 180.f);
            float z2 = r * cos((phi + step_phi) * M_PI / 180.f);
            top.push_back(Vertex{Vec3f{x1, y1, z1}});
            down.push_back(Vertex{Vec3f{x2, y2, z2}});
        }
        for (int i = 0; i < 71; i++)
        {
            faces.push_back(Face(top[i], top[i + 1], down[i]));
            faces.push_back(Face(down[i], down[i + 1], top[i + 1]));
        }
        faces.push_back(Face(top[71], top[0], down[71]));
        faces.push_back(Face(down[71], down[0], top[0]));
    }

    for (int i = 0; i < 60; i++)
    {
        int idx = i * 144;
        for (int j = 0; j < 72; j++)
        {
            faces[idx + 2 * j].a.texture = Vec2f{j / 72.f, i / 60.f};
            faces[idx + 2 * j].b.texture = Vec2f{(j + 1) / 72.f, i / 60.f};
            faces[idx + 2 * j].c.texture = Vec2f{j / 72.f, (i + 1) / 60.f};

            faces[idx + 2 * j + 1].a.texture = Vec2f{j / 72.f, (i + 1) / 60.f};
            faces[idx + 2 * j + 1].b.texture = Vec2f{(j + 1) / 72.f, (i + 1) / 60.f};
            faces[idx + 2 * j + 1].c.texture = Vec2f{(j + 1) / 72.f, i / 60.f};

        }
    }


}

Mat4x4f Model::objToWorld() const
{
    return Mat4x4f::Scaling(scale_x, scale_y, scale_z) *
           Mat4x4f::Rotation(alpha_x, alpha_y, alpha_z) *
           Mat4x4f::Translation(shift_x, shift_y, shift_z);
}

Model::Model(const string& name_object, int id)
{
    this->id_model = id;
    Cube cube;
    Cylinder cylin;
    Sphere sphere;
    if (name_object == "Cube")
        this->faces = cube.faces;
    else if (name_object == "Cylinder")
        this->faces = cylin.faces;
    else if (name_object == "Sphere")
        this->faces = sphere.faces;
}

void Model::moveX(float offs)
{
    this->shift_x = offs;
}
void Model::moveY(float offs)
{
    this->shift_y = offs;
}
void Model::moveZ(float offs)
{
    this->shift_z = offs;
}
void Model::scaleX(float kx)
{
    this->scale_x = kx;
}
void Model::scaleY(float ky)
{
    this->scale_y = ky;
}
void Model::scaleZ(float kz)
{
    this->scale_z = kz;
}

void Model::rotateX(float angle)
{
    this->alpha_x = angle;
}
void Model::rotateY(float angle)
{
    this->alpha_y = angle;
}
void Model::rotateZ(float angle)
{
    this->alpha_z = angle;
}
