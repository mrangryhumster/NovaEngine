#ifndef COLOR3_H
#define COLOR3_H

#include "../NTypes.h"

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

    color3(T Red_,T Green_,T Blue_)
    {
        Red = Red_;
        Green = Green_;
        Blue = Blue_;
    }


    color3 operator= (const color3 &other)
    {
        Red = other.Red;
        Green = other.Green;
        Blue = other.Blue;
        return *this;
    }

    color3 invert()
    {
        Red   = 255 - Red;
        Green = 255 - Green;
        Blue  = 255 - Blue;
    }

    T Red;
    T Green;
    T Blue;
};


}
}

#endif // COLOR3_H
