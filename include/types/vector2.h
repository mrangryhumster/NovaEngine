#ifndef VECTOR2_H
#define VECTOR2_H

#include "../NTypes.h"
#include <math.h>

namespace novaengine
{
namespace core
{

template <class T>
class vector2
{
public:
    vector2<T>()
    {};

    vector2<T>(T val_):
        x(val_),y(val_)
    {};

    vector2<T>(T x_,T y_):
        x(x_),y(y_)
    {};


    vector2<T> set(const T _val)
    {
        x = _val;
        y = _val;
        return *this;
    }

    vector2<T> set(const T _x,const T _y)
    {
        x = _x;
        y = _y;
        return *this;
    }

    vector2<T> set(const vector2<T>& other)
    {
        x = other.x;
        y = other.y;
        return *this;
    }

    vector2<T> add(const T _val)
    {
        x += _val;
        y += _val;
        return *this;
    }

    vector2<T> add(const T _x,const T _y)
    {
        x += _x;
        y += _y;
        return *this;
    }

    vector2<T> add(const vector2<T>& other)
    {
        x += other.x;
        y += other.y;
        return *this;
    }

    vector2<T> mul(const T _val)
    {
        x *= _val;
        y *= _val;
        return *this;
    }

    vector2<T> mul(const T _x,const T _y)
    {
        x *= _x;
        y *= _y;
        return *this;
    }

    vector2<T> mul(const vector2<T>& other)
    {
        x *= other.x;
        y *= other.y;
        return *this;
    }

    vector2<T> div(const T _val)
    {
        x /= _val;
        y /= _val;
        return *this;
    }

    vector2<T> div(const T _x,const T _y)
    {
        x /= _x;
        y /= _y;
        return *this;
    }

    vector2<T> div(const vector2<T>& other)
    {
        x /= other.x;
        y /= other.y;
        return *this;
    }


    vector2<T> operator= (const vector2 &other)
    {
        x = other.x;
        y = other.y;
        return *this;
    }

    inline vector2<T> operator= (const T other)
    {
        x = x;
        y = y;
        return *this;
    }

    vector2<T> operator - () const
    {
        return vector2(-x,-y);
    }

    vector2<T> operator += (const vector2 &other)
    {
        x += other.x;
        y += other.y;

        return *this;
    }

    vector2<T> operator -= (const vector2 &other)
    {
        x -= other.x;
        y -= other.y;

        return *this;
    }

    vector2<T> operator += (const T v)
    {
        x += v;
        y += v;

        return *this;
    }

    vector2<T> operator -= (const T v)
    {
        x -= v;
        y -= v;

        return *this;
    }

    vector2<T> operator *= (const vector2 &other)
    {
        x *= other.x;
        y *= other.y;

        return *this;
    }

    vector2<T> operator *= (const T v)
    {
        x *= v;
        y *= v;

        return *this;
    }

    vector2<T> operator /= (const vector2 &other)
    {
        x /= other.x;
        y /= other.y;

        return *this;
    }

    vector2<T> operator /= (const T v)
    {
        x /= v;
        y /= v;

        return *this;
    }

    vector2<T> operator * (const vector2 &other)
    {
        T tx = x* other.x;
        T ty = y* other.y;
        return vector2<T>(tx,ty);
    }

    vector2<T> operator * (const T value)
    {
        T tx = x* value;
        T ty = y* value;
        return vector2<T>(tx,ty);
    }

    vector2<T> operator + (const vector2 &other)
    {
        T tx = x + other.x;
        T ty = y + other.y;

        return vector2<T>(tx,ty);
    }

    vector2<T> operator - (const vector2 &other)
    {
        T tx = x - other.x;
        T ty = y - other.y;

        return vector2<T>(tx,ty);
    }


    vector2<T> operator + (const T v)
    {
        x += v;
        y += v;

        return *this;
    }

    vector2<T> operator - (const T v)
    {
        x -= v;
        y -= v;

        return *this;
    }

    bool operator == (const vector2 &other ) const
    {

        return x == other.x && y == other.y;
    }

    bool operator !=  (const vector2 &other ) const
    {

        return x != other.x || y != other.y;
    }

    T length () const
    {
        return (T)sqrt( x * x + y * y);
    }

    T lengthSq() const
    {
        return x * x + y * y;
    }

    vector2<T> normalize ()
    {
        return (*this) /= length();
    }

    T distanceTo (const vector2 &other)
    {
        return (T) sqrt(sqr(x - other.x) + sqr(y - other.y));
    }

    T distanceToSq (const vector2 &other)
    {
        return sqr(x - other.x) + sqr(y - other.y);
    }

    T sqr(T num)
    {
        return num*num;
    }

    T x;
    T y;
};

}

}




#endif // VECTOR2_H
