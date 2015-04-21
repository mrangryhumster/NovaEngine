#ifndef SVERTEX_H
#define SVERTEX_H

#include "NTypes.h"
#include "NExTypes.h"

namespace novaengine
{
namespace renderer
{

struct SVertex
{
    SVertex(){};
    SVertex(core::vector3f VertexPosition,core::vector2f VertexTexCoord,core::vector3f VertexNormal,core::color4u Color):
        Position(VertexPosition),TexCoord(VertexTexCoord),Normal(VertexNormal),Color(Color)
        {};
    SVertex(f32 position_x,f32 position_y,f32 position_z,f32 texture_x,f32 texture_y,f32 normal_x,f32 normal_y,f32 normal_z,u8 color_r,u8 color_g,u8 color_b,u8 color_a):
        Position(position_x,position_y,position_z),TexCoord(texture_x,texture_y),Normal(normal_x,normal_y,normal_z),Color(color_r,color_g,color_b,color_a)
        {};

    core::vector3f      Position;
    core::vector2f      TexCoord;
    core::vector3f      Normal;
    core::color4u       Color;
};

struct SVertexExtended
{
    core::vector3f      Position;
    core::vector2f      TexCoord;
    core::vector3f      Normal;
    core::color4u       Color;
    core::vector3f      Binormal;
    core::vector3f      Tangent;
};

}
}
#endif // SVERTEX_H
