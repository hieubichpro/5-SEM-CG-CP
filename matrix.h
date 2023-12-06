#ifndef MATRIX_H
#define MATRIX_H

#include "vector.h"
#include <cstring>
#include <iostream>

template <typename T, size_t S>
class Matrix
{
public:
    T arr[S][S];
public:

    Matrix& operator =(const Matrix& m)
    {
        memcpy(arr, m.arr, sizeof(arr));
        return *this;
    }

    Matrix operator *(const T& k)
    {
        Matrix res = *this;
        for( auto& row : res.arr )
        {
            for( T& e : row )
            {
                e *= k;
            }
        }
        return res;
    }

    Matrix& operator *=(const T& k)
    {
        *this = *this * k;
        return *this;
    }

    Matrix operator *(const Matrix& mat)
    {
        Matrix result;
        for( size_t j = 0; j < S; j++ )
        {
            for( size_t k = 0; k < S; k++ )
            {
                T sum = (T)0.0;
                for( size_t i = 0; i < S; i++ )
                {
                    sum += arr[j][i] * mat.arr[i][k];
                }
                result.arr[j][k] = sum;
            }
        }
        return result;
    }

    Matrix& operator *=(const Matrix& mat)
    {
        *this = *this * mat;
        return *this;
    }

    // транспонирование
    Matrix operator !() const
    {
        Matrix xp;
        for( size_t j = 0; j < S; j++ )
        {
            for( size_t k = 0; k < S; k++ )
            {
                xp.arr[j][k] = arr[k][j];
            }
        }
        return xp;
    }

    static Matrix Identity()
    {
        if constexpr (S == 3)
        {
            return
            {
                (T)1.0,(T)0.0,(T)0.0,
                (T)0.0,(T)1.0,(T)0.0,
                (T)0.0,(T)0.0,(T)1.0,
            };
        }
        else if constexpr (S == 4)
        {
            return
            {
                (T)1.0,(T)0.0,(T)0.0,(T)0.0,
                (T)0.0,(T)1.0,(T)0.0,(T)0.0,
                (T)0.0,(T)0.0,(T)1.0,(T)0.0,
                (T)0.0,(T)0.0,(T)0.0,(T)1.0,
            };
        }
    }

    static Matrix Scaling(const T& factor_x, const T& factor_y, const T& factor_z){
        if constexpr(S == 3)
        {
            return
            {
                factor_x,(T)0.0,(T)0.0,
                (T)0.0,factor_y,(T)0.0,
                (T)0.0,(T)0.0,factor_z,
            };
        }
        else if constexpr( S == 4 )
        {
            return
            {
                factor_x,(T)0.0,(T)0.0,(T)0.0,
                (T)0.0,factor_y,(T)0.0,(T)0.0,
                (T)0.0,(T)0.0,factor_z,(T)0.0,
                (T)0.0,(T)0.0,(T)0.0,(T)1.0,
            };
        }
    }

    static Matrix RotationZ(const T& theta)
    {
        const T angle = theta * M_PI / (T)180;
        const T sinTheta = sin( angle );
        const T cosTheta = cos( angle );
        if( S == 3 )
        {
            return
            {
                 cosTheta, sinTheta, (T)0.0,
                -sinTheta, cosTheta, (T)0.0,
                (T)0.0,    (T)0.0,   (T)1.0,
            };
        }
        else if constexpr( S == 4 )
        {
            return
            {
                 cosTheta, sinTheta, (T)0.0,(T)0.0,
                -sinTheta, cosTheta, (T)0.0,(T)0.0,
                (T)0.0,    (T)0.0,   (T)1.0,(T)0.0,
                (T)0.0,	   (T)0.0,   (T)0.0,(T)1.0,
            };
        }
    }

    static Matrix RotationY(const T& theta)
    {
        const T angle = theta * M_PI / (T)180;
        const T sinTheta = sin( angle );
        const T cosTheta = cos( angle );
        if ( S == 3 )
        {
            return
            {
                 cosTheta, (T)0.0,-sinTheta,
                 (T)0.0,   (T)1.0, (T)0.0,
                 sinTheta, (T)0.0, cosTheta
            };
        }
        else if constexpr ( S == 4 )
        {
            return
            {
                cosTheta, (T)0.0, -sinTheta,(T)0.0,
                (T)0.0,   (T)1.0, (T)0.0,   (T)0.0,
                sinTheta, (T)0.0, cosTheta, (T)0.0,
                (T)0.0,   (T)0.0, (T)0.0,   (T)1.0,
            };
        }
    }

    static Matrix RotationX(const T& theta){
        const T angle = theta * M_PI / (T)180;
        const T sinTheta = sin( angle );
        const T cosTheta = cos( angle );
        if ( S == 3 )
        {
            return
            {
                (T)1.0, (T)0.0,   (T)0.0,
                (T)0.0, cosTheta, sinTheta,
                (T)0.0,-sinTheta, cosTheta,
            };
        }
        else if constexpr ( S == 4 )
        {
            return
            {
                (T)1.0, (T)0.0,   (T)0.0,  (T)0.0,
                (T)0.0, cosTheta, sinTheta,(T)0.0,
                (T)0.0,-sinTheta, cosTheta,(T)0.0,
                (T)0.0, (T)0.0,   (T)0.0,  (T)1.0,
            };
        }
    }
    static Matrix Rotation(const T& alpha_x, const T& alpha_y, const T& alpha_z)
    {
        if (S == 4)
        {
            return RotationX(alpha_x) * RotationY(alpha_y) * RotationZ(alpha_z);
        }
    }
    static Matrix Translation(T x, T y, T z)
    {
        if ( S == 4 )
        {
            return
            {
                (T)1.0,(T)0.0,(T)0.0,(T)0.0,
                (T)0.0,(T)1.0,(T)0.0,(T)0.0,
                (T)0.0,(T)0.0,(T)1.0,(T)0.0,
                x,     y,      z,    (T)1.0,
            };
        }
    }
    static Matrix ProjectionFOV(T fov,T ar,T n,T f){
        if constexpr( S == 4 ){
            const T fov_rad = fov * (T)M_PI / (T)180.0;
            const T yScale = (T)1.0 / (T)std::tan(fov_rad * (T)0.5);
            const T xScale = yScale / ar;
            return{
                xScale,  (T)0.0,  (T)0.0,				(T)0.0,
                (T)0.0,  yScale,	(T)0.0,				(T)0.0,
                (T)0.0,	 (T)0.0,	 -(f + n) / (f - n),  (T)1.0,
                (T)0.0,	 (T)0.0,	 -(2 * f * n) / (f - n),	(T)0.0,
            };
        }
    }

    static Matrix LookAtLH(Vec3f position, Vec3f at, Vec3f up){
        if constexpr (S == 4){
            auto zaxis = (at - position).normalize();
            auto xaxis = (Vec3f::cross(up, zaxis)).normalize();
            auto yaxis = Vec3f::cross(zaxis, xaxis);
            return {
                xaxis.x,  yaxis.x, zaxis.x, (T)0,
                xaxis.y,  yaxis.y, zaxis.y, (T)0,
                xaxis.z,  yaxis.z, zaxis.z, (T)0,
                -Vec3f::dot(xaxis, position), -Vec3f::dot(yaxis, position), -Vec3f::dot(zaxis, position) , (T)1
            };
        }
    }

    static Matrix getTranslation(Vec3f pos){
        if constexpr (S == 4){
            return {
                (T)1.0,  (T)0.0, (T)0.0, (T)0,
                (T)0.0,  (T)1.0, (T)0.0, (T)0,
                (T)0.0,  (T)0.0, (T)1.0, (T)0,
                 -pos.x, -pos.y, -pos.z, (T)1.0
            };
        }
    }
    static Matrix getRotation(Vec3f right, Vec3f up, Vec3f direction){
        if constexpr (S == 4){
            return {
                right.x,  up.x, direction.x, (T)0,
                right.y,  up.y, direction.y, (T)0,
                right.z,  up.z, direction.z, (T)0,
                 (T)0.0, (T)0.0, (T)0.0, (T)1.0
            };
        }
    }
};

template<typename T>
Vec3<T>& operator *=(Vec3<T>& l, const Matrix<T, 3>& r){
    return l = l * r;
}

template<typename T>
Vec3<T> operator *(const Vec3<T>& lhs, const Matrix<T,3>& rhs){
    return{
        lhs.x * rhs.arr[0][0] + lhs.y * rhs.arr[1][0] + lhs.z * rhs.arr[2][0],
        lhs.x * rhs.arr[0][1] + lhs.y * rhs.arr[1][1] + lhs.z * rhs.arr[2][1],
        lhs.x * rhs.arr[0][2] + lhs.y * rhs.arr[1][2] + lhs.z * rhs.arr[2][2]
    };
}

template <typename T>
Vec4<T>& operator *=(const Vec4<T>& lhs, const Matrix<T, 4>& rhs){
    return lhs = lhs * rhs;
}

template <typename T>
Vec4<T> operator *(const Vec4<T>& lhs, const Matrix<T, 4>& rhs){
    return{
        lhs.x * rhs.arr[0][0] + lhs.y * rhs.arr[1][0] + lhs.z * rhs.arr[2][0] + lhs.w * rhs.arr[3][0],
        lhs.x * rhs.arr[0][1] + lhs.y * rhs.arr[1][1] + lhs.z * rhs.arr[2][1] + lhs.w * rhs.arr[3][1],
        lhs.x * rhs.arr[0][2] + lhs.y * rhs.arr[1][2] + lhs.z * rhs.arr[2][2] + lhs.w * rhs.arr[3][2],
        lhs.x * rhs.arr[0][3] + lhs.y * rhs.arr[1][3] + lhs.z * rhs.arr[2][3] + lhs.w * rhs.arr[3][3]
    };
}

using Mat3x3f = Matrix<float, 3>;
using Mat3x3d = Matrix<double, 3>;
using Mat4x4f = Matrix<float, 4>;
using Mat4x4d = Matrix<double, 4>;

#endif // MATRIX_H
