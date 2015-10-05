#ifdef NE_OPENGLES1_RENDERER

#ifndef COpenGLES1MeshBuffer_H
#define COpenGLES1MeshBuffer_H

#include "NovaEngine.h"

#include "IRenderer.h"
#include "CMeshBuffer.h"

#include "log/NLogger.h"

#include <EGL/egl.h>
#include <GLES/gl.h>

namespace novaengine
{
namespace renderer
{

class COpenGLES1MeshBuffer : public CMeshBuffer
{
public:
    COpenGLES1MeshBuffer();
    virtual ~COpenGLES1MeshBuffer();

    u32 getVertexCount();

    void lock();
    void unlock();

    void bind_buffer();
    void unbind_buffer();

    bool Update();
    void render();

    u32 getVBO_VERTICLES_ID();
    u32 getVBO_INDICES_ID();
    u32 getVAO_ID();

protected:
private:

    bool is_vbo_suported();

    void init_vbo();
    void deinit_vbo();
    void build_vbo();
    void update_vao();

    u32 OpenGL_VBO_verticles_ID;
    u32 OpenGL_VBO_indices_ID;
    u32 OpenGL_VAO_ID;

    u32 OpenGL_VBO_Status;
    u32 OpenGL_VBO_Format;
    u32 OpenGL_VBO_VerticesCount;
};

}
}
#endif // COpenGLES1MeshBuffer_H

#endif // NE_OPENGLES1_RENDERER

