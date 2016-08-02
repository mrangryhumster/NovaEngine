#include "COpenGLMeshBuffer.h"

//warning undef GLEW_ARB_vertex_array_object
#undef GLEW_ARB_vertex_array_object
#define GLEW_ARB_vertex_array_object 0

namespace novaengine
{
namespace renderer
{
COpenGLMeshBuffer::COpenGLMeshBuffer():
	CurrentMapping(EMBMH_DEFAULT),
	OpenGL_VBO_verticles(0),
	OpenGL_VBO_verticles_count(0),
	OpenGL_VBO_indices(0),
	OpenGL_VBO_indices_count(0),
	OpenGL_VAO(0)
{

}
//-------------------------------------------------------------------------------------------
COpenGLMeshBuffer::~COpenGLMeshBuffer()
{
	if (CurrentMapping != EMBMH_DEFAULT)
		delete_vbo();
}

u32 COpenGLMeshBuffer::getVertexCount()
{
	if (CurrentMapping == EMBMH_DEFAULT)
		return CMeshBuffer::getVertexCount();
	return OpenGL_VBO_verticles_count;
}

u32 COpenGLMeshBuffer::getIndicesCount()
{
	if (CurrentMapping == EMBMH_DEFAULT)
		return CMeshBuffer::getIndicesCount();
	return OpenGL_VBO_indices_count;
}

size_t COpenGLMeshBuffer::getBufferSize(u32 buffer)
{
	if (CurrentMapping == EMBMH_DEFAULT)
		return CMeshBuffer::getBufferSize(buffer);

	switch (buffer)
	{
	case EVA_POSITION:
		return VBO_buffer_positions_size;
	case EVA_NORMAL:
		return VBO_buffer_normals_size;
	case EVA_BINORMAL:
		return VBO_buffer_binormals_size;
	case EVA_TANGENT:
		return VBO_buffer_tangents_size;
	case EVA_COLOR:
		return VBO_buffer_colors_size;
	case EVA_TEXCOORD:
		return VBO_buffer_texcoords_size;
	case EVA_CUSTOM:
		return VBO_buffer_custom_size;
	default: //in this case undefined behavior will begin =D
		return 0;
	}
}

void COpenGLMeshBuffer::update()
{
	UpdateRequired = false;

	if (CurrentMapping == MappingHint)
	{
		//--------------------------------------
		if (CurrentMapping != EMBMH_DEFAULT)
		{
			build_vbo();

			if (GLEW_ARB_vertex_array_object)
				build_vao();
		}
		//--------------------------------------
	}
	else if ((CurrentMapping == EMBMH_DEFAULT) && (MappingHint != EMBMH_DEFAULT))
	{
		//--------------------------------------
		if (GLEW_ARB_vertex_buffer_object)
		{
			create_vbo();
			build_vbo();
			if (GLEW_ARB_vertex_array_object)
				build_vao();
			CurrentMapping = MappingHint;
		}
		else
		{
			MappingHint = EMBMH_DEFAULT;
		}
		//--------------------------------------
	}
	else if ((CurrentMapping != EMBMH_DEFAULT) && (MappingHint == EMBMH_DEFAULT))
	{
		delete_vbo();
		CurrentMapping = MappingHint;
	}

	if ((CurrentMapping == EMBMH_VBO_STATIC) || (CurrentMapping == EMBMH_VBO_STREAM))
	{
		clear_native_buffers();
	}
}

void COpenGLMeshBuffer::bind_buffer()
{
	//!< bind VAO, if VAO not supported we bind VBO vertex and VBO indices
	if (GLEW_ARB_vertex_array_object)
	{
		glBindVertexArray(OpenGL_VAO);
	}
	else if (GLEW_ARB_vertex_buffer_object)
	{
		glBindBuffer(GL_ARRAY_BUFFER, OpenGL_VBO_verticles);
		if (OpenGL_VBO_indices)
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, OpenGL_VBO_indices);
	}
}

void COpenGLMeshBuffer::unbind_buffer()
{
	//!< unbind VAO AND VBO
	if (GLEW_ARB_vertex_array_object)
	{
		glBindVertexArray(0);
	}
	if (GLEW_ARB_vertex_buffer_object)
	{
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		if (OpenGL_VBO_indices)
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}
}

const u32& COpenGLMeshBuffer::getBufferedVertexCount()
{
	return OpenGL_VBO_verticles_count;
}

const u32& COpenGLMeshBuffer::getBufferedIndicesCount()
{
	return OpenGL_VBO_indices_count;
}

void COpenGLMeshBuffer::clear_native_buffers()
{
	Positions.clear();
	Normals.clear();
	Binormals.clear();
	Tangents.clear();
	Colors.clear();
	TexCoords.clear();
	CustomVerticles.clear();
	Indices.clear();

	Positions.shrink_to_fit();
	Normals.shrink_to_fit();
	Binormals.shrink_to_fit();
	Tangents.shrink_to_fit();
	Colors.shrink_to_fit();
	TexCoords.shrink_to_fit();
	CustomVerticles.shrink_to_fit();
	Indices.shrink_to_fit();

}

void COpenGLMeshBuffer::create_vbo()
{
	if (GLEW_ARB_vertex_buffer_object)
	{
		glGenBuffers(1, &OpenGL_VBO_verticles);
		if (Indices.size() > 0)
			glGenBuffers(1, &OpenGL_VBO_indices);
	}
	if (GLEW_ARB_vertex_array_object)
	{
		glGenVertexArrays(1, &OpenGL_VAO);
	}
}
void COpenGLMeshBuffer::delete_vbo()
{
	if (GLEW_ARB_vertex_array_object)
	{
		glDeleteVertexArrays(1, &OpenGL_VAO);
	}
	if (GLEW_ARB_vertex_buffer_object)
	{
		glDeleteBuffers(1, &OpenGL_VBO_verticles);
		if (!OpenGL_VBO_indices)
			glDeleteBuffers(1, &OpenGL_VBO_indices);
	}
}
void COpenGLMeshBuffer::build_vbo()
{
	u32 IndicesCount = getIndicesCount();
	u32 VertexCount  = getVertexCount();

	if (VertexCount == 0)
		return;

	u32 BufferSize = 0;
	u32 VertexFormatFlags = VertexFormat.getFlags();

	if (VertexFormatFlags & EVA_POSITION)
		BufferSize += getBufferSize(EVA_POSITION);
	if (VertexFormatFlags & EVA_NORMAL)
		BufferSize += getBufferSize(EVA_NORMAL);
	if (VertexFormatFlags & EVA_COLOR)
		BufferSize += getBufferSize(EVA_COLOR);
	if (VertexFormatFlags & EVA_TEXCOORD)
		BufferSize += getBufferSize(EVA_TEXCOORD);

	GLenum OpenGL_Buffer_Usage = GL_DYNAMIC_DRAW;

	if (CurrentMapping == EMBMH_VBO_STATIC || CurrentMapping == EMBMH_VBO_STATIC_COPY)
		OpenGL_Buffer_Usage = GL_STATIC_DRAW;


	glBindBuffer(GL_ARRAY_BUFFER, OpenGL_VBO_verticles);
	glBufferData(GL_ARRAY_BUFFER, BufferSize, 0, OpenGL_Buffer_Usage);

	u32 BufferInsertShift = 0;
	if (VertexFormatFlags & EVA_POSITION)
	{
		glBufferSubData(GL_ARRAY_BUFFER, BufferInsertShift, getBufferSize(EVA_POSITION), getBufferData(EVA_POSITION));
		BufferInsertShift += (VBO_buffer_positions_size = getBufferSize(EVA_POSITION));
	}
	if (VertexFormatFlags & EVA_NORMAL)
	{
		glBufferSubData(GL_ARRAY_BUFFER, BufferInsertShift, getBufferSize(EVA_NORMAL), getBufferData(EVA_NORMAL));
		BufferInsertShift += (VBO_buffer_normals_size = getBufferSize(EVA_NORMAL));
	}
	if (VertexFormatFlags & EVA_COLOR)
	{
		glBufferSubData(GL_ARRAY_BUFFER, BufferInsertShift, getBufferSize(EVA_COLOR), getBufferData(EVA_COLOR));
		BufferInsertShift += (VBO_buffer_colors_size = getBufferSize(EVA_COLOR));
	}
	if (VertexFormatFlags & EVA_TEXCOORD)
	{
		glBufferSubData(GL_ARRAY_BUFFER, BufferInsertShift, getBufferSize(EVA_TEXCOORD), getBufferData(EVA_TEXCOORD));
		BufferInsertShift += (VBO_buffer_texcoords_size = getBufferSize(EVA_TEXCOORD));
	}


	if (IndicesCount)
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, OpenGL_VBO_indices);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, getIndicesBufferSize(), getIndicesBufferData(), OpenGL_Buffer_Usage);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}
	glBindBuffer(GL_ARRAY_BUFFER, 0);


	OpenGL_VBO_verticles_count = VertexCount;
	OpenGL_VBO_indices_count   = IndicesCount;
}
void COpenGLMeshBuffer::build_vao()
{
	//------------------------------------------------------------
	bool have_verticles = (VertexFormat.getFlags() & EVA_POSITION) != 0;
	bool have_normals   = (VertexFormat.getFlags() & EVA_NORMAL)   != 0;
	bool have_colors    = (VertexFormat.getFlags() & EVA_COLOR)    != 0;
	bool have_texcoords = (VertexFormat.getFlags() & EVA_TEXCOORD) != 0;

	if (have_verticles == false)
		return;

	glBindVertexArray(OpenGL_VAO);

	glBindBuffer(GL_ARRAY_BUFFER, OpenGL_VBO_verticles);
	if(OpenGL_VBO_indices)
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, OpenGL_VBO_indices);


	u32 BufferShift = 0;
	if (have_verticles)
	{
		glEnableClientState(GL_VERTEX_ARRAY);
		glVertexPointer(3, to_opengl_type(VertexFormat.getAttributeFormat(EVA_POSITION)->type), 0, (void*)BufferShift);
		BufferShift += getBufferSize(EVA_POSITION);
	}
	if (have_normals)
	{
		glEnableClientState(GL_NORMAL_ARRAY);
		glNormalPointer(to_opengl_type(VertexFormat.getAttributeFormat(EVA_NORMAL)->type), 0, (void*)BufferShift);
		BufferShift += getBufferSize(EVA_NORMAL);
	}
	if (have_colors)
	{
		glEnableClientState(GL_COLOR_ARRAY);
		glColorPointer(4,to_opengl_type(VertexFormat.getAttributeFormat(EVA_COLOR)->type), 0, (void*)BufferShift);
		BufferShift += getBufferSize(EVA_COLOR);
	}
	if (have_texcoords)
	{
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		glTexCoordPointer(3, to_opengl_type(VertexFormat.getAttributeFormat(EVA_TEXCOORD)->type), 0, (void*)BufferShift);
		BufferShift += getBufferSize(EVA_TEXCOORD);
	}
	glBindVertexArray(0);
}

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
}
}
