#ifndef COPENGLMeshBuffer_H
#define COPENGLMeshBuffer_H

#include "NovaEngine.h"

#include "IRenderer.h"
#include "CMeshBuffer.h"

#include "NLogger.h"

#include "GL/glew.h"

namespace novaengine
{
namespace renderer
{

class COpenGLMeshBuffer : public CMeshBuffer
{
public:
    COpenGLMeshBuffer();
    virtual ~COpenGLMeshBuffer();
	
	u32 getVertexCount();
	u32 getIndicesCount();

	void bind_buffer();
	void unbind_buffer();

	void update();

	u32 getBufferedVertexCount();
	u32 getBufferedIndicesCount();

private:

	void clear_native_buffers();
	void create_vbo();
	void delete_vbo();
	void build_vbo();
	void build_vao();

	u32 CurrentMapping;

	u32 OpenGL_VBO_verticles;
	u32 OpenGL_VBO_verticles_count;
	u32 OpenGL_VBO_indices;
	u32 OpenGL_VBO_indices_count;
	u32 OpenGL_VAO;

	u32 to_opengl_type(u32 ne_type);


};

}
}
#endif // COPENGLMeshBuffer_H
