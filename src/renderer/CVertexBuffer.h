#ifndef CVertexBuffer_H
#define CVertexBuffer_H

#include <vector>
#include "IVertexBuffer.h"

namespace novaengine
{
namespace renderer
{


class CVertexBuffer : public IVertexBuffer
{

public:
    CVertexBuffer()
    {
        VertexFormat  = EVF_VERTEX;
        PrimitiveType = EPT_TRIANGLE;
        BufferType     = EVBT_UNDEFINDED;
        UpdateRequired = true;
    }
    u32 getVertexCount()
    {
        return Positions.size();
    }
//---------------------------------------------------------------------------
    void addPosition(core::vector3f vertex_position)
    {
        Positions.push_back(vertex_position);
    }
    void addPositions(core::vector3f* vertex_positions,u32 vertex_positions_count)
    {
        Positions.insert(Positions.end(),vertex_positions,vertex_positions+vertex_positions_count);
    }
    void setPositions(core::vector3f* vertex_positions,u32 vertex_positions_count)
    {
        Positions.clear();
        if(vertex_positions_count > 0)
            Positions.insert(Positions.begin(),vertex_positions,vertex_positions+vertex_positions_count);
    }
    u32  getPositionsCount()
    {
        return Positions.size();
    }
    core::vector3f* getPositions()
    {
        return &Positions.front();
    }
    std::vector<core::vector3f>* getPositionsArray()
    {
        return &Positions;
    }
//---------------------------------------------------------------------------
    void addUV(core::vector2f vertex_uv)
    {
        UVs.push_back(vertex_uv);
    }
    void addUVs(core::vector2f* vertex_uvs,u32 vertex_uvs_count)
    {
        UVs.insert(UVs.end(),vertex_uvs,vertex_uvs+vertex_uvs_count);
    }
    void setUVs(core::vector2f* vertex_uvs,u32 vertex_uvs_count)
    {
        UVs.clear();
        if(vertex_uvs_count > 0)
            UVs.insert(UVs.begin(),vertex_uvs,vertex_uvs+vertex_uvs_count);
    }
    u32  getUVsCount()
    {
        return UVs.size();
    }
    core::vector2f* getUVs()
    {
        return &UVs.front();
    }
    std::vector<core::vector2f>* getUVsArray()
    {
        return &UVs;
    }
//---------------------------------------------------------------------------
    void addNormal(core::vector3f vertex_normal)
    {
        Normals.push_back(vertex_normal);
    }
    void addNormals(core::vector3f* vertex_normals,u32 vertex_normals_count)
    {
        Normals.insert(Normals.end(),vertex_normals,vertex_normals+vertex_normals_count);
    }
    void setNormals(core::vector3f* vertex_normals,u32 vertex_normals_count)
    {
        Normals.clear();
        if(vertex_normals_count > 0)
            Normals.insert(Normals.begin(),vertex_normals,vertex_normals+vertex_normals_count);
    }
    u32  getNormalsCount()
    {
        return Normals.size();
    }
    core::vector3f* getNormals()
    {
        return &Normals.front();
    }
    std::vector<core::vector3f>* getNormalsArray()
    {
        return &Normals;
    }
//---------------------------------------------------------------------------
    void addColor(core::color4u vertex_color)
    {
        Colors.push_back(vertex_color);
    }
    void addColors(core::color4u* vertex_colors,u32 vertex_colors_count)
    {
        Colors.insert(Colors.end(),vertex_colors,vertex_colors+vertex_colors_count);
    }
    void setColors(core::color4u* vertex_colors,u32 vertex_colors_count)
    {
        Colors.begin();
        if(vertex_colors_count > 0)
            Colors.insert(Colors.begin(),vertex_colors,vertex_colors+vertex_colors_count);
    }
    u32  getColorsCount()
    {
        return Colors.size();
    }
    core::color4u* getColors()
    {
        return &Colors.front();
    }
    std::vector<core::color4u>* getColorsArray()
    {
        return &Colors;
    }
//---------------------------------------------------------------------------
    void addIndex(u16 index)
    {
        Indices.push_back(index);
    }
    void addIndices(u16* indices,u16 indices_count)
    {
        Indices.insert(Indices.end(),indices,indices+indices_count);
    }
    void setIndices(u16* indices,u16 indices_count)
    {
        Indices.clear();
        if(indices_count > 0)
            Indices.insert(Indices.begin(),indices,indices+indices_count);
    }
    u16  getIndicesCount()
    {
        return Indices.size();
    }
    u16* getIndices()
    {
        return &Indices.front();
    }
    std::vector<u16>* getIndicesArray()
    {
        return &Indices;
    }
//---------------------------------------------------------------------------
    u32 getVertexFormat()
    {
        return VertexFormat;
    }
    void setVertexFormat(u32 vt)
    {
        VertexFormat = vt;
    }

    u32 getPrimitiveType()
    {
        return PrimitiveType;
    }
    void setPrimitiveType(E_PRIMITIVE_TYPE pt)
    {
        PrimitiveType = pt;
    }

    virtual void setBufferType(E_VERTEX_BUFFER_TYPE type)
    {
        BufferType = type;
    }
    virtual u32  getBufferType()
    {
        return BufferType;
    }

    void setUpdateRequired(bool up)
    {
        UpdateRequired = up;
    }
    bool isUpdateRequired()
    {
        return UpdateRequired;
    }
    bool Update()
    {
        UpdateRequired = false;
        return true;
    }

    void lock()     {}
    void unlock()   {}
    void render()  {}

    bool isValid()
    {
        if(VertexFormat & EVF_VERTEX && BufferType != EVBT_UNDEFINDED)
        {
            if(VertexFormat & EVF_TEXCOORD      && Positions.size() != UVs.size())
                return false;
            if(VertexFormat & EVF_NORMAL        && Positions.size() != Normals.size())
                return false;
            if(VertexFormat & EVF_VERTEX_COLOR  && Positions.size() != Colors.size())
                return false;

            return true;
        }
        return false;
    }

protected:

    std::vector<core::vector3f>    Positions;
    std::vector<core::vector2f>    UVs;
    std::vector<core::vector3f>    Normals;
    std::vector<core::color4u>     Colors;
    std::vector<u16>               Indices;

    u32                     VertexFormat;
    u32                     PrimitiveType;

    u32                     BufferType;
    bool                    UpdateRequired;
};

}
}
#endif // CVertexBuffer_H
