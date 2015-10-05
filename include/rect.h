#ifndef RECT_H
#define RECT_H

#include "NTypes.h"
#include <cmath>

namespace novaengine
{
namespace core
{

template <class T>
class rect
{
public:
    rect<T>()
    {
    };
    rect<T>(T x1,T y1,T x2,T y2)
    {
        X1 = x1;
        Y1 = y1;
        X2 = x2;
        Y2 = y2;
    };

    bool operator == (const rect<T> &other ) const
    {
        return X1 == other.X1 && X2 == other.X2 && Y1 == other.Y1 && Y2 == other.Y2;
    }

    bool operator != (const rect<T> &other ) const
    {
        return X1 != other.X1 || X2 != other.X2 || Y1 != other.Y1 || Y2 != other.Y2;
    }

    bool point_in_rect(T point_x,T point_y)
    {
        return (X1 < point_x && X2 > point_x && Y1 < point_y && Y2 > point_y);
    }

    T width()
    {
        return (T)std::abs((double)(X2 - X1));
    }

    T height()
    {
        return (T)std::abs((double)Y2 - Y1);
    }

    T area()
    {
        return (T)std::abs((double)(X2 - X1) * (Y2 - Y1));
    }

    dim2<T> dimension()
    {
        return dim2<T>(width(),height());
    }

    T X1;
    T Y1;
    T X2;
    T Y2;
};



}
}

#endif // RECT_H
