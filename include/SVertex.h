#ifndef SVERTEX_H
#define SVERTEX_H

#include "NTypes.h"
#include "NExTypes.h"

#define GET_COOKIE ;

namespace novaengine
{
namespace renderer
{

enum E_VERTEX_FORMAT
{
    EVF_DEFAULT = 0,
    EVF_CUSTOM  = 1,
};

enum E_VERTEX_ATTRIB
{
    EVA_POSITION    = 1,
    EVA_NORMAL      = 2,
    EVA_BINORMAL    = 4,
    EVA_TANGENT     = 8,
    EVA_COLOR       = 16,
    EVA_TEXCOORD    = 32,
    EVA_CUSTOM      = 64
};
// TODO (Gosha#1#): check SVertexAttribute field sizes

struct SVertexAttribute
{
public:
    SVertexAttribute()
    {

    }
    SVertexAttribute(u32 attribute_name,u32 attribute_type,u32 attribute_size):
        name(attribute_name),type(attribute_type),size(attribute_size)
    {
    }
    u16 name;
    u16 type;
    u16 size;

    //u16 hint;
};

struct SVertexFormat
{
public:
    SVertexFormat(u32 Flags)
    {
        setFlags(Flags);
    }

    u32  getFlags() const
    {
        return AttributeFlags;
    }

    void setFlags(u32 Flags)
    {
        if(Flags & EVA_POSITION)
            PositionsAttribute = SVertexAttribute(EVA_POSITION,NTYPE_f32,3);
        if(Flags & EVA_NORMAL)
            NormalAttribute    = SVertexAttribute(EVA_NORMAL  ,NTYPE_f32,3);
        if(Flags & EVA_BINORMAL)
            BinormalAttribute  = SVertexAttribute(EVA_BINORMAL,NTYPE_f32,3);
        if(Flags & EVA_TANGENT)
            TangentAttribute   = SVertexAttribute(EVA_TANGENT ,NTYPE_f32,3);
        if(Flags & EVA_COLOR)
            ColorAttribute     = SVertexAttribute(EVA_COLOR   ,NTYPE_u8 ,4);
        if(Flags & EVA_TEXCOORD)
            TexCoordAttribute.push_back(SVertexAttribute(EVA_TEXCOORD,NTYPE_f32,2));

        AttributeFlags = Flags;
    }

    u32 getAttributeFormatCount(u32 attrib) const
    {
        switch(attrib)
        {
        case EVA_POSITION:
            return (AttributeFlags & EVA_POSITION);

        case EVA_NORMAL:
            return (AttributeFlags & EVA_NORMAL);

        case EVA_BINORMAL:
            return (AttributeFlags & EVA_BINORMAL);

        case EVA_TANGENT:
            return (AttributeFlags & EVA_TANGENT);

        case EVA_COLOR:
            return (AttributeFlags & EVA_COLOR);

        case EVA_TEXCOORD:
            return TexCoordAttribute.size();

        case EVA_CUSTOM:
            return CustomAttribute.size();
        }
    }
    const SVertexAttribute* getAttributeFormat(u32 attrib,u32 index = 0) const
    {
        switch(attrib)
        {
        case EVA_POSITION:
            return &PositionsAttribute;

        case EVA_NORMAL:
            return &NormalAttribute;

        case EVA_BINORMAL:
            return &BinormalAttribute;

        case EVA_TANGENT:
            return &TangentAttribute;

        case EVA_COLOR:
            return &ColorAttribute;

        case EVA_TEXCOORD:
            if(index < TexCoordAttribute.size())
                return &TexCoordAttribute[index];
            return nullptr;

        case EVA_CUSTOM:
            if(index < CustomAttribute.size())
                return &CustomAttribute[index];
            return nullptr;
        }
    }

    u32 getVertexFormatType()
    {
        return EVF_DEFAULT;
    }

private:

    u32 AttributeFlags;

    SVertexAttribute PositionsAttribute;

    SVertexAttribute NormalAttribute;
    SVertexAttribute BinormalAttribute;
    SVertexAttribute TangentAttribute;

    SVertexAttribute ColorAttribute;

    std::vector <SVertexAttribute> TexCoordAttribute;

    std::vector <SVertexAttribute> CustomAttribute;
};


struct SVertex
{
    SVertex() {};
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
