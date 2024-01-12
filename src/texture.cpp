#include "texture.h"
#include "barycenter.h"
#include <QtDebug>
#include <QGraphicsScene>

TextureShader::TextureShader(const QImage& img){
    texture = img;
}

Vec3f TextureShader::shade(const Vertex &a, const Vertex &b, const Vertex &c, const Vec3f &bary){

    auto face_color = baryCentricInterpolation(a.color, b.color, c.color, bary);

    float pixel_u = interPolateCord(a.texture.x , b.texture.x, c.texture.x, bary);
    float pixel_v = interPolateCord(a.texture.y , b.texture.y, c.texture.y, bary);

    int x = std::floor(pixel_u * (texture.width()) - 1);
    int y = std::floor(pixel_v * (texture.height() - 1));

    if (x < 0) x = 0;
    if (y < 0) y = 0;


    auto color = texture.pixelColor(x, y);
    auto red = (float)color.red();
    auto green = (float)color.green();
    auto blue = (float)color.blue();
    Vec3f pixel_color = Vec3f{red / 255.f, green/ 255.f , blue /255.f};

    auto t = pixel_color.hadamard(face_color).saturate();

    return t;
}



