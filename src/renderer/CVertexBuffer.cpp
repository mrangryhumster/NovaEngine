#include "CVertexBuffer.h"

namespace novaengine
{
namespace renderer
{
CVertexBuffer::CVertexBuffer():
    VertexFormat(EVA_POSITION),
    IndicesType(NTYPE_u16),
    PrimitiveType(EPT_TRIANGLE),
    MappingHint(EVBMH_DEFAULT),
    UpdateRequired(true)
{
}
//---------------------------------------------------------------------------
void  CVertexBuffer::setBufferData(u32 buffer,const void* data,size_t size)
{
    std::vector<u8>* BufferData = getBuffer(buffer);
    if(BufferData == nullptr) return;

    BufferData->clear();
    if(data && size)
        BufferData->insert(BufferData->end(),(u8*)data,((u8*)data+size));
}
//---------------------------------------------------------------------------
void  CVertexBuffer::addBufferData(u32 buffer,const void* data,size_t size)
{
    std::vector<u8>* BufferData = getBuffer(buffer);
    if(BufferData == nullptr) return;

    if(data && size)
        BufferData->insert(BufferData->end(),(u8*)data,((u8*)data+size));
}
//---------------------------------------------------------------------------
void* CVertexBuffer::getBufferData(u32 buffer)
{
    std::vector<u8>* BufferData = getBuffer(buffer);
    if(BufferData == nullptr) return nullptr;
    return BufferData->data();
}
//---------------------------------------------------------------------------
size_t CVertexBuffer::getBufferSize(u32 buffer)
{
    std::vector<u8>* BufferData = getBuffer(buffer);
    if(BufferData == nullptr) return 0;

    return BufferData->size();
}
//---------------------------------------------------------------------------
void CVertexBuffer::setBufferElement(u32 buffer,u32 index,const void* value,size_t size) {}
//---------------------------------------------------------------------------
void CVertexBuffer::addBufferElement(u32 buffer,const void* value,size_t size) {}
//---------------------------------------------------------------------------
void* CVertexBuffer::getBufferElement(u32 buffer,u32 index) {}
//---------------------------------------------------------------------------
u32 CVertexBuffer::getBufferElementCount(u32 buffer) {}
//---------------------------------------------------------------------------
size_t CVertexBuffer::getIndicesBufferSize()
{
    return Indices.size();
}
//---------------------------------------------------------------------------
void CVertexBuffer::setIndicesBufferData(const void* data,size_t size)
{
    Indices.clear();
    addIndicesBufferData(data,size);
}
//---------------------------------------------------------------------------
void CVertexBuffer::addIndicesBufferData(const void* data,size_t size)
{
    if(data == nullptr) return;
    if(data && size)
        Indices.insert(Indices.end(),(u8*)data,((u8*)data+size));
}
//---------------------------------------------------------------------------
void* CVertexBuffer::getIndicesBufferData()
{
    return Indices.data();
}
//---------------------------------------------------------------------------
void CVertexBuffer::setIndicesBufferType(u32 type)
{
    if(type == NTYPE_u16 || type == NTYPE_u32)
        IndicesType = type;
    else
        IndicesType = NTYPE_u16;
}
//---------------------------------------------------------------------------
u32 CVertexBuffer::getIndicesBufferType()
{
    return IndicesType;
}
//---------------------------------------------------------------------------
void CVertexBuffer::setVertexFormat(SVertexFormat newFormat)
{
    VertexFormat = newFormat;
}
//---------------------------------------------------------------------------
SVertexFormat CVertexBuffer::getVertexFormat()
{
    return VertexFormat;
}
//---------------------------------------------------------------------------
u32 CVertexBuffer::getPrimitiveType()
{
    return PrimitiveType;
}
//---------------------------------------------------------------------------
void CVertexBuffer::setPrimitiveType(E_PRIMITIVE_TYPE pt)
{
    PrimitiveType = pt;
}
//---------------------------------------------------------------------------
void CVertexBuffer::setUpdateRequest(bool up)
{
    UpdateRequired = up;
}
//---------------------------------------------------------------------------
bool CVertexBuffer::getUpdateRequest()
{
    return UpdateRequired;
}
//---------------------------------------------------------------------------
void CVertexBuffer::update()
{
    UpdateRequired = false;
}
//---------------------------------------------------------------------------
void CVertexBuffer::lock()
{
    Positions.clear();
    TexCoords.clear();
}
//---------------------------------------------------------------------------
void CVertexBuffer::unlock()
{}
//---------------------------------------------------------------------------
void CVertexBuffer::render()
{}
//---------------------------------------------------------------------------
bool CVertexBuffer::isValid()
{
    return false;
}
//---------------------------------------------------------------------------
void CVertexBuffer::setMappingHint(u32 Hint)
{
    MappingHint = Hint;
}
//---------------------------------------------------------------------------
u32  CVertexBuffer::getMappingHint()
{
    return MappingHint;
}

}
}
