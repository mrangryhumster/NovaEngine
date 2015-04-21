#ifndef IIMAGE_H
#define IIMAGE_H

#include "NExTypes.h"
#include "Resource.h"
#include "SPixelFormat.h"

namespace novaengine
{
namespace renderer
{

class IImage : public Resource
{
public:

    virtual core::dim2u getImageDimension() = 0;

    virtual core::color4u getPixel(u32 x,u32 y) = 0;
    virtual void drawPixel(u32 x,u32 y,core::color4u color) = 0;
    virtual void fill(core::color4u color) = 0;
    virtual void flip(bool flip_x_axis,bool flip_y_axis) = 0;

    virtual SPixelFormat getPixelFormat() = 0;
    virtual IImage* getCopy() = 0;

    virtual void* lock()   = 0;//!< Return pointer to pixel array(unsigned char*)
    virtual void  unlock() = 0;

protected:
private:
};

}
}

#endif // IIMAGE_H
