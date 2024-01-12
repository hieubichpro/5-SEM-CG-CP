#include "model.h"
Vec3f get_normal(const Vertex& a, const Vertex& b, const Vertex &c)
{
    auto res = Vec3f::cross(b.position - a.position, c.position - a.position);
    if (Vec3f::dot(res, a.position) < 0)
        res *= -1;
    return res;
}

void Cube::addFace(Vertex a, Vertex b, Vertex c)
{

    faces.push_back(Face{a, b, c});

    faces.back().normal = get_normal(a, b, c);
}


Cube::Cube()
{
    float step_x = 0.1, step_y = 0.1, step_z = 0.1;
    for (float x = -1.0; x < 1.0; x += step_x)
        for (float y = -1.0; y < 1.0; y += step_y)
        {
            addFace(Vertex{Vec3f{x, y, 1}}, Vertex{Vec3f{x, y + step_y, 1}}, Vertex(Vec3f{x + step_x, y, 1}));
            addFace(Vertex{Vec3f{x + step_x, y + step_y, 1}}, Vertex{Vec3f{x, y + step_y, 1}}, Vertex(Vec3f{x + step_x, y, 1}));
        }

    for (float x = -1.0; x < 1.0; x += step_x)
        for (float y = -1.0; y < 1.0; y += step_y)
        {
            addFace(Vertex{Vec3f{x, y, -1}}, Vertex{Vec3f{x, y + step_y, -1}}, Vertex(Vec3f{x + step_x, y, -1}));
            addFace(Vertex{Vec3f{x + step_x, y + step_y, -1}}, Vertex{Vec3f{x, y + step_y, -1}}, Vertex(Vec3f{x + step_x, y, -1}));
        }

    for (float x = -1.0; x < 1.0; x += step_x)
        for (float z = -1.0; z < 1.0; z += step_z)
        {
            addFace(Vertex{Vec3f{x, 1, z}}, Vertex{Vec3f{x, 1, z + step_z}}, Vertex(Vec3f{x + step_x, 1, z}));
            addFace(Vertex{Vec3f{x + step_x, 1, z + step_z}}, Vertex{Vec3f{x, 1, z + step_z}}, Vertex(Vec3f{x + step_x, 1, z}));
        }
    for (float x = -1.0; x < 1.0; x += step_x)
        for (float z = -1.0; z < 1.0; z += step_z)
        {
            addFace(Vertex{Vec3f{x, -1, z}}, Vertex{Vec3f{x, -1, z + step_z}}, Vertex(Vec3f{x + step_x, -1, z}));
            addFace(Vertex{Vec3f{x + step_x, -1, z + step_z}}, Vertex{Vec3f{x, -1, z + step_z}}, Vertex(Vec3f{x + step_x, -1, z}));
        }

    for (float y = -1.0; y < 1.0; y += step_y)
        for (float z = -1.0; z < 1.0; z += step_z)
        {
            addFace(Vertex{Vec3f{1, y, z}}, Vertex{Vec3f{1, y, z + step_z}}, Vertex(Vec3f{1, y + step_y, z}));
            addFace(Vertex{Vec3f{1, y + step_y, z + step_z}}, Vertex{Vec3f{1, y, z + step_z}}, Vertex(Vec3f{1, y + step_y, z}));
        }
    for (float y = -1.0; y < 1.0; y += step_y)
        for (float z = -1.0; z < 1.0; z += step_z)
        {
            addFace(Vertex{Vec3f{-1, y, z}}, Vertex{Vec3f{-1, y, z + step_z}}, Vertex(Vec3f{-1, y + step_y, z}));
            addFace(Vertex{Vec3f{-1, y + step_y, z + step_z}}, Vertex{Vec3f{-1, y, z + step_z}}, Vertex(Vec3f{-1, y + step_y, z}));
        }
    for(auto &face: faces)
    {
        face.a.normal = face.normal;
        face.b.normal = face.normal;
        face.c.normal = face.normal;
    }

    for (int i = 0; i < 6; i++)
    {
        int idx1 = i * 800;
        for (int j = 0; j < 20; j += 1)
        {
            int idx = j * 40;
            for (int k = 0; k < 20; k++)
            {
                faces[idx1 + idx + 2 * k].a.texture = Vec2f{k / 20.f, j / 20.f};
                faces[idx1 + idx + 2 * k].b.texture = Vec2f{(k + 1) / 20.f, j / 20.f};
                faces[idx1 + idx + 2 * k].c.texture = Vec2f{k / 20.f, (j + 1) / 20.f};

                faces[idx1 + idx + 2 * k + 1].a.texture = Vec2f{(k + 1) / 20.f, (j + 1) / 20.f};
                faces[idx1 + idx + 2 * k + 1].b.texture = Vec2f{(k + 1) / 20.f, j / 20.f};
                faces[idx1 + idx + 2 * k + 1].c.texture = Vec2f{k / 20.f, (j + 1) / 20.f};
            }
         }
    }
}

Vec3f Cylinder::avgNormals(set<int> idxs)
{
    Vec3f res = Vec3f{0, 0, 0};
    for (auto &id : idxs)
    {
        res += faces[id].normal;
    }
    return res;
}
void Cylinder::addFace(int idx_a, int idx_b, int idx_c)
{
    faces.push_back(Face{vertices[idx_a], vertices[idx_b], vertices[idx_c]});
    int idx_face = faces.size() - 1;
    faceNeighbor.at(idx_a).insert(idx_face);
    faceNeighbor.at(idx_b).insert(idx_face);
    faceNeighbor.at(idx_c).insert(idx_face);
}

Cylinder::Cylinder()
{
    int r = 1;
    float step_h = 0.04;
    float step_phi = 10;
    for (float h = -1; h <= 1; h += step_h)
    {
        for (float phi = 0; phi < 360; phi += step_phi)
        {
            float x = r * cos(phi * M_PI / 180.f), y = r * sin(phi * M_PI / 180.f), z = h;
            vertices.push_back(Vertex{Vec3f{x, y, z}});
        }
    }
    vertices.push_back(Vertex{Vec3f{0, 0, -1}});
    vertices.push_back(Vertex{Vec3f{0, 0, 1}});
    for (int i = 0; i < vertices.size(); i++)
    {
        faceNeighbor.insert({i, {}});
        vertices[i].idx = i;
    }
    for (int i = 0; i < 49; i++)
    {
        int top = i * 36;
        int down = (i + 1) * 36;
        for (int j = 0; j < 35; j++)
        {
            addFace(top + j, top + j + 1, down + j);
            addFace(down + j, down + j + 1, top + j + 1);
        }
        addFace(top + 35, top, down + 35);
        addFace(down + 35, down, top);
    }
    auto top1 = vertices.size() - 2;
    auto top2 = vertices.size() - 1;
    for (int i = 0; i < 35; i++)
        addFace(top1, i, i + 1);
    addFace(top1, 35, 0);

    auto start = 49 * 36;
    for (int i = start; i < start + 35; i++)
        addFace(top2, i, i + 1);
    addFace(top2, start + 35, start);

    for(auto &face: faces)
    {
        face.a.normal = avgNormals(faceNeighbor.at(face.a.idx)).normalize();
        face.b.normal = avgNormals(faceNeighbor.at(face.b.idx)).normalize();
        face.c.normal = avgNormals(faceNeighbor.at(face.c.idx)).normalize();
    }
    for (int i = 0; i < 50; i++)
    {
        int idx = i * 72;
        for (int j = 0; j < 36; j++)
        {
            faces[idx + 2 * j].a.texture = Vec2f{j / 36.f, i / 60.f};
            faces[idx + 2 * j].b.texture = Vec2f{(j + 1) / 36.f, i / 60.f};
            faces[idx + 2 * j].c.texture = Vec2f{j / 36.f, (i + 1) / 60.f};

            faces[idx + 2 * j + 1].a.texture = Vec2f{j / 36.f, (i + 1) / 60.f};
            faces[idx + 2 * j + 1].b.texture = Vec2f{(j + 1) / 36.f, (i + 1) / 60.f};
            faces[idx + 2 * j + 1].c.texture = Vec2f{(j + 1) / 36.f, i / 60.f};

        }
    }
}

Vec3f Sphere::avgNormals(set<int> idxs)
{
    Vec3f res = Vec3f{0, 0, 0};
    for (auto &id : idxs)
    {
        res += faces[id].normal;
    }
    return res;
}
void Sphere::addFace(int idx_a, int idx_b, int idx_c)
{
    faces.push_back(Face{vertices[idx_a], vertices[idx_b], vertices[idx_c]});
    int idx_face = faces.size() - 1;
    faceNeighbor.at(idx_a).insert(idx_face);
    faceNeighbor.at(idx_b).insert(idx_face);
    faceNeighbor.at(idx_c).insert(idx_face);
}

Sphere::Sphere()
{
    float r = 1;
    int step_phi = 3;
    int step_theta = 10                                                                                                                             ;
    for (float phi = 0.f; phi <= 180.f; phi += step_phi)
    {
        for (int theta = 0; theta < 360; theta += step_theta)
        {
            float x1 = r * sin(phi * M_PI / 180.f) * cos(theta * M_PI / 180.f);
            float y1 = r * sin(phi * M_PI / 180.f) * sin(theta * M_PI / 180.f);
            float z1 = r * cos(phi * M_PI / 180.f);
            vertices.push_back(Vertex{Vec3f{x1, y1, z1}});
        }
    }

    for (int i = 0; i < vertices.size(); i++)
    {
        faceNeighbor.insert({i, {}});
        vertices[i].idx = i;
    }

    for (int i = 0; i < 60; i++)
    {
        int top = i * 36;
        int down = (i + 1) * 36;
        for (int i = 0; i < 35; i++)
        {
            addFace(top + i, top + i + 1, down + i);
            addFace(down + i, down + i + 1, top + i + 1);
        }
        addFace(top + 35, top, down + 35);
        addFace(down + 35, down, top);
    }
    for(auto &face: faces)
    {
        face.a.normal = avgNormals(faceNeighbor.at(face.a.idx)).normalize();
        face.b.normal = avgNormals(faceNeighbor.at(face.b.idx)).normalize();
        face.c.normal = avgNormals(faceNeighbor.at(face.c.idx)).normalize();
    }
    for (int i = 0; i < 60; i++)
    {
        int idx = i * 72;
        for (int j = 0; j < 36; j++)
        {
            faces[idx + 2 * j].a.texture = Vec2f{j / 36.f, i / 60.f};
            faces[idx + 2 * j].b.texture = Vec2f{(j + 1) / 36.f, i / 60.f};
            faces[idx + 2 * j].c.texture = Vec2f{j / 36.f, (i + 1) / 60.f};

            faces[idx + 2 * j + 1].a.texture = Vec2f{j / 36.f, (i + 1) / 60.f};
            faces[idx + 2 * j + 1].b.texture = Vec2f{(j + 1) / 36.f, (i + 1) / 60.f};
            faces[idx + 2 * j + 1].c.texture = Vec2f{(j + 1) / 36.f, i / 60.f};

        }
    }


}

Mat4x4f Model::objToWorld() const
{
    return Mat4x4f::Scaling(scale_x, scale_y, scale_z) *
           rotation_matrix *
           Mat4x4f::Translation(shift_x, shift_y, shift_z);
}

Model::Model(const string& name_object, int id)
{
    this->id_model = id;
    Cube cube;
    Cylinder cylin;
    Sphere sphere;
    if (name_object == "Куб")
        this->faces = cube.faces;
    else if (name_object == "Цилиндр")
        this->faces = cylin.faces;
    else if (name_object == "Сфера")
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
    auto step = rot_step;
    if (angle < alpha_x)
        step *= -1;
    this->alpha_x = angle;
    rotation_matrix *= Mat4x4f::RotationX(step);
}
void Model::rotateY(float angle)
{
    auto step = rot_step;
    if (angle < alpha_y)
        step *= -1;
    this->alpha_y = angle;
    rotation_matrix *= Mat4x4f::RotationY(step);
}
void Model::rotateZ(float angle)
{
    auto step = rot_step;
    if (angle < alpha_z)
        step *= -1;
    this->alpha_z = angle;
    rotation_matrix *= Mat4x4f::RotationZ(step);
}
