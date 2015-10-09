#ifndef CMeshBuffer_H
#define CMeshBuffer_H

#include <stdio.h>
#include <vector>
#include "IMeshBuffer.h"

namespace novaengine
{
namespace renderer
{
class CMeshBuffer : public IMeshBuffer
{
public:
    CMeshBuffer();
    virtual ~CMeshBuffer();
    //---------------------------------------------------------------------------
    void                 setMaterial(renderer::IMaterial*);
    renderer::IMaterial* getMaterial();
    //---------------------------------------------------------------------------
    void   setBufferData(u32 buffer,const void* data,size_t size);
    void   addBufferData(u32 buffer,const void* data,size_t size);
    void*  getBufferData(u32 buffer);
    size_t getBufferSize(u32 buffer);

    void  setBufferElement(u32 buffer,u32 index,const void* value,size_t size);
    void  addBufferElement(u32 buffer,const void* value,size_t size);
    void* getBufferElement(u32 buffer,u32 index);
    u32   getBufferElementCount(u32 buffer);

    size_t getIndicesBufferSize();
    void   setIndicesBufferData(const void* data,size_t size);
    void   addIndicesBufferData(const void* data,size_t size);
    void*  getIndicesBufferData();

    void   setIndicesBufferType(u32 type);
    u32    getIndicesBufferType();
    //---------------------------------------------------------------------------
    void                  setVertexFormat(SVertexFormat newFormat);
	const SVertexFormat&  getVertexFormat();

    u32 getPrimitiveType();
    void setPrimitiveType(E_PRIMITIVE_TYPE pt);

    void setUpdateRequest(bool);
    bool getUpdateRequest();
    void update();

    void lock();
    void unlock();

    void render();

    bool isValid();

    void setMappingHint(u32);
    u32  getMappingHint();

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

    renderer::IMaterial* Material;

    SVertexFormat      VertexFormat;
    std::vector<u8>    Positions;
    std::vector<u8>    Normals;
    std::vector<u8>    Binormals;
    std::vector<u8>    Tangents;
    std::vector<u8>    Colors;
    std::vector<u8>    TexCoords;
    std::vector<u8>    CustomVerticles;

    std::vector<u8>    Indices;
    u32                IndicesType;

    u32                PrimitiveType;

    u32    MappingHint;
    bool   UpdateRequired;
};

}
}
#endif // CMeshBuffer_H
