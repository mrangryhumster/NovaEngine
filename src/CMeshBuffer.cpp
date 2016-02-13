#include "CMeshBuffer.h"

namespace novaengine
{
namespace renderer
{
CMeshBuffer::CMeshBuffer():
    Material(nullptr),
    VertexFormat(EVA_POSITION),
    IndicesType(NTYPE_u16),
    PrimitiveType(EPT_TRIANGLE),
    MappingHint(EMBMH_DEFAULT),
    UpdateRequired(true)
{
}
//---------------------------------------------------------------------------
CMeshBuffer::~CMeshBuffer()
{
    if(Material)
        Material->release();
}
//---------------------------------------------------------------------------
void CMeshBuffer::setMaterial(renderer::IMaterial* newMaterial)
{
    if(newMaterial)
        newMaterial->capture();
    if(Material)
        Material->release();
    Material = newMaterial;
}
//---------------------------------------------------------------------------
renderer::IMaterial* CMeshBuffer::getMaterial()
{
    return Material;
}
//---------------------------------------------------------------------------
u32 CMeshBuffer::getVertexCount()
{
	return Positions.size() / (VertexFormat.getAttributeFormat(EVA_POSITION)->size * ne_sizeof((NE_TYPE)VertexFormat.getAttributeFormat(EVA_POSITION)->type));
}
//---------------------------------------------------------------------------
u32 CMeshBuffer::getIndicesCount()
{
	return Indices.size() / ne_sizeof((NE_TYPE)IndicesType);
}
//---------------------------------------------------------------------------
void  CMeshBuffer::setBufferData(u32 buffer,const void* data,size_t size)
{
    std::vector<u8>* BufferData = getBuffer(buffer);
    if(BufferData == nullptr) return;

    BufferData->clear();
    if(data && size)
        BufferData->insert(BufferData->end(),(u8*)data,((u8*)data+size));
}
//---------------------------------------------------------------------------
void  CMeshBuffer::addBufferData(u32 buffer,const void* data,size_t size)
{
    std::vector<u8>* BufferData = getBuffer(buffer);
    if(BufferData == nullptr) return;

    if(data && size)
        BufferData->insert(BufferData->end(),(u8*)data,((u8*)data+size));
}
//---------------------------------------------------------------------------
void* CMeshBuffer::getBufferData(u32 buffer)
{
    std::vector<u8>* BufferData = getBuffer(buffer);
    if(BufferData == nullptr) return nullptr;
    return BufferData->data();
}
//---------------------------------------------------------------------------
size_t CMeshBuffer::getBufferSize(u32 buffer)
{
    std::vector<u8>* BufferData = getBuffer(buffer);
    if(BufferData == nullptr) return 0;

    return BufferData->size();
}
//---------------------------------------------------------------------------
void CMeshBuffer::setBufferElement(u32 buffer,u32 index,const void* value,size_t size) {}
//---------------------------------------------------------------------------
void CMeshBuffer::addBufferElement(u32 buffer,const void* value,size_t size) {}
//---------------------------------------------------------------------------
void* CMeshBuffer::getBufferElement(u32 buffer,u32 index) {return nullptr;}
//---------------------------------------------------------------------------
u32 CMeshBuffer::getBufferElementCount(u32 buffer) {return 0;}
//---------------------------------------------------------------------------
size_t CMeshBuffer::getIndicesBufferSize()
{
    return Indices.size();
}
//---------------------------------------------------------------------------
void CMeshBuffer::setIndicesBufferData(const void* data,size_t size)
{
    Indices.clear();
    addIndicesBufferData(data,size);
}
//---------------------------------------------------------------------------
void CMeshBuffer::addIndicesBufferData(const void* data,size_t size)
{
    if(data == nullptr) return;
    if(data && size)
        Indices.insert(Indices.end(),(u8*)data,((u8*)data+size));
}
//---------------------------------------------------------------------------
void* CMeshBuffer::getIndicesBufferData()
{
    return Indices.data();
}
//---------------------------------------------------------------------------
void CMeshBuffer::setIndicesBufferType(u32 type)
{
    if(type == NTYPE_u16 || type == NTYPE_u32)
        IndicesType = type;
    else
        IndicesType = NTYPE_u16;
}
//---------------------------------------------------------------------------
u32 CMeshBuffer::getIndicesBufferType()
{
    return IndicesType;
}
//---------------------------------------------------------------------------
void CMeshBuffer::setVertexFormat(const SVertexFormat& newFormat)
{
    VertexFormat = newFormat;
}
//---------------------------------------------------------------------------
const SVertexFormat& CMeshBuffer::getVertexFormat()
{
    return VertexFormat;
}
//---------------------------------------------------------------------------
u32 CMeshBuffer::getPrimitiveType()
{
    return PrimitiveType;
}
//---------------------------------------------------------------------------
void CMeshBuffer::setPrimitiveType(E_PRIMITIVE_TYPE pt)
{
    PrimitiveType = pt;
}
//---------------------------------------------------------------------------
void CMeshBuffer::setUpdateRequest(bool up)
{
    UpdateRequired = up;
}
//---------------------------------------------------------------------------
bool CMeshBuffer::getUpdateRequest()
{
    return UpdateRequired;
}
//---------------------------------------------------------------------------
void CMeshBuffer::update()
{
    UpdateRequired = false;
}
//---------------------------------------------------------------------------
void CMeshBuffer::lock()
{
    Positions.clear();
    TexCoords.clear();
}
//---------------------------------------------------------------------------
void CMeshBuffer::unlock()
{}
//---------------------------------------------------------------------------
void CMeshBuffer::render()
{}
//---------------------------------------------------------------------------
bool CMeshBuffer::isValid()
{
    return false;
}
//---------------------------------------------------------------------------
void CMeshBuffer::setMappingHint(u32 Hint)
{
	UpdateRequired = true;
    MappingHint = Hint;
}
//---------------------------------------------------------------------------
u32  CMeshBuffer::getMappingHint()
{
    return MappingHint;
}

}
}
