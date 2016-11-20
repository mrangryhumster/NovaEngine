#include "CMeshBuffer.h"

namespace novaengine
{
namespace renderer
{
CMeshBuffer::CMeshBuffer():
    Material(nullptr),
    m_isIndicesBuffer32(true),
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

    for(auto it = m_Buffers.begin(); it != m_Buffers.end(); it++)
        (*it).Data.clear();
    m_Buffers.clear();
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
    s32 Buffer_index = getBufferIndexByHint(renderer::EVA_POSITION);

    if(Buffer_index < 0)
        DEBUG_EXCEPTION("NO_EVA_POSITION");

    return m_Buffers[Buffer_index].Data.size() / (m_Buffers[Buffer_index].Format.size * ne_sizeof((NE_TYPE)m_Buffers[Buffer_index].Format.type));
}
//---------------------------------------------------------------------------
u32 CMeshBuffer::getIndicesCount()
{
    return m_IndicesBuffer.size() / (m_isIndicesBuffer32 ? 4 : 2) ;
}
//---------------------------------------------------------------------------
s32 CMeshBuffer::createBuffer(u32 buffer_uid,u32 buffer_hint)
{
    SVertexBuffer newBuffer;

    newBuffer.UID   = buffer_uid;
    newBuffer.Hint = buffer_hint;

    switch (buffer_hint)
    {
    case EVA_POSITION:
        newBuffer.Format = SVertexBufferFormat(buffer_uid,NTYPE_f32,3);
        break;
    case EVA_NORMAL:
        newBuffer.Format = SVertexBufferFormat(buffer_uid,NTYPE_f32,3);
        break;
    case EVA_BINORMAL:
        newBuffer.Format = SVertexBufferFormat(buffer_uid,NTYPE_f32,3);
        break;
    case EVA_TANGENT:
        newBuffer.Format = SVertexBufferFormat(buffer_uid,NTYPE_f32,3);
        break;
    case EVA_COLOR:
        newBuffer.Format = SVertexBufferFormat(buffer_uid,NTYPE_u8,4);
        break;
    case EVA_TEXCOORD:
        newBuffer.Format = SVertexBufferFormat(buffer_uid,NTYPE_f32,2);
        break;
    default:
        newBuffer.Format = SVertexBufferFormat();
    }

    m_Buffers.push_back(newBuffer);
    return (getBuffersCount() - 1);
}
//---------------------------------------------------------------------------
void CMeshBuffer::deleteBuffer(u32 buffer_uid)
{
    for(auto it = m_Buffers.begin(); it != m_Buffers.end(); it++)
        if((*it).UID == buffer_uid)
            {
                (*it).Data.clear();
                m_Buffers.erase(it);
                break;
            }
}
//---------------------------------------------------------------------------
u32 CMeshBuffer::getBuffersCount()
{
    return m_Buffers.size();
}
//---------------------------------------------------------------------------
s32 CMeshBuffer::getBufferIndexByUID(u32 buffer_uid)
{
    u32 BuffersCount = getBuffersCount();
    for(u32 i = 0; i < BuffersCount; i++)
        if(m_Buffers[i].UID == buffer_uid)
            return static_cast<s32>(i);

    return -1;
}
//---------------------------------------------------------------------------
s32 CMeshBuffer::getBufferIndexByHint(u32 buffer_hint)
{
    u32 BuffersCount = getBuffersCount();
    for(u32 i = 0; i < BuffersCount; i++)
        if(m_Buffers[i].Hint == buffer_hint)
            return static_cast<s32>(i);

    return -1;
}
//---------------------------------------------------------------------------
void CMeshBuffer::setBufferFormat(s32 buffer_index,const SVertexBufferFormat& newFormat)
{
    if(buffer_index < 0)
        DEBUG_EXCEPTION("setBufferFormat");

    m_Buffers[buffer_index].Format = newFormat;
}
//---------------------------------------------------------------------------
const SVertexBufferFormat& CMeshBuffer::getBufferFormat(s32 buffer_index)
{
    if(buffer_index < 0)
        DEBUG_EXCEPTION("setBufferFormat");

    return m_Buffers[buffer_index].Format;
}
//---------------------------------------------------------------------------
void CMeshBuffer::resizeBuffer(s32 buffer_index, size_t size)
{
	if (buffer_index < 0)
		DEBUG_EXCEPTION("reserveBuffer");
	m_Buffers[buffer_index].Data.resize(size);
}
//---------------------------------------------------------------------------
void  CMeshBuffer::setBufferData(s32 buffer_index,const void* data,size_t size)
{
    if(buffer_index < 0)
        DEBUG_EXCEPTION("setBufferData");


    m_Buffers[buffer_index].Data.clear();
    if(data && size)
        m_Buffers[buffer_index].Data.insert(m_Buffers[buffer_index].Data.end(),(u8*)data,((u8*)data+size));
}
//---------------------------------------------------------------------------
void  CMeshBuffer::addBufferData(s32 buffer_index,const void* data,size_t size)
{
    if(buffer_index < 0)
        DEBUG_EXCEPTION("addBufferData - OutOfBounds");
    if(buffer_index > m_Buffers.size())
        DEBUG_EXCEPTION("addBufferData - OutOfBounds");


    if(data && size)
        m_Buffers[buffer_index].Data.insert(m_Buffers[buffer_index].Data.end(),(u8*)data,((u8*)data+size));
}
//---------------------------------------------------------------------------
void* CMeshBuffer::getBufferData(s32 buffer_index)
{
    if(buffer_index < 0)
        DEBUG_EXCEPTION("getBufferData");
    return m_Buffers[buffer_index].Data.data();
}
//---------------------------------------------------------------------------
size_t CMeshBuffer::getBufferSize(s32 buffer_index)
{
    if(buffer_index < 0)
        DEBUG_EXCEPTION("getBufferSize");

    return m_Buffers[buffer_index].Data.size();
}
//---------------------------------------------------------------------------
void CMeshBuffer::setBufferElement(s32 buffer_index,u32 index,const void* value,size_t size) {}
//---------------------------------------------------------------------------
void CMeshBuffer::addBufferElement(s32 buffer_index,const void* value,size_t size) {}
//---------------------------------------------------------------------------
void* CMeshBuffer::getBufferElement(s32 buffer_index,u32 index)
{
    return nullptr;
}
//---------------------------------------------------------------------------
u32 CMeshBuffer::getBufferElementCount(s32 buffer_index)
{
    return 0;
}
//---------------------------------------------------------------------------
void CMeshBuffer::resizeIndicesBuffer(size_t size)
{
	m_IndicesBuffer.resize(size);
}
//---------------------------------------------------------------------------
size_t CMeshBuffer::getIndicesBufferSize()
{
    return m_IndicesBuffer.size();
}
//---------------------------------------------------------------------------
void CMeshBuffer::setIndicesBufferData(const void* data,size_t size)
{
    m_IndicesBuffer.clear();
    addIndicesBufferData(data,size);
}
//---------------------------------------------------------------------------
void CMeshBuffer::addIndicesBufferData(const void* data,size_t size)
{
    if(data == nullptr) return;
    if(data && size)
        m_IndicesBuffer.insert(m_IndicesBuffer.end(),(u8*)data,((u8*)data+size));
}
//---------------------------------------------------------------------------
void* CMeshBuffer::getIndicesBufferData()
{
    return m_IndicesBuffer.data();
}
//---------------------------------------------------------------------------
void CMeshBuffer::setIndicesBuffer32(bool state)
{
	m_isIndicesBuffer32 = state;
}
//---------------------------------------------------------------------------
bool CMeshBuffer::isIndicesBuffer32()
{
	return m_isIndicesBuffer32;
}
//---------------------------------------------------------------------------
u32 CMeshBuffer::getPrimitiveType()
{
    return PrimitiveType;
}
//---------------------------------------------------------------------------
void CMeshBuffer::setPrimitiveType(E_PRIMITIVE_TYPE newPrimitiveType)
{
    PrimitiveType = newPrimitiveType;
}
//---------------------------------------------------------------------------
void CMeshBuffer::setUpdateRequest(bool isUpdateRequired)
{
    UpdateRequired = isUpdateRequired;
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
void CMeshBuffer::lock(bool edit_only)
{
    setLockedFlag(true);
}
//---------------------------------------------------------------------------
void CMeshBuffer::unlock()
{
    setLockedFlag(false);
}
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
//---------------------------------------------------------------------------
void CMeshBuffer::draw()
{
}


}
}
