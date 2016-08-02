
#pragma once

#include "ne_types.h"

namespace novaengine
{
namespace core
{

template <class T>
class color3
{
public:
    color3()
    {
    }

    color3(T r_,T g_,T b_)
    {
        r = r_;
        g = g_;
        b = b_;
    }


    color3 operator= (const color3 &other)
    {
        r = other.r;
        g = other.g;
        b = other.b;
        return *this;
    }


    T r;
    T g;
    T b;
};


}
}
