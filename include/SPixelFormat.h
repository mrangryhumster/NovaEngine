#ifndef SPIXELFORMAT_H
#define SPIXELFORMAT_H

#include "NTypes.h"

namespace novaengine
{
namespace renderer
{

enum E_PIXEL_FORMAT
{
    EPF_UNKNOW   = 0x0,
    EPF_R8G8B8   = 0x1,
    EPF_R8G8B8A8 = 0x2,
    EPF_B8G8R8   = 0x3,
    EPF_B8G8R8A8 = 0x4,
    EPF_DEPTH    = 0x5,
};

struct SPixelFormat
{
    SPixelFormat(u32 newPixelFormat = EPF_UNKNOW) :
        RedMask(0),
        RedBits(0),
        RedShift(0),
        GreenMask(0),
        GreenBits(0),
        GreenShift(0),
        BlueMask(0),
        BlueBits(0),
        BlueShift(0),
        AlphaMask(0),
        AlphaBits(0),
        AlphaShift(0),
        BitsPerPixel(0),
        BytesPerPixel(0),
        PixelFormat(0)
    {
        switch(newPixelFormat)
        {
        case EPF_R8G8B8:

            BytesPerPixel = 3;
            BitsPerPixel = 24;

            RedBits = 8;
            RedMask = 0xFF000000;
            RedShift = 0;

            GreenBits = 8;
            GreenMask = 0x00FF0000;
            GreenShift = 1;

            BlueBits = 8;
            BlueMask = 0x0000FF00;
            BlueShift = 2;

            AlphaBits = 0;
            AlphaMask = 0;
            AlphaShift = 0;

            PixelFormat = renderer::EPF_R8G8B8;
            break;

        case EPF_R8G8B8A8:

            BytesPerPixel = 4;
            BitsPerPixel = 32;

            RedBits = 8;
            RedMask = 0xFF000000;
            RedShift = 0;

            GreenBits = 8;
            GreenMask = 0x00FF0000;
            GreenShift = 1;

            BlueBits = 8;
            BlueMask = 0x0000FF00;
            BlueShift = 2;

            AlphaBits = 8;
            AlphaMask = 0x000000FF;
            AlphaShift = 3;

            PixelFormat = renderer::EPF_R8G8B8A8;
            break;

        case EPF_B8G8R8:
            BytesPerPixel = 3;
            BitsPerPixel = 24;

            RedBits = 8;
            RedMask = 0x0000FF00;
            RedShift = 2;

            GreenBits = 8;
            GreenMask = 0x00FF0000;
            GreenShift = 1;

            BlueBits = 8;
            BlueMask = 0xFF000000;
            BlueShift = 0;

            AlphaBits = 0;
            AlphaMask = 0;
            AlphaShift = 0;

            PixelFormat = renderer::EPF_B8G8R8;
            break;

        case EPF_B8G8R8A8:
            BytesPerPixel = 4;
            BitsPerPixel = 32;

            RedBits = 8;
            RedMask = 0x0000FF00;
            RedShift = 2;

            GreenBits = 8;
            GreenMask = 0x00FF0000;
            GreenShift = 1;

            BlueBits = 8;
            BlueMask = 0xFF000000;
            BlueShift = 0;

            AlphaBits = 8;
            AlphaMask = 0x000000FF;
            AlphaShift = 3;

            PixelFormat = renderer::EPF_B8G8R8;
            break;

        case EPF_DEPTH:
            BytesPerPixel = 4;
            BitsPerPixel = 32;

            RedBits = 8;
            RedMask = 0xFF000000;
            RedShift = 0;

            GreenBits = 8;
            GreenMask = 0x00FF0000;
            GreenShift = 1;

            BlueBits = 8;
            BlueMask = 0x0000FF00;
            BlueShift = 2;

            AlphaBits = 8;
            AlphaMask = 0x000000FF;
            AlphaShift = 3;

            PixelFormat = renderer::EPF_DEPTH;
            break;

        default:
            break;
        }
    }

    u32 RedMask;
    u32 RedBits;
    s32 RedShift;

    u32 GreenMask;
    u32 GreenBits;
    s32 GreenShift;

    u32 BlueMask;
    u32 BlueBits;
    s32 BlueShift;

    u32 AlphaMask;
    u32 AlphaBits;
    s32 AlphaShift;

    u32 BitsPerPixel;
    u32 BytesPerPixel;

    u32 PixelFormat;
};

}
}
#endif // SPIXELFORMAT_H
