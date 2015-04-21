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
    EVBT_VBO_STATIC = 0x3, //!< VertexBufferObject (VBO) Store data only in vbo, to change any vertex you must call lock() (of course, unlock() if you done..)
};

class IVertexBuffer : public Resource
{
public:

    virtual u32 getVertexCount()                        = 0;

    virtual void addPosition(core::vector3f)            = 0;
    virtual void addPositions(core::vector3f*,u32)      = 0;
    virtual void setPositions(core::vector3f*,u32)      = 0;
    virtual u32  getPositionsCount()                    = 0;
    virtual core::vector3f* getPositions()              = 0;
    virtual std::vector<core::vector3f>* getPositionsArray() = 0;

    virtual void addUV(core::vector2f)              = 0;
    virtual void addUVs(core::vector2f*,u32)        = 0;
    virtual void setUVs(core::vector2f*,u32)        = 0;
    virtual u32  getUVsCount()                      = 0;
    virtual core::vector2f* getUVs()                = 0;
    virtual std::vector<core::vector2f>* getUVsArray()   = 0;

    virtual void addNormal(core::vector3f)              = 0;
    virtual void addNormals(core::vector3f*,u32)        = 0;
    virtual void setNormals(core::vector3f*,u32)        = 0;
    virtual u32  getNormalsCount()                      = 0;
    virtual core::vector3f* getNormals()                = 0;
    virtual std::vector<core::vector3f>* getNormalsArray()   = 0;

    virtual void addColor(core::color4u)             = 0;
    virtual void addColors(core::color4u*,u32)       = 0;
    virtual void setColors(core::color4u*,u32)       = 0;
    virtual u32  getColorsCount()                     = 0;
    virtual core::color4u* getColors()               = 0;
    virtual std::vector<core::color4u>* getColorsArray()  = 0;

    virtual void addIndex(u16 index)        = 0;
    virtual void addIndices(u16*,u16)       = 0;
    virtual void setIndices(u16*,u16)       = 0;
    virtual u16  getIndicesCount()          = 0;
    virtual u16* getIndices()               = 0;
    virtual std::vector<u16>* getIndicesArray()  = 0;

    virtual u32  getVertexFormat()     = 0;
    virtual void setVertexFormat(u32)  = 0;

    virtual u32  getPrimitiveType()                     = 0;
    virtual void setPrimitiveType(E_PRIMITIVE_TYPE)     = 0;

    virtual void setBufferType(E_VERTEX_BUFFER_TYPE type)  = 0;
    virtual u32  getBufferType()                           = 0;

    virtual void setUpdateRequired(bool) = 0;
    virtual bool isUpdateRequired()      = 0;
    virtual bool Update()                = 0;

    virtual void lock()     = 0;
    virtual void unlock()   = 0;

    virtual bool isValid() = 0;
    virtual void render()  = 0;


};


}
}
#endif // IVertexBuffer_H
