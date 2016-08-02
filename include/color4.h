
#pragma once


namespace novaengine
{
namespace core
{

template <class T>
class color4
{
public:
    color4()
    {
    }
    color4(T Red_,T Green_,T Blue_,T Alpha_)
    {
        r = Red_;
        g = Green_;
        b = Blue_;
        a = Alpha_;
    }


    color4 operator= (const color4 &other)
    {
        r = other.r;
        g = other.g;
        b = other.b;
        a = other.a;
        return *this;
    }

    inline color4<T>& set(const T r_,const T g_,const T b_,const T a_)
    {
        r = r_;
        g = g_;
        b = b_;
        a = a_;
        return *this;
    }

    inline color4<T>& set(const color4<T>& other)
    {
        r = other.r;
        g = other.g;
        b = other.b;
        a = other.a;
        return *this;
    }

    inline color4<T>& mul(const f32 val)
    {
        r    *= val;
        g    *= val;
        b    *= val;
        a    *= val;
        return *this;
    }


    T r;
    T g;
    T b;
    T a;

};


}
}

