#ifndef IVERTEXARRAY_H
#define IVERTEXARRAY_H



#include "NTypes.h"
#include "Resource.h"


#include "SVertex.h"
#include "SPrimitive.h"
#include "ERendererEnums.h"

namespace novaengine
{
namespace renderer
{

enum E_VERTEX_BUFFER_TYPE
{
    EVBT_UNDEFINDED = 0x0, //!< Not Defined. Vertices maybe exist, maybe not...
    EVBT_RAWDATA    = 0x1, //!< Common VertexBuffer. Store data in ram only
    EVBT_VBO_STREAM = 0x2, //!< VertexBufferObject (VBO) Store data both in vbo and ram
    EVBT_VBO_STATIC = 0x3  //!< VertexBufferObject (VBO) Store data only in vbo, to change any vertex you must call lock() (of course, unlock() if you done..)
};

class IVertexBuffer : public Resource
{
public:

    virtual void  setBufferData(u32 buffer,const void* data,u32 size) = 0;
    virtual void  addBufferData(u32 buffer,const void* data,u32 size) = 0;
    virtual void* getBufferData(u32 buffer)                           = 0;
    virtual u32   getBufferSize(u32 buffer)                           = 0;

    virtual void  setBufferElement(u32 buffer,u32 index,const void* value,u32 size = 0) = 0;
    virtual void  addBufferElement(u32 buffer,const void* value,u32 size = 0)           = 0;
    virtual void* getBufferElement(u32 buffer,u32 index) = 0;
    virtual u32   getBufferElementCount(u32 buffer) = 0;

    //!indices
    virtual u16  getIndicesBufferSize() = 0;
    virtual void setIndicesBufferData(const u16* data,u16 size) = 0;
    virtual void addIndicesBufferData(const u16* data,u16 size) = 0;
    virtual u16* getIndicesBufferData() = 0;

    virtual void          setVertexFormat(SVertexFormat newFormat) = 0;
    virtual SVertexFormat getVertexFormat() = 0;

    virtual void setBufferType(E_VERTEX_BUFFER_TYPE type)  = 0;
    virtual u32  getBufferType()                           = 0;

    virtual void setPrimitiveType(E_PRIMITIVE_TYPE)     = 0;
    virtual u32  getPrimitiveType()                     = 0;

    virtual void setUpdateRequired(bool) = 0;
    virtual bool isUpdateRequired()      = 0;
    virtual bool Update()                = 0;

    virtual void lock()     = 0;
    virtual void unlock()   = 0;

    virtual bool isValid() = 0;
};


}
}
#endif // IVertexBuffer_H
