
#pragma once

#include <cstddef>
#include <cstdint>

//--------------------------
//fixed size types
typedef std::int8_t     s8;
typedef std::uint8_t	u8;

typedef std::int16_t    s16;
typedef std::uint16_t   u16;

typedef std::int32_t    s32;
typedef std::uint32_t   u32;

typedef std::int64_t    s64;
typedef std::uint64_t   u64;
//--------------------------
//floating point types
typedef float           f32;
typedef double          f64;
typedef long double     f96;
//--------------------------


namespace novaengine
{
    namespace core
    {
        template<typename T>class matrix;
        typedef matrix<f32>  matrixf;
        typedef matrix<f64>  matrixd;
        typedef matrix<f96>  matrixld;

        template<typename T>class vector2;
        typedef vector2<s32> vector2s;
        typedef vector2<u32> vector2u;
        typedef vector2<f32> vector2f;
        typedef vector2<f64> vector2d;
        typedef vector2<f96> vector2ld;

        template<typename T>class vector3;
        typedef vector3<s32> vector3s;
        typedef vector3<u32> vector3u;
        typedef vector3<f32> vector3f;
        typedef vector3<f64> vector3d;
        typedef vector3<f96> vector3ld;

#ifdef NE_POSITION_PRECISION_64
        typedef vector2<f64> vector2p;
        typedef vector3<f64> vector3p;
#else
        typedef vector2<f64> vector2p;
        typedef vector3<f64> vector3p;
#endif // NE_POSITION_PRECISION_64

        template<typename T>class quaternion;
        typedef quaternion<f32> quaternionf;
        typedef quaternion<f64> quaterniond;
        typedef quaternion<f96> quaternionld;

        template<typename T>class dim2;
        typedef dim2<s32> dim2s;
        typedef dim2<u32> dim2u;
        typedef dim2<f32> dim2f;

        template<typename T>class rect;
        typedef rect<s32> rects;
        typedef rect<u32> rectu;
        typedef rect<f32> rectf;

        template<typename T>class color3;
        template<typename T>class color4;

        typedef color3<f32> color3u;
        typedef color4<f32> color4u;

        typedef color3<f32> color3f;
        typedef color4<f32> color4f;

        //  in future i replace this on my own string class
//        typedef  std::basic_string<char>     string;
//        typedef  std::basic_string<wchar_t>  wstring;



    }
}

#include "matrix.h"
#include "vector2.h"
#include "vector3.h"
#include "quaternion.h"
#include "dim2.h"
#include "rect.h"
#include "color3.h"
#include "color4.h"

#include <string>
#include <vector>


//additional shit
enum NE_TYPE
{
    NTYPE_s8 = 0,
    NTYPE_u8,
    NTYPE_s16,
    NTYPE_u16,
    NTYPE_s32,
    NTYPE_u32,
    NTYPE_s64,
    NTYPE_u64,

    NTYPE_f32,
    NTYPE_f64,
};

inline size_t ne_sizeof(NE_TYPE type)
{
    switch (type)
    {
    case NTYPE_s8:
    case NTYPE_u8:
        return 1;
    case NTYPE_s16:
    case NTYPE_u16:
        return 2;
    case NTYPE_s32:
    case NTYPE_u32:
        return 4;
    case NTYPE_s64:
    case NTYPE_u64:
        return 8;

    case NTYPE_f32:
        return 4;
        break;
    case NTYPE_f64:
        return 8;
        break;
    }
    return 1;
}

