#ifndef NEXTYPES_H
#define NEXTYPES_H

#include "NTypes.h"

#include "types/matrix.h"
#include "types/vector2.h"
#include "types/vector3.h"
#include "types/quaternion.h"
#include "types/dim2.h"
#include "types/rect.h"
#include "types/color3.h"
#include "types/color4.h"

#include <string>
#include <vector>

namespace novaengine
{
    namespace core
    {
        typedef matrix<f32>  matrixf;
        typedef matrix<f64>  matrixd;
        typedef matrix<f96>  matrixld;

        typedef vector2<s32> vector2s;
        typedef vector2<u32> vector2u;

        typedef vector2<f32> vector2f;
        typedef vector2<f64> vector2d;
        typedef vector2<f96> vector2ld;


        typedef vector3<s32> vector3s;
        typedef vector3<f32> vector3f;
        typedef vector3<f64> vector3d;
        typedef vector3<f96> vector3ld;

        typedef vector2f vec2;
        typedef vector3f vec3;

        typedef quaternion<f32> quaternionf;
        typedef quaternion<f64> quaterniond;
        typedef quaternion<f96> quaternionld;

        typedef dim2<s32> dim2s;
        typedef dim2<u32> dim2u;
        typedef dim2<f32> dim2f;

        typedef rect<s32> rects;
        typedef rect<u32> rectu;
        typedef rect<f32> rectf;

        typedef color3<u8> color3u;
        typedef color4<u8> color4u;

        typedef color3<f32> color3f;
        typedef color4<f32> color4f;

        //  in future i replace this on my own string class
        typedef  std::basic_string<char>     string;
        typedef  std::basic_string<wchar_t>  wstring;



    }
}

#endif // NEXTYPES_H
