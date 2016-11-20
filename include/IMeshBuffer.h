#ifndef IMESHBUFFER_H
#define IMESHBUFFER_H



#include "ne_types.h"
#include "Resource.h"


#include "SVertex.h"
#include "ERendererEnums.h"

#include "IMaterial.h"

namespace novaengine
{
namespace renderer
{

enum E_VERTEX_BUFFER_MAPPING_HINT
{
    EMBMH_DEFAULT         = 0,
    EMBMH_VBO_STREAM      = 1,
    EMBMH_VBO_STATIC      = 2,
    EMBMH_VBO_STREAM_COPY = 3,
    EMBMH_VBO_STATIC_COPY = 4
};

struct SVertexBufferFormat
{
        SVertexBufferFormat(){};
        SVertexBufferFormat(u32 attribute_id,u32 attribute_type,u32 attribute_size):
            id(attribute_id),type(attribute_type),size(attribute_size) {}
        SVertexBufferFormat(const SVertexBufferFormat& other):
            id(other.id),type(other.type),size(other.size) {}

    u32 id;
    u32 type;
    u32 size;
};

class IMeshBuffer : public Resource
{
public:

    virtual void setMaterial(renderer::IMaterial*) = 0;
    virtual renderer::IMaterial* getMaterial()     = 0;

	virtual u32 getVertexCount()  = 0;
	virtual u32 getIndicesCount() = 0;

	//create/delete buffer and request idx from uid/hint
    virtual s32  createBuffer(u32 buffer_uid,u32 buffer_hint = EVA_CUSTOM) = 0;
    virtual void deleteBuffer(u32 buffer_uid) = 0;

    virtual u32  getBuffersCount() = 0;

    virtual s32  getBufferIndexByUID(u32 buffer_uid) = 0;
    virtual s32  getBufferIndexByHint(u32 buffer_hint) = 0;

    //Manipulate buffers data
    virtual void                       setBufferFormat(s32 buffer_index,const SVertexBufferFormat& newFormat) = 0;
    virtual const SVertexBufferFormat& getBufferFormat(s32 buffer_index) = 0;

	virtual void   resizeBuffer(s32 buffer_index,size_t size) = 0;
    virtual void   setBufferData(s32 buffer_index,const void* data,size_t size) = 0;
    virtual void   addBufferData(s32 buffer_index,const void* data,size_t size) = 0;
    virtual void*  getBufferData(s32 buffer_index) = 0;
    virtual size_t getBufferSize(s32 buffer_index) = 0;

	//TODO
    //virtual void  setBufferElement(u32 buffer_id,u32 index,const void* value,size_t size = 0) = 0;
    //virtual void  addBufferElement(u32 buffer_id,          const void* value,size_t size = 0) = 0;
    //virtual void* getBufferElement(u32 buffer_id,u32 index) = 0;
    //virtual u32   getBufferElementCount(u32 buffer_id) = 0;

    //!indices
	virtual void   resizeIndicesBuffer(size_t size) = 0;
    virtual size_t getIndicesBufferSize() = 0;
    virtual void   setIndicesBufferData(const void* data,size_t size) = 0;
    virtual void   addIndicesBufferData(const void* data,size_t size) = 0;
    virtual void*  getIndicesBufferData() = 0;

    virtual void setIndicesBuffer32(bool) = 0;
    virtual bool isIndicesBuffer32()      = 0;

    virtual void setPrimitiveType(E_PRIMITIVE_TYPE) = 0;
    virtual u32  getPrimitiveType()                 = 0;

    virtual void setUpdateRequest(bool) = 0;
    virtual bool getUpdateRequest()     = 0;
    virtual void update()               = 0;

    virtual void lock(bool edit_only = false) = 0;
    virtual void unlock()                     = 0;

    virtual void setMappingHint(u32) = 0;
    virtual u32  getMappingHint()    = 0;

    virtual bool isValid() = 0;

	virtual void draw() = 0;
};


}
}
#endif // IMeshBuffer_H
