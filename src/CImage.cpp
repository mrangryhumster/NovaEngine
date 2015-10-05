#include "CImage.h"


namespace novaengine
{
namespace renderer
{

CImage::CImage(E_PIXEL_FORMAT pf,core::dim2u dim,u8* data)
{
    setObjectName("image");

    PixelFormat = SPixelFormat(pf);
    ImageDimension = dim;

    u32 DataSize = ImageDimension.width * ImageDimension.height * PixelFormat.BytesPerPixel;

    PixelsData = new u8[DataSize];
    if(data)
        memcpy(PixelsData,data,DataSize);
}

CImage::~CImage()
{
    if(PixelsData)
        delete[] PixelsData;
}

core::color4u CImage::getPixel(u32 x,u32 y)
{
    if(!isValid())
        return core::color4u(0,0,0,0);

    if(x < ImageDimension.width && y < ImageDimension.height)
    {
        core::color4u color;
        unsigned int Index = (ImageDimension.width * y + x)*PixelFormat.BytesPerPixel;
        color.r = PixelsData[Index+PixelFormat.RedShift];
        color.g = PixelsData[Index+PixelFormat.GreenShift];
        color.b = PixelsData[Index+PixelFormat.BlueShift];
        if(PixelFormat.AlphaBits != 0)
            color.a = PixelsData[Index+PixelFormat.AlphaShift];
        return color;
    }
    return core::color4u(0,0,0,0);
}

void CImage::drawPixel(u32 x,u32 y,core::color4u color)
{
    if(!isValid())
        return;

    if(x < ImageDimension.width && y < ImageDimension.height)
    {
        unsigned int Index = (ImageDimension.width * y + x)*PixelFormat.BytesPerPixel;
        PixelsData[Index + PixelFormat.RedShift] = color.r;
        PixelsData[Index + PixelFormat.GreenShift] = color.g;
        PixelsData[Index + PixelFormat.BlueShift] = color.b;
        if(PixelFormat.AlphaBits != 0)
            PixelsData[Index + PixelFormat.AlphaShift] = color.a;
    }
}

void CImage::fill(core::color4u color)
{
    if(!isValid())
        return;

        u32 ImgSize =  ImageDimension.width * ImageDimension.height * PixelFormat.BytesPerPixel;

        for(u32 index = 0; index < ImgSize; index+=PixelFormat.BytesPerPixel)
        {
            PixelsData[index + PixelFormat.RedShift] = color.r;
            PixelsData[index + PixelFormat.GreenShift] = color.g;
            PixelsData[index + PixelFormat.BlueShift] = color.b;
            if(PixelFormat.AlphaBits != 0)
                PixelsData[index + PixelFormat.AlphaShift] = color.a;
        }

}

void CImage::flip(bool flip_x_axis,bool flip_y_axis)
{
    flip_x_axis =flip_x_axis&flip_y_axis; //do nothing
}

core::dim2u CImage::getImageDimension()
{
    return ImageDimension;
}

SPixelFormat CImage::getPixelFormat()
{
    return PixelFormat;
}

IImage* CImage::getCopy()
{
    return new CImage((E_PIXEL_FORMAT)PixelFormat.PixelFormat,ImageDimension,PixelsData);
}

void* CImage::lock()
{
    return PixelsData;
}

void  CImage::unlock()
{
    /*
//    //\\//\\//\\//\\
//    || ||  ||  || ||
//    \\_/ \/||\/ \_//
//     \____/  \____/
//          \__/
//
*/
}

bool CImage::isValid()
{
    return !(ImageDimension.width == 0 || ImageDimension.height == 0 || (!PixelsData));
}

}
}
