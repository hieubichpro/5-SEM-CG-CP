#ifndef SCENE_MANAGER_H
#define SCENE_MANAGER_H
#include <string>
#include <vector>
#include <numeric>
#include <map>
#include <memory>
#include <QImage>
#include "model.h"
#include "camera.h"
#include "shader.h"
#include "barycenter.h"
#include "texture.h"
#include <QGraphicsScene>
#include "light.h"
#include <time.h>
#include <random>
#include <fstream>
#define eps 1e-6

enum type {MOVE_X, MOVE_Y, MOVE_Z,
          ROTATE_X, ROTATE_Y, ROTATE_Z,
          SCALE_X, SCALE_Y, SCALE_Z, UP_Y, DOWN_Y};

enum type_light {LEFT, RIGHT, TOP, DOWN, NEAR, FAR};

using namespace std;
class SceneManager
{
public:
    SceneManager() = default;

    SceneManager(int width_, int height_, QColor background_, QGraphicsScene* scene_): width{width_}, height{height_},
        background(background_), scene{scene_}{

        img = QImage(width, height, QImage::Format_RGB32);
        Zbuffer.resize(width);
        for (auto &vec : Zbuffer)
            vec.resize(height, std::numeric_limits<float>::max());

        img.fill(background);
        camera = Camera(width, height);
    }
    void uploadModel(std::string name, int &id);
    void deleteModel();
    void render();
    void rasterize(Model &m);
    void rasterizeTriangle(Vertex a, Vertex b, Vertex c);
    bool testDepth(const Vec3f& p);
    void show();
    void initialize();
    void setCurrModel(int id);
    void move(type t, float offs);
    void scale(type t, float k);
    void rotate(type t, float angle);
    void setTexture(QImage &img);
    void moveCamera(type t, float dist);
    void moveLight(type_light t);
    void changeIntensity(double param);
    void changeAmbient(double param);
    void changeDiffuse(double param);
    void changeSpecular(double param);
    void changeAlpha(double param);
    void bump_map();
//    void test_proga();

private:
    QImage img;
    vector<Model*> models;
    vector<vector<float>> Zbuffer;
    Camera camera;
    int curr_model = 0;
    int idx_model = 1;
    std::shared_ptr<PixelShaderInterface> pixel_shader;
    std::shared_ptr<VertexShaderInterface> vertex_shader;
    int width, height;
    QGraphicsScene *scene;
    QColor background;
    Light light;
};

#endif // SCENE_MANAGER_H
