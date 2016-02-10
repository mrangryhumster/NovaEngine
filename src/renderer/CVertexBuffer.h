#ifndef CVertexBuffer_H
#define CVertexBuffer_H

#include <stdio.h>
#include <vector>
#include "IVertexBuffer.h"

namespace novaengine
{
namespace renderer
{
class CVertexBuffer : public IVertexBuffer
{
public:
    CVertexBuffer():
        VertexFormat(EVA_POSITION),
        PrimitiveType(EPT_TRIANGLE),
        BufferType(EVBT_RAWDATA),
        UpdateRequired(true)
    {
    }
    //---------------------------------------------------------------------------
    void  setBufferData(u32 buffer,const void* data,u32 size)
    {
        std::vector<u8>* BufferData = getBuffer(buffer);
        if(BufferData == nullptr) return;

        BufferData->clear();
        if(data && size)
            BufferData->insert(BufferData->end(),(u8*)data,((u8*)data+size));
    }
    void  addBufferData(u32 buffer,const void* data,u32 size)
    {
        std::vector<u8>* BufferData = getBuffer(buffer);
        if(BufferData == nullptr) return;

        if(data && size)
            BufferData->insert(BufferData->end(),(u8*)data,((u8*)data+size));
    }
    void* getBufferData(u32 buffer)
    {
        std::vector<u8>* BufferData = getBuffer(buffer);
        if(BufferData == nullptr) return nullptr;
        return BufferData->data();
    }
    u32   getBufferSize(u32 buffer)
    {
        std::vector<u8>* BufferData = getBuffer(buffer);
        if(BufferData == nullptr) return 0;

        return BufferData->size();
    }

    void  setBufferElement(u32 buffer,u32 index,const void* value,u32 size) {}
    void  addBufferElement(u32 buffer,const void* value,u32 size) {}
    void* getBufferElement(u32 buffer,u32 index) {}
    u32   getBufferElementCount(u32 buffer) {}

    u16  getIndicesBufferSize()
    {
        return Indices.size();
    }
    void setIndicesBufferData(const u16* data,u16 size)
    {
        Indices.clear();
        addIndicesBufferData(data,size);
    }
    void addIndicesBufferData(const u16* data,u16 size)
    {
        if(data == nullptr) return;
        if(data && size)
            Indices.insert(Indices.end(),data,(data+size));
    }
    u16* getIndicesBufferData()
    {
        return Indices.data();
    }
    //---------------------------------------------------------------------------
    void          setVertexFormat(SVertexFormat newFormat)
    {
        VertexFormat = newFormat;
    }
    SVertexFormat getVertexFormat()
    {
        return VertexFormat;
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

    void lock()
    {
        Positions.clear();
        TexCoords.clear();
    }
    void unlock()   {}
    void render()  {}

    bool isValid()
    {
        return false;
    }

protected:
    inline std::vector<u8>* getBuffer(u32& buffer)
    {
        switch(buffer)
        {
        case EVA_POSITION:
            return &Positions;
        case EVA_NORMAL:
            return &Normals;
        case EVA_BINORMAL:
            return &Binormals;
        case EVA_TANGENT:
            return &Tangents;
        case EVA_COLOR:
            return &Colors;
        case EVA_TEXCOORD:
            return &TexCoords;
        case EVA_CUSTOM:
            return &CustomVerticles;
        default:
            return nullptr;
        }
    }

    SVertexFormat      VertexFormat;
    std::vector<u8>    Positions;
    std::vector<u8>    Normals;
    std::vector<u8>    Binormals;
    std::vector<u8>    Tangents;
    std::vector<u8>    Colors;
    std::vector<u8>    TexCoords;
    std::vector<u8>    CustomVerticles;

    std::vector<u16>   Indices;

    u32                     PrimitiveType;

    u32                     BufferType;
    bool                    UpdateRequired;
};

}
}
#endif // CVertexBuffer_H
