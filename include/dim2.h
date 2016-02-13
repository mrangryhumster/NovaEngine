#ifndef DIMENSION2D_T_H
#define DIMENSION2D_T_H

#include "NMath.h"

namespace novaengine
{
namespace core
{
template< class T >
class dim2
{
public:
    dim2()
    {
    }
    dim2(T width_,T height_):
        width(width_),
        height(height_)
    {
    }

    dim2<T> operator*(const dim2<T> &other)
    {
        dim2<T> result;
        result.width = width * other.width;
        result.height = height * other.height;
        return result;
    }

    dim2<T> operator*=(const dim2<T> &other)
    {
        dim2<T> result;
        width *= other.width;
        height *= other.height;
		return this;
    }

    bool operator == (const dim2<T> &other ) const
    {
        return width == other.width && height == other.height ;
    }

    bool operator != (const dim2<T> &other ) const
    {
        return width != other.width && height != other.height ;
    }

    T area()
    {
        return (width * height);
    }

    T width;
    T height;
};


}

}


#endif // dim2_H
