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

	size_t getBufferSize(u32 buffer);

	void bind_buffer();
	void unbind_buffer();

	void update();

	const u32& getBufferedVertexCount();
	const u32& getBufferedIndicesCount();

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

	//if no VAO using whis as shift in buffer
	size_t VBO_buffer_positions_size;
	size_t VBO_buffer_normals_size;
	size_t VBO_buffer_binormals_size;
	size_t VBO_buffer_tangents_size;
	size_t VBO_buffer_colors_size;
	size_t VBO_buffer_texcoords_size;
	size_t VBO_buffer_custom_size;


	u32 to_opengl_type(u32 ne_type);


};

}
}
#endif // COPENGLMeshBuffer_H
