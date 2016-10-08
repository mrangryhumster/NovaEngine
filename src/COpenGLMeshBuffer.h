#ifndef COPENGLMeshBuffer_H
#define COPENGLMeshBuffer_H

#include "ne_types.h"

#include "IRenderer.h"
#include "CMeshBuffer.h"

#include "NLogger.h"

#include "GL/glew.h"

namespace novaengine
{
namespace renderer
{

struct SOpenGLVertexBuffer
{
    SOpenGLVertexBuffer():vbo_size(0),vbo_state(0){}
    size_t vbo_size;
    u32    vbo_state;
};

class COpenGLMeshBuffer : public CMeshBuffer
{
public:
    COpenGLMeshBuffer();
    virtual ~COpenGLMeshBuffer();

    u32 getVertexCount();
    u32 getIndicesCount();

    s32   createBuffer(u32 buffer_uid,u32 buffer_hint);
    void  deleteBuffer(u32 buffer_uid);

    size_t getBufferSize(u32 buffer);

    void bind_buffer();
    void unbind_buffer();

    void lock(bool edit_only);
    void unlock();

    void update();
	void draw();

    const u32& getBufferedVertexCount();
    const u32& getBufferedIndicesCount();

protected:
private:

    void clear_native_buffers();

    void create_vbo();
    void delete_vbo();

    void build_vbo();
    void build_vao();

    u32 m_CurrentMapping;

    std::vector<SOpenGLVertexBuffer> m_OpenGLBuffers;

    u32 m_OpenGL_VBO_verticles;
    u32 m_OpenGL_VBO_verticles_count;
    u32 m_OpenGL_VBO_indices;
    u32 m_OpenGL_VBO_indices_count;

    u32 m_OpenGL_VAO;

    u32 to_opengl_type(u32 ne_type);


};

}
}
#endif // COPENGLMeshBuffer_H
