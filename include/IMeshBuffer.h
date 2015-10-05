#ifndef IMESHBUFFER_H
#define IMESHBUFFER_H



#include "NTypes.h"
#include "Resource.h"


#include "SVertex.h"
#include "SPrimitive.h"
#include "ERendererEnums.h"

#include "IMaterial.h"

namespace novaengine
{
namespace renderer
{

enum E_VERTEX_BUFFER_TYPE
{
    EMBT_UNDEFINDED = 0x0, //!< Not Defined. Vertices maybe exist, maybe not...
    EMBT_RAWDATA    = 0x1, //!< Common MeshBuffer. Store data in ram only
    EMBT_VBO_STREAM = 0x2, //!< MeshBufferObject (VBO) Store data both in vbo and ram
    EMBT_VBO_STATIC = 0x3  //!< MeshBufferObject (VBO) Store data only in vbo, to change any vertex you must call lock() (of course, unlock() if you done..)
};

enum E_VERTEX_BUFFER_MAPPING_HINT
{
    EMBMH_DEFAULT         = 0,
    EMBMH_VBO_STREAM      = 1,
    EMBMH_VBO_STATIC      = 2,
    EMBMH_VBO_STREAM_COPY = 3,
    EMBMH_VBO_STATIC_COPY = 4
};

class IMeshBuffer : public Resource
{
public:

    virtual void setMaterial(renderer::IMaterial*) = 0;
    virtual renderer::IMaterial* getMaterial()     = 0;

    virtual void   setBufferData(u32 buffer,const void* data,size_t size) = 0;
    virtual void   addBufferData(u32 buffer,const void* data,size_t size) = 0;
    virtual void*  getBufferData(u32 buffer)                           = 0;
    virtual size_t getBufferSize(u32 buffer)                           = 0;

    virtual void  setBufferElement(u32 buffer,u32 index,const void* value,size_t size = 0) = 0;
    virtual void  addBufferElement(u32 buffer,const void* value,size_t size = 0)           = 0;
    virtual void* getBufferElement(u32 buffer,u32 index) = 0;
    virtual u32   getBufferElementCount(u32 buffer) = 0;

    //!indices
    virtual size_t getIndicesBufferSize() = 0;
    virtual void   setIndicesBufferData(const void* data,size_t size) = 0;
    virtual void   addIndicesBufferData(const void* data,size_t size) = 0;
    virtual void*  getIndicesBufferData() = 0;

    virtual void   setIndicesBufferType(u32 type) = 0;
    virtual u32    getIndicesBufferType() = 0;

    virtual void          setVertexFormat(SVertexFormat newFormat) = 0;
    virtual SVertexFormat getVertexFormat() = 0;

    virtual void setPrimitiveType(E_PRIMITIVE_TYPE)     = 0;
    virtual u32  getPrimitiveType()                     = 0;

    virtual void setUpdateRequest(bool) = 0;
    virtual bool getUpdateRequest()     = 0;
    virtual void update()               = 0;

    virtual void lock()     = 0;
    virtual void unlock()   = 0;

    virtual void setMappingHint(u32) = 0;
    virtual u32  getMappingHint()    = 0;

    virtual bool isValid() = 0;
};


}
}
#endif // IMeshBuffer_H
