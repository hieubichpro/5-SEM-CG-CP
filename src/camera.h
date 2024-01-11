#ifndef CAMERA_H
#define CAMERA_H
#include "vector.h"
#include "matrix.h"
#include <QDebug>
#include <iostream>
const float eps_cam = 1e-5;

class Camera
{
    public:
    Camera(){}
    Camera(float width, float height, Vec3f pos = {0, 0, -10}, Vec3f up = {0, 1, 0}, Vec3f target = {0, 0, 1},
           float fov = 45, float zn_ = 0.1f, float zf_ = 1000.f): position{pos}, up{up}, target{target},
                                                                  fov{fov}, zn{zn_}, zf{zf_}
    {
        aspect_ratio = width / height;
        projectionMatrix = Mat4x4f::ProjectionFOV(fov, aspect_ratio, zn, zf);
    }

    Mat4x4f viewMatrix() const
    {
        return Mat4x4f::LookAtLH(position, position + target, up);
    }

    void shiftX(float dist)
    {
        auto normal = Vec3f::cross(target, up).normalize();
        normal *= dist;
        position += normal;
    }

    void shiftZ(float dist)
    {
        position += target.normalize() * dist;
    }

    void shiftY(float dist)
    {
        position.y += dist;
    }

    void rotateX(float angle)
    {

        target = target * Mat3x3f::RotationX(angle);
        up = up * Mat3x3f::RotationX(angle) ;
    }

    void rotateY(float angle)
    {
        target = target * Mat3x3f::RotationY(angle);
        up = up * Mat3x3f::RotationY(angle);
    }

public:
    Vec3f position;
    Vec3f up;
    Vec3f target;
    float fov;
    float zn;
    float zf;
    float aspect_ratio;
    Mat4x4f projectionMatrix;
};

#endif // CAMERA_H
