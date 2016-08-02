#ifndef STextureParameters_H
#define STextureParameters_H

#include "ne_types.h"

namespace novaengine
{
namespace renderer
{

enum E_TEXTURE_PARAMETERS
{
    ETP_MAG_FILTER          = 0x01,
    ETP_MIN_FILTER          = 0x02,
    ETP_WRAP_X              = 0x03,
    ETP_WRAP_Y              = 0x04,
    ETP_GENERATE_MIPMAPS    = 0x05
};

enum ETP_FILTER
{
    ETPF_UNDEFINED  = 0x0,
    ETPF_NEAREST    = 0x1,
    ETPF_LINEAR     = 0x2,

	ETPF_LINEAR_MIPMAP_LINEAR 	    = 0x4,
	ETPF_LINEAR_MIPMAP_NEAREST 	    = 0x5,
	ETPF_NEAREST_MIPMAP_LINEAR 	    = 0x6,
	ETPF_NEAREST_MIPMAP_NEAREST 	= 0x7,
};

enum ETP_WRAP
{
    ETPW_REPEAT     = 0x1,
    ETPW_NOT_REPEAT = 0x2,
    //---
    // ETPW_CLAMP
    // ETPW_CLAMP_TO_EDGE

};

struct STextureParameters
{
    STextureParameters():
    Texture_mag_filter(ETPF_LINEAR),
    Texture_min_filter(ETPF_LINEAR),
    Texture_wrap_x(ETPW_REPEAT),
    Texture_wrap_y(ETPW_REPEAT),
    Generate_mipmaps(false)
    {
    }

    u8 Texture_mag_filter;
    u8 Texture_min_filter;
    u8 Texture_wrap_x;
    u8 Texture_wrap_y;

    u8 Generate_mipmaps;

};

}
}

#endif // STextureParameters_H
