#ifndef CMeshBuffer_H
#define CMeshBuffer_H

#include <stdio.h>
#include <vector>
#include "IMeshBuffer.h"
#include "NLogger.h"

#define DEBUG_EXCEPTION(E) {printf("Exception: %s FILE:%s LINE:%d\n",E,__FILE__,__LINE__);throw;}
namespace novaengine
{
namespace renderer
{

struct SVertexBuffer
{
    u32                 UID;
    std::vector<u8>     Data;
    SVertexBufferFormat Format;
    u32                 Hint;
};

class CMeshBuffer : public IMeshBuffer
{
public:
    CMeshBuffer();
    virtual ~CMeshBuffer();
    //---------------------------------------------------------------------------
    void                 setMaterial(renderer::IMaterial*);
    renderer::IMaterial* getMaterial();
    //---------------------------------------------------------------------------
    u32 getVertexCount();
    u32 getIndicesCount();
    //---------------------------------------------------------------------------
    s32   createBuffer(u32 buffer_uid,u32 buffer_hint);
    void  deleteBuffer(u32 buffer_uid);

    u32   getBuffersCount();

    s32   getBufferIndexByUID(u32 buffer_uid);
    s32   getBufferIndexByHint(u32 buffer_hint);

    void  setBufferFormat(s32 buffer_index,const SVertexBufferFormat& newFormat);
    const SVertexBufferFormat& getBufferFormat(s32 buffer_index);
    //---------------------------------------------------------------------------
    void   setBufferData(s32 buffer_index,const void* data,size_t size);
    void   addBufferData(s32 buffer_index,const void* data,size_t size);
    void*  getBufferData(s32 buffer_index);
    size_t getBufferSize(s32 buffer_index);

    void  setBufferElement(s32 buffer_index,u32 index,const void* value,size_t size);
    void  addBufferElement(s32 buffer_index,const void* value,size_t size);
    void* getBufferElement(s32 buffer_index,u32 index);
    u32   getBufferElementCount(s32 buffer_index);

    size_t getIndicesBufferSize();
    void   setIndicesBufferData(const void* data,size_t size);
    void   addIndicesBufferData(const void* data,size_t size);
    void*  getIndicesBufferData();

    void   setIndicesBufferType(u32 type);
    u32    getIndicesBufferType();
    //---------------------------------------------------------------------------
    u32  getPrimitiveType();
    void setPrimitiveType(E_PRIMITIVE_TYPE pt);

    void setUpdateRequest(bool);
    bool getUpdateRequest();
    void update();

    void lock(bool edit_only);
    void unlock();

    void setMappingHint(u32);
    u32  getMappingHint();

	bool isValid();

	void draw();

protected:

    inline SVertexBuffer* findBufferByHint(const u32& Hint)
    {
        for(auto it = m_Buffers.begin(); it != m_Buffers.end(); it++)
            if((*it).Hint == Hint)
                return &(*it);
        return nullptr;
    }

    inline SVertexBuffer* findBufferById(const u32& UID)
    {
        for(auto it = m_Buffers.begin(); it != m_Buffers.end(); it++)
            if((*it).UID == UID)
                return &(*it);
        return nullptr;
    }



    renderer::IMaterial* Material;

    //Vertex attributes
    std::vector<SVertexBuffer> m_Buffers;

    //Indexes
    std::vector<u8>    m_IndicesBuffer;
    u32                m_IndicesBufferType;

    //Primitive type
    u32                PrimitiveType;

    //Some internal staff
    u32    MappingHint;
    bool   UpdateRequired;
};

}
}
#endif // CMeshBuffer_H
