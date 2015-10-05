#ifndef CIMAGE_H
#define CIMAGE_H

#include "IImage.h"
namespace novaengine
{
namespace renderer
{

class CImage : public IImage
{
public:
    CImage(renderer::E_PIXEL_FORMAT pf,core::dim2u dim,u8* dat);
    virtual ~CImage();

    core::dim2u getImageDimension();

    core::color4u getPixel(u32 x,u32 y);
    void drawPixel(u32 x,u32 y,core::color4u color);
    void fill(core::color4u color);
    void flip(bool flip_x_axis,bool flip_y_axis);

    SPixelFormat getPixelFormat();
    IImage* getCopy();

    void* lock();//!< Return pointer to pixel array(unsigned char*)
    void  unlock();



protected:
private:

    bool isValid();

    core::dim2u  ImageDimension;
    SPixelFormat PixelFormat;
    u8*          PixelsData;

};

}
}

#endif // CIMAGE_H
