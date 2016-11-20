#include "COpenGLShaderProgramLibrary.h"

namespace novaengine
{
	namespace renderer
	{

		const char* Default_GLSL_VertexShader_Source =
			"#version 330\n"
			"layout (location = 0) in vec3 ne_vertex;\n"
			"layout (location = 5) in vec2 ne_texcord;\n"
			"uniform mat4  ne_mvpmatrix;\n"
			"out vec2 texCoord;\n"
			"void main()\n"
			"{\n"
			"gl_Position = ne_mvpmatrix * vec4(ne_vertex,1.0);\n"
			"texCoord    = ne_texcord;\n"
			"}\n";

		const char* Default_GLSL_FragmentShader_Source =
			"#version 330\n"
			"uniform sampler2D ne_texture2d;\n"
			"in  vec2 texCoord;\n"
			"out vec4 outColor;\n"
			"void main()\n"
			"{\n"
			"  outColor = texture(ne_texture2d,texCoord);\n"
			"}\n";

		COpenGLShaderProgramLibrary::COpenGLShaderProgramLibrary(IRenderer* p_OGLRenderer):
			m_OGLRenderer(p_OGLRenderer)
		{

		}
		//-------------------------------------------------------------------------------------------
		COpenGLShaderProgramLibrary::~COpenGLShaderProgramLibrary()
		{
			for (auto&& Prog : m_ProgramsCache)
				Prog.second->release();
		}
		//-------------------------------------------------------------------------------------------
		bool COpenGLShaderProgramLibrary::initShaderPrograms()
		{
			IShaderProgram* default_Program = m_OGLRenderer->createShaderProgram();
			default_Program->setVertexShaderSource(Default_GLSL_VertexShader_Source);
			default_Program->setFragmentShaderSource(Default_GLSL_FragmentShader_Source);
			if (!default_Program->build())	return false;
			m_ProgramsCache.insert(std::map<u32, IShaderProgram*>::value_type(ERMF_DEFAULT, default_Program));

			return true;
		}
		//-------------------------------------------------------------------------------------------
		IShaderProgram* COpenGLShaderProgramLibrary::getShaderProgram(u32 shader_features)
		{
			auto it = m_ProgramsCache.find(shader_features);
			if (it != m_ProgramsCache.end())
				return it->second;
			return nullptr;
		}
		//-------------------------------------------------------------------------------------------
	}
}