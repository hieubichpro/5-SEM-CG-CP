#ifndef VECTOR_H
#define VECTOR_H
#include <math.h>
#include <algorithm>
#include <iostream>

template <typename T>
class Vec2
{
public:
    T x, y;
public:
    Vec2(T _x = T(0), T _y = T(0)):
        x{_x}, y{_y}{}

    bool operator==(const Vec2& other) const
    {
        return (this->X == other.X && this->Y == other.Y);
    }

    bool operator!=(const Vec2& other) const
    {
        return !(*this == other);
    }

    Vec2 operator+(const Vec2& other)
    {
        return Vec2(this->X + other.X, this->Y + other.Y);
    }

    Vec2 operator-(const Vec2& other)
    {
        return Vec2(this->X - other.X, this->Y - other.Y);
    }
    Vec2 operator*(const T& k)
    {
        return Vec2(this->X*k, this->Y*k);
    }
};

using Vec2f = Vec2<float>;
using Vec2d = Vec2<double>;
using Vec2i = Vec2<int>;

template <typename T>
class Vec3
{
public:
    T x, y, z;
public:
    Vec3(T _x = T(0), T _y = T(0), T _z = T(0)):
        x(_x), y(_y), z(_z){}
    Vec3(const Vec3& other)
    {
        x = other.x;
        y = other.y;
        z = other.z;
    }
    void print_vec()
    {
        std::cout << x << " " << y << " " << z << std::endl;
    }
    T len() const
    {
        return std::sqrt(x * x + y * y + z * z);
    }

    Vec3 normalize() const
    {
        T l = len();
        if (fabs(l) < std::numeric_limits<T>::epsilon())
            l = (T)1;
        return Vec3(x/l, y/l, z/l);
    }

    Vec3& operator =(const Vec3 &other)
    {
        if (this == &other)
            return *this;
        x = other.x;
        y = other.y;
        z = other.z;
        return *this;
    }

    Vec3 operator +(const Vec3 &other) const
    {
        return Vec3(x + other.x, y + other.y, z + other.z);
    }

    Vec3& operator +=(const Vec3 &other)
    {
        *this = *this + other;
        return *this;
    }

    Vec3 operator -()
    {
        return Vec3(-x, -y, -z);
    }

    Vec3 operator -(const Vec3 &other) const
    {
        return Vec3(x - other.x, y - other.y, z - other.z);
    }

    Vec3 operator -=(const Vec3 &other)
    {
        *this = *this - other;
        return *this;
    }

    Vec3 operator *(const T& k)
    {
        return Vec3(x * k, y * k, z * k);
    }

    Vec3 operator *=(const T& k)
    {
        *this = *this * k;
        return *this;
    }

    Vec3 operator /(const T& k)
    {
        return Vec3(x / k, y / k, z / k);
    }

    Vec3 operator /=(const T& k)
    {
        *this = *this / k;
        return *this;
    }

    bool operator ==(const Vec3& other)
    {
        return x == other.x && y == other.y && z == other.z;
    }

    bool operator !=(const Vec3& other)
    {
        return !(*this == other);
    }

    static T dot(const Vec3&a, const Vec3& b){
        return a.x * b.x + a.y * b.y + a.z * b.z;
    }

    static Vec3 cross(const Vec3& a, const Vec3& b){
        return Vec3(a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x);
    }

    Vec3 hadamard(const Vec3& v) const{
        Vec3 out(*this);
        out.x *= v.x;
        out.y *= v.y;
        out.z *= v.z;
        return out;
    }

    Vec3& saturate(){
        x = std::min((T)1.f, std::max((T)0.f, x));
        y = std::min((T)1.f, std::max((T)0.f, y));
        z = std::min((T)1.f, std::max((T)0.f, z));
        return *this;
    }

};

using Vec3f = Vec3<float>;
using Vec3d = Vec3<double>;
using Vec3i = Vec3<int>;


template <typename T>
class Vec4 : public Vec3<T>
{
public:
    T w;
public:
    Vec4() = default;

    Vec4(T x_, T y_, T z_, T w_ = 1.0f):
        Vec3<T>(x_, y_, z_), w(w_){}

    Vec4(const Vec3<T>& v, T w_ = 1.0f):
        Vec3<T>(v.x, v.y, v.z), w(w_){}
    void print_v()
    {
        this->print_vec();
        std::cout << w << std::endl;
    }
    Vec4 operator -() const
    {
        return Vec4(-this->x, -this->y, -this->z, -w);
    }

    Vec4& operator =(const Vec4& v)
    {
        this->x = v.x;
        this->y = v.y;
        this->z = v.z;
        this->w = v.w;
        return *this;
    }

    Vec4 operator +(const Vec4& v)
    {
        return Vec4(this->x + v.x, this->y + v.y, this->z + v.z, this->w + v.w);
    }

    Vec4& operator +=(const Vec4& v)
    {
        *this = *this + v;
        return *this;
    }

    Vec4 operator -(const Vec4& v)
    {
        return Vec4(this->x - v.x, this->y - v.y, this->z - v.z, this->w - v.w);
    }

    Vec4& operator -=(const Vec4& v)
    {
        *this = *this - v;
        return *this;
    }

    Vec4 operator *(const T& val)
    {
        return Vec4(this->x * val, this->y * val, this->z * val, this->w * val);
    }

    Vec4& operator *=(const T& val)
    {
        *this = *this * val;
        return *this;
    }

    Vec4 operator /(const T& val)
    {
        return Vec4(this->x / val, this->y / val, this->z / val, this->w / val);
    }

    Vec4& operator /=(const T& val)
    {
        *this = *this / val;
        return *this;
    }

    Vec4& operator ==(const Vec4& v) const
    {
        return this->x == v.x and this->y == v.y and this->z == v.z and this->w == v.w;
    }

    Vec4& operator !=(const Vec4& v) const
    {
        return !(*this == v);
    }

};


using Vec4f = Vec4<float>;
using Vec4d = Vec4<double>;
using Vec4i = Vec4<int>;

#endif // VECTOR_H
