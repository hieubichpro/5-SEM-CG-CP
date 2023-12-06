#ifndef CAMERA_H
#define CAMERA_H
#include "vector.h"
#include "matrix.h"
#include <QDebug>
#include <iostream>
inline double radians(const double &angle) { return angle * M_PI / 180; }
inline double degrees(const double &angle) { return angle * 180 / M_PI; }

class Camera
{
    public:
    Camera(float width, float height, Vec3f pos = {0, 0, -7}, Vec3f top = {0, 1, 0}, float yaw = -90, float pitch = 0,
           float fov = 45, float zn_ = 0.1f, float zf_ = 1000.f)
        :position{pos}, top{top}, yaw{yaw}, pitch{pitch}, fov{fov}, zn{zn_}, zf{zf_}
    {
        aspect_ratio = width / height;
        projectionMatrix = Mat4x4f::ProjectionFOV(fov, aspect_ratio, zn, zf);
        updateCoords();
    }

    Mat4x4f getLookAt() const
    {
        Mat4x4f rotate = Mat4x4f::getRotation(right, up, direction);
        Mat4x4f translate = Mat4x4f::getTranslation(position);
        return translate * rotate;
    }
    void updateCoords()
    {
        float yaw = radians(yaw);
        float pitch = radians(pitch);
        direction.x = cos(yaw) * cos(pitch);
        direction.y = sin(pitch);
        direction.z = sin(yaw) * cos(pitch);
        direction.normalize();
        right = Vec3f::cross(top, direction).normalize();
        up = Vec3f::cross(direction, right).normalize();
        up.print_vec();
        direction.print_vec();
        right.print_vec();
    }
    void shiftX(float dist)
    {
        position += right * dist;
    }

    void shiftZ(float dist)
    {
        position += direction * dist;
    }


    void rotate(float angle)
    {
        float temp_yaw = yaw;
        float temp_pitch = pitch;
        temp_yaw += angle;
        temp_pitch += angle;
        if (temp_pitch >= 90)
        {
            temp_pitch = 90;
        }
        else if (temp_pitch <= -90)
        {
            temp_pitch = -90;
        }
        if (temp_yaw >= 180)
        {
            temp_yaw = -180;
        }
        else if (temp_yaw <= -180)
        {
            temp_yaw = 180;
        }
        this->yaw = temp_yaw;
        this->pitch = temp_pitch;
        updateCoords();
    }

public:
    Vec3f position;
    Vec3f direction;
    Vec3f up;
    Vec3f right;
    Vec3f top;
    float yaw;
    float pitch;

    float fov;
    float zn;
    float zf;
    float aspect_ratio;
    Mat4x4f projectionMatrix;

};

#endif // CAMERA_H
