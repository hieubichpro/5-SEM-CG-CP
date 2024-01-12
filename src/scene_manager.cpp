#include "scene_manager.h"
#include <iostream>

#define NDCX_TO_RASTER(ndc_x, width) (((ndc_x + 1.0f) * (width >> 1)))
#define NDCY_TO_RASTER(ndc_y, height) (((1.0f - ndc_y) * (height >> 1)))

bool SceneManager::testDepth(const Vec3f& p)
{
    int x = round(p.x), y = round(p.y);
    if (x >= width || y >= height || x < 0 || y < 0)
        return false;
    if (p.z < Zbuffer[x][y])
    {
        Zbuffer[x][y] = p.z;
        return true;
    }
    return false;
}

void denormolize(int width, int height, Vertex& v)
{
    v.position.x = NDCX_TO_RASTER(v.position.x, width);
    v.position.y = NDCY_TO_RASTER(v.position.y, height);
}

void SceneManager::rasterizeTriangle(Vertex a, Vertex b, Vertex c)
{
    denormolize(width, height, a);
    denormolize(width, height, b);
    denormolize(width, height, c);

    auto p1 = a.position;
    auto p2 = b.position;
    auto p3 = c.position;

    float sx = floor(min(min(p1.x, p2.x), p3.x));
    float ex = ceil(max(max(p1.x, p2.x), p3.x));

    float sy = floor(min(min(p1.y, p2.y), p3.y));
    float ey = ceil(max(max(p1.y, p2.y), p3.y));

    for (int y = static_cast<int>(sy); y < static_cast<int>(ey); y++)
    {
        for (int x = static_cast<int>(sx); x < static_cast<int>(ex); x++)
        {
            Vec3f bary = getBarycentric(p1, p2, p3, Vec3f(static_cast<float>(x), static_cast<float>(y)));
            if ( (bary.x > 0.0f || fabs(bary.x) < eps) && (bary.x < 1.0f || fabs(bary.x - 1.0f) < eps) &&
                 (bary.y > 0.0f || fabs(bary.y) < eps) && (bary.y < 1.0f || fabs(bary.y - 1.0f) < eps) &&
                 (bary.z > 0.0f || fabs(bary.z) < eps) && (bary.z < 1.0f || fabs(bary.z - 1.0f) < eps))
            {
                auto interpolated = baryCentricInterpolation(p1, p2, p3, bary);
                interpolated.x = x;
                interpolated.y = y;
                if (testDepth(interpolated))
                {
                    auto pixel_color = pixel_shader->shade(a, b, c, bary) * 255.f;
                    img.setPixelColor(x, y, qRgb(pixel_color.x, pixel_color.y, pixel_color.z));
                }
            }
        }
    }
}

void SceneManager::rasterize(Model& m)
{
    auto projectMatrix = camera.projectionMatrix;
    auto viewMatrix = camera.viewMatrix();
    auto objToWorld = m.objToWorld();
    auto rotation_matrix = m.rotation_matrix;

    for (auto& face: m.faces)
    {
        auto a = vertex_shader->shade(face.a, objToWorld, rotation_matrix, projectMatrix, viewMatrix, light, m, camera);
        auto b = vertex_shader->shade(face.b, objToWorld, rotation_matrix, projectMatrix, viewMatrix, light, m, camera);
        auto c = vertex_shader->shade(face.c, objToWorld, rotation_matrix, projectMatrix, viewMatrix, light, m, camera);
        rasterizeTriangle(a, b, c);
    }
}

void SceneManager::render()
{
    this->img.fill(Qt::black);
    for (auto &row : this->Zbuffer)
        std::fill(row.begin(), row.end(), numeric_limits<float>::max());
    for (auto& m : models)
    {
        if (m->isTextured)
            pixel_shader = std::make_shared<TextureShader>(m->texture);
        rasterize(*m);
    }
    show();
}

void SceneManager::show()
{
    scene->clear();
    scene->addPixmap(QPixmap::fromImage(img));
}

void SceneManager::uploadModel(std::string name, int &id)
{
    id = idx_model++;
    this->models.push_back(new Model(name, id));
    this->render();
}

void SceneManager::initialize()
{
    pixel_shader = std::make_shared<ColorShader>();
    vertex_shader = std::make_shared<VertexShader>();
    render();
}

void SceneManager::setCurrModel(int id)
{
    int i = 0;
    for (auto it = models.begin() ; it <  models.end(); it++, i++)
        if ((*it)->id_model == id)
            break;
    curr_model = i;
}

void SceneManager::deleteModel()
{
    this->models.erase(this->models.begin() + curr_model);
    this->render();
}

void SceneManager::move(type t, float offs)
{
    switch (t)
    {
        case MOVE_X:
            models[curr_model]->moveX(offs);
            break;
        case MOVE_Y:
            models[curr_model]->moveY(offs);
            break;
        case MOVE_Z:
            models[curr_model]->moveZ(offs);
        default:
            break;
    }
    render();
}
void SceneManager::scale(type t, float k)
{
    switch (t)
    {
        case SCALE_X:
            models[curr_model]->scaleX(k);
            break;
        case SCALE_Y:
            models[curr_model]->scaleY(k);
            break;
        case SCALE_Z:
            models[curr_model]->scaleZ(k);
        default:
            break;
    }
    render();
}
void SceneManager::rotate(type t, float angle)
{
    switch (t)
    {
        case ROTATE_X:
            models[curr_model]->rotateX(angle);
            break;
        case ROTATE_Y:
            models[curr_model]->rotateY(angle);
            break;
        case ROTATE_Z:
            models[curr_model]->rotateZ(angle);
        default:
            break;
    }
    render();
}

void SceneManager::changeIntensity(double param)
{
    light.intensity = param;
    render();
}

void SceneManager::changeAmbient(double param)
{
    models[curr_model]->ambient_coef = param;
    render();
}
void SceneManager::changeDiffuse(double param)
{
    models[curr_model]->diffuse_coef = param;
    render();
}
void SceneManager::changeSpecular(double param)
{
    models[curr_model]->specular_coef = param;
    render();
}
void SceneManager::changeAlpha(double param)
{
    models[curr_model]->alpha_coef = param;
    render();
}

void SceneManager::setTexture(QImage &img)
{
    models[curr_model]->texture = img;
    models[curr_model]->isTextured = true;
    render();
}

void SceneManager::moveCamera(type t, float dist)
{
    std::function<void ()> change_func;
    switch (t)
    {
    case MOVE_X:
        change_func = [&]()
        {
            camera.shiftX(dist);
        };
        break;
    case MOVE_Z:
        change_func = [&]()
        {
            camera.shiftZ(dist);
        };
        break;
    case ROTATE_X:
        change_func = [&]()
        {
            camera.rotateX(dist);
        };
        break;
    case ROTATE_Y:
        change_func = [&]()
        {
            camera.rotateY(dist);
        };
        break;
    default:
        return;
    }

    change_func();
    render();
}
const float offs = 15;
void SceneManager::moveLight(type_light t)
{
    switch(t)
    {
    case LEFT:
        light.shiftX(-offs);
        break;
    case RIGHT:
        light.shiftX(offs);
        break;
    case TOP:
        light.shiftY(offs);
        break;
    case DOWN:
        light.shiftY(-offs);
        break;
    case NEAR:
        light.shiftZ(offs);
        break;
    case FAR:
        light.shiftZ(-offs);
        break;
    }
    render();
}
float random_num(double min, double max) {
    static std::random_device rd;
    static std::mt19937 generator(rd());
    std::uniform_real_distribution<double> distribution(min, max);
    return distribution(generator);
}

// Функция для создания произвольного вектора на основе заданного вектора
Vec3f random_vector(Vec3f a) {
    float n1, n2, n3;
    for (int i = 0; i < 3; i++)
    {
        n1 = random_num(-5, 5) * i + random_num(3, 3);
        n2 = random_num(-5, 5) * i + random_num(3, 3);
        n3 = random_num(-5, 5) * i + random_num(3, 3);
    }

    return (a + Vec3f{n1, n2, n3}.normalize()).normalize();
}
void SceneManager::bump_map()
{
    auto m = models[curr_model];
    for (auto& face : m->faces)
    {
        face.b.normal = random_vector(face.b.normal);
        face.c.normal = random_vector(face.c.normal);
    }
    render();
}
//void SceneManager::test_proga()
//{
//    vector<int> arr;
//    for (int i = 5; i <= 50; i += 5)
//        arr.push_back(i);
//    ofstream f = ofstream("../res.txt");
//    for (int i = 0; i < arr.size(); i++)
//    {
//        double time1 = 0, time2 = 0, time3 = 0;
//        clock_t beg, end;
//        for (int j = 0; j < arr[i]; j++)
//        {
//            this->models.push_back(new Model("Куб", j));
//        }
//        beg = clock();
//        this->render();
//        end = clock();
//        time1 = (end - beg) * 1000 / CLOCKS_PER_SEC;

//        this->models.clear();

//        for (int j = 0; j < arr[i]; j++)
//        {
//            this->models.push_back(new Model("Цилиндр", j));
//        }
//        beg = clock();
//        this->render();
//        end = clock();
//        time2 = (end - beg) * 1000 / CLOCKS_PER_SEC;

//        this->models.clear();

//        for (int j = 0; j < arr[i]; j++)
//        {
//            this->models.push_back(new Model("Сфера", j));
//        }
//        beg = clock();
//        this->render();
//        end = clock();
//        time3 = (end - beg) * 1000 / CLOCKS_PER_SEC;

//        this->models.clear();

//        f << time1 << "," << time2 << "," << time3 << endl;
//    }
//    f.close();
//}
