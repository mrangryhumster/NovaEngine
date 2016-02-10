#ifndef VECTOR3_H
#define VECTOR3_H

#include "NTypes.h"
#include "NMath.h"

namespace novaengine
{
namespace core
{

template <class T>
class vector3
{
public:
    vector3<T>()
    {};

    vector3<T>(T v):
        x(v),y(v),z(v)
    {};

    vector3<T>(T x_,T y_,T z_):
        x(x_),y(y_),z(z_)
    {};


    inline vector3<T>& set(const T _val)
    {
        x = _val;
        y = _val;
        z = _val;
        return *this;
    }


    inline vector3<T>& set(const T _x,const T _y,const T _z)
    {
        x = _x;
        y = _y;
        z = _z;
        return *this;
    }

    inline vector3<T>& set(const vector3<T>& other)
    {
        x = other.x;
        y = other.y;
        z = other.z;
        return *this;
    }

    inline vector3<T>& add(const T _val)
    {
        x+=_val;
        y+=_val;
        z+=_val;
        return *this;
    }

    inline vector3<T>& add(const T _x,const T _y,const T _z)
    {
        x += _x;
        y += _y;
        z += _z;
        return *this;
    }

    inline vector3<T>& add(const vector3<T>& other)
    {
        x += other.x;
        y += other.y;
        z += other.z;
        return *this;
    }

    inline vector3<T>& mul(const T _val)
    {
        x *= _val;
        y *= _val;
        z *= _val;
        return *this;
    }

    inline vector3<T>& mul(const T _x,const T _y,const T _z)
    {
        x *= _x;
        y *= _y;
        z *= _z;
        return *this;
    }

    inline vector3<T>& mul(const vector3<T>& other)
    {
        x *= other.x;
        y *= other.y;
        z *= other.z;
        return *this;
    }

    inline vector3<T>& div(const T _val)
    {
        x /= _val;
        y /= _val;
        z /= _val;
        return *this;
    }

    inline vector3<T>& div(const T _x,const T _y,const T _z)
    {
        x /= _x;
        y /= _y;
        z /= _z;
        return *this;
    }

    inline vector3<T>& div(const vector3<T>& other)
    {
        x /= other.x;
        y /= other.y;
        z /= other.z;
        return *this;
    }

    inline bool cmp(const T _x,const T _y,const T _z) const
    {
        return (x == _x && y == _y && z == _z);
    }

    inline bool cmp(const vector3<T>& other) const
    {
        return (x == other.x && y == other.y && z == other.z);
    }

    inline vector3<T> operator= (const vector3 &other)
    {
        x = other.x;
        y = other.y;
        z = other.z;
        return *this;
    }

    inline vector3<T> operator= (const T other)
    {
        x = other;
        y = other;
        z = other;
        return *this;
    }

    inline vector3<T> operator - () const
    {
        return vector3<T>(-x,-y,-z);
    }

    inline vector3<T> operator += (const vector3 &other)
    {
        x += other.x;
        y += other.y;
        z += other.z;
        return *this;
    }

    inline vector3<T> operator += (const T &value)
    {
        x += value;
        y += value;
        z += value;
        return *this;
    }

    inline vector3<T> operator + (const vector3 &other)
    {
        return vector3<T>(x+ other.x,y+ other.y,z+ other.z);
    }

    inline vector3<T> operator + (const T &value)
    {
        T tx = x+ value;
        T ty = y+ value;
        T tz = z+ value;
        return vector3<T>(tx,ty,tz);
    }

    inline vector3<T> operator -= (const vector3 &other)
    {
        x -= other.x;
        y -= other.y;
        z -= other.z;
        return *this;
    }

    inline vector3<T> operator -= (const T value)
    {
        x -= value;
        y -= value;
        z -= value;
        return *this;
    }

    inline vector3<T> operator - (const vector3 &other)
    {
        T tx = x- other.x;
        T ty = y- other.y;
        T tz = z- other.z;
        return vector3<T>(tx,ty,tz);
    }

    inline vector3<T> operator - (const T value)
    {
        T tx = x- value;
        T ty = y- value;
        T tz = z- value;
        return vector3<T>(tx,ty,tz);
    }

    inline vector3<T> operator *= (const vector3 &other)
    {
        x *= other.x;
        y *= other.y;
        z *= other.z;
        return *this;
    }

    inline vector3<T> operator *= (const T value)
    {
        x *= value;
        y *= value;
        z *= value;
        return *this;
    }

    inline vector3<T> operator * (const vector3 &other)
    {
        T tx = x* other.x;
        T ty = y* other.y;
        T tz = z* other.z;
        return vector3<T>(tx,ty,tz);
    }

    inline vector3<T> operator * (const T value)
    {
        T tx = x* value;
        T ty = y* value;
        T tz = z* value;
        return vector3<T>(tx,ty,tz);
    }

    inline vector3<T> operator /= (const vector3 &other)
    {
        x /= other.x;
        y /= other.y;
        z /= other.z;
        return *this;
    }

    inline vector3<T> operator /= (const T value)
    {
        x /= value;
        y /= value;
        z /= value;
        return *this;
    }

    inline vector3<T> operator / (const vector3 &other)
    {
        T tx = x/ other.x;
        T ty = y/ other.y;
        T tz = z/ other.z;
        return vector3<T>(tx,ty,tz);
    }

    inline vector3<T> operator / (const T value)
    {
        T tx = x/ value;
        T ty = y/ value;
        T tz = z/ value;
        return vector3<T>(tx,ty,tz);
    }

    inline bool operator == (const vector3 &other ) const
    {
        return x == other.x && y == other.y && z == other.z;
    }

    inline bool operator !=  (const vector3 &other ) const
    {
        return x != other.x || y != other.y || z != other.z;
    }
    //------------------------------------------------------
    inline T length () const
    {
        return (T) sqrt(x*x + y*y + z*z);
    }

    inline T length_sqr() const
    {
        return x * x + y * y + z * z;
    }

    inline vector3<T> normalize ()
    {
        return (*this) /= length();
    }

    inline T dot_product(const vector3<T>& other) const
    {
        return x*other.x + y*other.y + z*other.z;
    }

    inline vector3<T> cross_product(const vector3<T>& vec) const
    {
        return vector3<T>(y * vec.z - z * vec.y, z * vec.x - x * vec.z, x * vec.y - y * vec.x);
    }


    inline f64 distance_to (const vector3 &other)
    {
        return (T) sqrt(sqr(x - other.x) + sqr(y - other.y) + sqr(z - other.z));
    }

    inline f64 distance_to_sqr (const vector3 &other)
    {
        return sqr(x - other.x) + sqr(y - other.y) + sqr(z - other.z);
    }

    inline vector3<T> direction_to(const vector3 &other)
    {
        return vector3<T>(other.x - x,other.y - y,other.z - z);
    }

    inline vector3<T> norm_direction_to(const vector3 &other)
    {
        vector3<T> tmp_vec = other - *this;
        tmp_vec.normalize();
        return tmp_vec;
    }
    //test
    void _EXT_decard_to_sphere_coords()
    {
        T tmp_x = 0,tmp_y = 0,tmp_z = 0;

        tmp_x = length();
        tmp_y = acos(z/tmp_x);
        tmp_z = math::PI - atan(y/z);

        set(tmp_x,tmp_y,tmp_z);
    }

    void _EXT_sphere_to_decard_coords()
    {
        T tmp_x = 0,tmp_y = 0,tmp_z = 0;

        tmp_x = x * sin(y) * cos(z);
        tmp_y = x * sin(y) * sin(z);
        tmp_z = x * cos(y);

        set(tmp_x,tmp_y,tmp_z);
    }

    void _EXT_normalize_rotation_rad()
    {
        while(x >= math::PI || x <= -math::PI)
            if(x >= math::PI)
                x -= 2*math::PI;
            else if(x <= -math::PI)
                x += 2*math::PI;

        while(y >= math::PI || y <= -math::PI)
            if(y >= math::PI)
                y -= 2*math::PI;
            else if(y <= -math::PI)
                y += 2*math::PI;

        while(z >= math::PI || z <= -math::PI)
            if(z >= math::PI)
                z -= 2*math::PI;
            else if(z <= -math::PI)
                z += 2*math::PI;
    }

    void _EXT_normalize_rotation_deg()
    {
        while(x >= 180.f || x <= -180.f)
            if(x >= 180.f)
                x -= 2*180.f;
            else if(x <= -180.f)
                x += 2*180.f;

        while(y >= 180.f || y <= -180.f)
            if(y >= 180.f)
                y -= 2*180.f;
            else if(y <= -180.f)
                y += 2*180.f;

        while(z >= 180.f || z <= -180.f)
            if(z >= 180.f)
                z -= 2*180.f;
            else if(z <= -180.f)
                z += 2*180.f;
    }
    //----

    T x;
    T y;
    T z;
private:
    T sqr(T num)
    {
        return num*num;
    }
};


}
}
#endif // VECTOR3_H
