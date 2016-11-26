#include "COpenGLMeshBuffer.h"

namespace novaengine
{
namespace renderer
{
COpenGLMeshBuffer::COpenGLMeshBuffer():
    m_CurrentMapping(EMBMH_DEFAULT),
    m_OpenGL_VBO_verticles(0),
    m_OpenGL_VBO_verticles_count(0),
    m_OpenGL_VBO_indices(0),
    m_OpenGL_VBO_indices_count(0),
    m_OpenGL_VAO(0)
{
    create_vbo();
}
//------------------------------------------------------------------------------
COpenGLMeshBuffer::~COpenGLMeshBuffer()
{
    delete_vbo();
    m_OpenGLBuffers.clear();
}
//------------------------------------------------------------------------------
u32 COpenGLMeshBuffer::getVertexCount()
{
    if (isLocked())
        return CMeshBuffer::getVertexCount();
    return m_OpenGL_VBO_verticles_count;
}
//------------------------------------------------------------------------------
u32 COpenGLMeshBuffer::getIndicesCount()
{
    if (isLocked())
        return CMeshBuffer::getIndicesCount();
    return m_OpenGL_VBO_indices_count;
}
//------------------------------------------------------------------------------
s32   COpenGLMeshBuffer::createBuffer(u32 buffer_uid,u32 buffer_hint)
{
    SOpenGLVertexBuffer newOpenGLBuffer;
    m_OpenGLBuffers.push_back(newOpenGLBuffer);
    return CMeshBuffer::createBuffer(buffer_uid,buffer_hint);
}
//------------------------------------------------------------------------------
void  COpenGLMeshBuffer::deleteBuffer(u32 buffer_uid)
{
    m_OpenGLBuffers.erase(m_OpenGLBuffers.begin() + getBufferIndexByUID(buffer_uid));
    CMeshBuffer::deleteBuffer(buffer_uid);
}
//------------------------------------------------------------------------------
size_t COpenGLMeshBuffer::getBufferSize(u32 buffer_index)
{
    if (m_CurrentMapping == EMBMH_DEFAULT)
        return CMeshBuffer::getBufferSize(buffer_index);

    return m_OpenGLBuffers[buffer_index].vbo_size;
}
//------------------------------------------------------------------------------
void COpenGLMeshBuffer::lock(bool edit_only)
{
    CMeshBuffer::lock(edit_only);

    //if buffer not empty we map data from gpu to ram buffers
    //else just clear ram buffers
    if(m_OpenGL_VBO_verticles_count)
    {
        //bind_buffer();

        //!ARRAY BUFFER
        glBindBuffer(GL_ARRAY_BUFFER, m_OpenGL_VBO_verticles);

        void* OpenGL_VBO_Data = glMapBuffer(GL_ARRAY_BUFFER,GL_READ_ONLY );
        if(OpenGL_VBO_Data == nullptr)
            DEBUG_EXCEPTION("glMapBuffer return is null");


        GLsizeiptr OpenGL_Buffer_Shift = 0;
        size_t     Native_BuffersCount = m_Buffers.size();
        for(u32 i = 0; i < Native_BuffersCount; i++)
        {
            if(m_OpenGLBuffers[i].vbo_state == true)
            {
                m_Buffers[i].Data.clear();
                m_Buffers[i].Data.shrink_to_fit();
                m_Buffers[i].Data.insert(m_Buffers[i].Data.end(),((u8*)OpenGL_VBO_Data + OpenGL_Buffer_Shift),((u8*)OpenGL_VBO_Data + OpenGL_Buffer_Shift) + m_OpenGLBuffers[i].vbo_size);
                OpenGL_Buffer_Shift += m_OpenGLBuffers[i].vbo_size;
            }
        }
        glUnmapBuffer(GL_ARRAY_BUFFER);

        //!ELEMENT BUFFER
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_OpenGL_VBO_indices);

        OpenGL_VBO_Data = glMapBuffer(GL_ELEMENT_ARRAY_BUFFER,GL_READ_ONLY);
        if(OpenGL_VBO_Data == nullptr)
            DEBUG_EXCEPTION("glMapBuffer return is null");

        m_IndicesBuffer.clear();
        m_IndicesBuffer.shrink_to_fit();
        m_IndicesBuffer.insert(m_IndicesBuffer.end(),
								reinterpret_cast<u8*>(OpenGL_VBO_Data),
							   (reinterpret_cast<u8*>(OpenGL_VBO_Data) + (m_OpenGL_VBO_indices_count * ((m_isIndicesBuffer32) ? 2 : 4)) ));

        glUnmapBuffer(GL_ELEMENT_ARRAY_BUFFER);

        //unbind_buffer();
    }
    else
    {
        clear_native_buffers();
    }
}
//------------------------------------------------------------------------------
void COpenGLMeshBuffer::unlock()
{
    build_vbo();
    build_vao();
    clear_native_buffers();
    CMeshBuffer::unlock();
}
//------------------------------------------------------------------------------
void COpenGLMeshBuffer::update()
{
    UpdateRequired = false;
}
//------------------------------------------------------------------------------
void COpenGLMeshBuffer::draw()
{
	if (this->isLocked())
		return;

	this->bind_buffer();

	if (this->getIndicesCount() > 0)
		glDrawElements(GL_TRIANGLES, this->getIndicesCount(), m_isIndicesBuffer32 ? GL_UNSIGNED_INT : GL_UNSIGNED_SHORT, 0);
	else
		glDrawArrays(GL_TRIANGLES, 0, this->getVertexCount());

	this->unbind_buffer();
}
//------------------------------------------------------------------------------
void COpenGLMeshBuffer::bind_buffer()
{
    glBindVertexArray(m_OpenGL_VAO);
}
//------------------------------------------------------------------------------
void COpenGLMeshBuffer::unbind_buffer()
{
    glBindVertexArray(0);
}
//------------------------------------------------------------------------------
const u32& COpenGLMeshBuffer::getBufferedVertexCount()
{
    return m_OpenGL_VBO_verticles_count;
}
//------------------------------------------------------------------------------
const u32& COpenGLMeshBuffer::getBufferedIndicesCount()
{
    return m_OpenGL_VBO_indices_count;
}
//------------------------------------------------------------------------------
void COpenGLMeshBuffer::clear_native_buffers()
{
    for(auto it = m_Buffers.begin(); it != m_Buffers.end(); it++)
    {
        (*it).Data.clear();
        (*it).Data.shrink_to_fit();
    }

    m_IndicesBuffer.clear();
    m_IndicesBuffer.shrink_to_fit();
}
//------------------------------------------------------------------------------
void COpenGLMeshBuffer::create_vbo()
{
    glGenBuffers(1, &m_OpenGL_VBO_verticles);
    glGenBuffers(1, &m_OpenGL_VBO_indices);

    glGenVertexArrays(1, &m_OpenGL_VAO);
}
//------------------------------------------------------------------------------
void COpenGLMeshBuffer::delete_vbo()
{
    glDeleteVertexArrays(1, &m_OpenGL_VAO);

    glDeleteBuffers(1, &m_OpenGL_VBO_verticles);
    glDeleteBuffers(1, &m_OpenGL_VBO_indices);
}
//------------------------------------------------------------------------------
void COpenGLMeshBuffer::build_vbo()
{
    u32 IndicesCount = getIndicesCount();
    u32 VertexCount  = getVertexCount();

    if (VertexCount == 0)
        return;

    //Calc final buffer size
    GLsizeiptr OpenGL_BufferSize = 0;
    for(auto it = m_Buffers.begin(); it != m_Buffers.end(); it++)
        OpenGL_BufferSize += (*it).Data.size();

    GLenum OpenGL_BufferUsage = GL_DYNAMIC_DRAW;

    //common buffer bind and init data with zero
    glBindBuffer(GL_ARRAY_BUFFER, m_OpenGL_VBO_verticles);
    glBufferData(GL_ARRAY_BUFFER, OpenGL_BufferSize, 0, OpenGL_BufferUsage);

    //Insert all buffers in vbo
    GLsizeiptr OpenGL_BufferInsert_Shift = 0;
    size_t     Native_BuffersCount = m_Buffers.size();
    for(u32 i = 0; i < Native_BuffersCount; i++)
    {
        m_OpenGLBuffers[i].vbo_size = m_Buffers[i].Data.size();
        if(m_OpenGLBuffers[i].vbo_size == 0)
        {
            m_OpenGLBuffers[i].vbo_state = false;
        }
        else
        {
            glBufferSubData(GL_ARRAY_BUFFER,OpenGL_BufferInsert_Shift,m_OpenGLBuffers[i].vbo_size,m_Buffers[i].Data.data());
            OpenGL_BufferInsert_Shift += m_OpenGLBuffers[i].vbo_size;

            m_OpenGLBuffers[i].vbo_state = true;
        }
    }

    if (IndicesCount)
    {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_OpenGL_VBO_indices);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, getIndicesBufferSize(), getIndicesBufferData(), OpenGL_BufferUsage);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    m_OpenGL_VBO_verticles_count = VertexCount;
    m_OpenGL_VBO_indices_count   = IndicesCount;
}
//------------------------------------------------------------------------------
void COpenGLMeshBuffer::build_vao()
{

    //Recreate VAO
    glDeleteVertexArrays(1, &m_OpenGL_VAO);
    glGenVertexArrays(1,&m_OpenGL_VAO);

    //Setup VAO
	glBindVertexArray(m_OpenGL_VAO);

    glBindBuffer(GL_ARRAY_BUFFER, m_OpenGL_VBO_verticles);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_OpenGL_VBO_indices);

    GLsizeiptr OpenGL_Buffer_offset = 0;
    u32 OpenGL_BuffersCount = m_OpenGLBuffers.size();
    for(u32 i = 0; i < OpenGL_BuffersCount; i++)
    {
        const SVertexBufferFormat& Format = m_Buffers[i].Format;
        glEnableVertexAttribArray(Format.id);
        glVertexAttribPointer(Format.id,Format.size,to_opengl_type(Format.type),GL_FALSE,0,(void*)OpenGL_Buffer_offset);
        OpenGL_Buffer_offset += m_OpenGLBuffers[i].vbo_size;
    }
	glBindVertexArray(0);
}
//------------------------------------------------------------------------------
u32 COpenGLMeshBuffer::to_opengl_type(u32 ne_type)
{
    switch (ne_type)
    {
    case NTYPE_s8:
        return GL_BYTE;
    case NTYPE_s16:
        return GL_SHORT;
    case NTYPE_s32:
        return GL_INT;
    case NTYPE_u8:
        return GL_UNSIGNED_BYTE;
    case NTYPE_u16:
        return GL_UNSIGNED_SHORT;
    case NTYPE_u32:
        return GL_UNSIGNED_INT;
    case NTYPE_f32:
        return GL_FLOAT;
    case NTYPE_f64:
        return GL_DOUBLE;
    }
    return 0;
}
//------------------------------------------------------------------------------
}
}
