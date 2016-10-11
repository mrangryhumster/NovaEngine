#pragma once

#include <unordered_map>
#include <vector>

#include "IRenderer.h"
#include "IShaderProgram.h"
#include "NLogger.h"
#include <GL/glew.h>

namespace novaengine
{
	namespace renderer
	{
		enum E_OPENGL_PROGRAM_CONSTANTS
		{
			EOGL_PC_MVPMATRIX,
			EOGL_PC_PROJECTIONMATRIX,
			EOGL_PC_VIEWMATRIX,
			EOGL_PC_MODELMATRIX,
			EOGL_PC_APPLY	//reserved,
		};

		class COpenGLShaderProgram : public IShaderProgram
		{
		public:
			COpenGLShaderProgram(IRenderer* p_OGLRenderer);
			virtual ~COpenGLShaderProgram();

			void setVertexShaderSource(const char*);
			void setFragmentShaderSource(const char*);

			bool build();

			u32                       getUniformInfoCount();
			const SShaderUniformInfo* getUniformsInfo();

			s32 getUniformLocation(const char* UniformName);

			void setUniform(s32 uniform_location, u32 uniform_type, u32 uniform_count, const void* data);
			void setUniform(const char* uniform_name, u32 uniform_type, u32 uniform_count, const void* data);

			void bind();

			u32 getLastError();

			//OpenGLShaderProgram public:
			u32 getProgramID();

			void clearConstantLocations();
			void setConstant(u32 constant_type, const void* constant_data);

		protected:
		private:

			void update_uniform_info();
			void shader_log(GLuint source);


			IRenderer* m_OGLRenderer;

			// NOTE (mrang#1#): Maybe map with const char* ?
			std::unordered_map<std::string, GLint> m_UniformNames;
			std::vector<SShaderUniformInfo>        m_Uniforms;

			//Constant locations
			s32 m_ProgramConstant_location_mvpmatrix;
			s32 m_ProgramConstant_location_projectionmatrix;
			s32 m_ProgramConstant_location_viewmatrix;
			s32 m_ProgramConstant_location_modelmatrix;


			//OpenGL
			GLuint m_GLVertexShader;
			GLuint m_GLFragmentShader;
			GLuint m_GLProgram;

			//depricated
			u32 m_LastError;

		};

	}
}
