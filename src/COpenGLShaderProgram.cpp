#include "COpenGLShaderProgram.h"

namespace novaengine
{
namespace renderer
{
const char* Default_VertexShader_Source =
    "varying vec2 Texcoord;\n"
    "void main() {\n"
    "  gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;\n"
    "Texcoord    = gl_MultiTexCoord0.xy;\n"
    "}\n";

const char* Default_FragmentShader_Source =
    "uniform sampler2D texture;\n"
    "varying vec2 Texcoord;"
    "void main() {\n"
    "  gl_FragColor = texture2D( texture  , Texcoord);\n"
    "}\n";

COpenGLShaderProgram::COpenGLShaderProgram(IRenderer* p_OGLRenderer):
    m_OGLRenderer(p_OGLRenderer),
    m_LastError(0)
{
    m_GLVertexShader     = glCreateShader(GL_VERTEX_SHADER);
    m_GLFragmentShader   = glCreateShader(GL_FRAGMENT_SHADER);
    m_GLProgram          = glCreateProgram();

    setVertexShaderSource(Default_VertexShader_Source);
    setFragmentShaderSource(Default_FragmentShader_Source);

}
//-------------------------------------------------------------------------------------------
COpenGLShaderProgram::~COpenGLShaderProgram()
{
    glDeleteShader(m_GLVertexShader);
    glDeleteShader(m_GLFragmentShader);
    glDeleteProgram(m_GLProgram);

	m_UniformNames.clear();
}
//-------------------------------------------------------------------------------------------
void COpenGLShaderProgram::setVertexShaderSource(const char* source)
{
    glShaderSource(m_GLVertexShader,1,&source,NULL);
}
//-------------------------------------------------------------------------------------------
void COpenGLShaderProgram::setFragmentShaderSource(const char* source)
{
    glShaderSource(m_GLFragmentShader,1,&source,NULL);
}
//-------------------------------------------------------------------------------------------
bool COpenGLShaderProgram::build()
{
    m_LastError = 0;
    //----------------------------------------------------------
    glCompileShaderARB(m_GLVertexShader);
    glCompileShaderARB(m_GLFragmentShader);
    //----------------------------------------------------------
    GLint compiled;
    glGetShaderiv(m_GLVertexShader, GL_COMPILE_STATUS, &compiled);
    if (compiled)
        LOG_DEBUG("Vertex shader successfully compiled\n");
    else
    {
        shader_log(m_GLVertexShader);
        m_LastError = 1;
        return 0;
    }
    //----------------------------------------------------------
    glGetShaderiv(m_GLFragmentShader, GL_COMPILE_STATUS, &compiled);
    if (compiled)
        LOG_DEBUG("Fragment shader successfully compiled\n");
    else
    {
        shader_log(m_GLFragmentShader);
        m_LastError = 2;
        return 0;
    }
    //----------------------------------------------------------
    glAttachShader(m_GLProgram,m_GLVertexShader);
    glAttachShader(m_GLProgram,m_GLFragmentShader);
    glLinkProgram(m_GLProgram);
    //----------------------------------------------------------
    GLint linked;
    glGetProgramiv(m_GLProgram, GL_LINK_STATUS, &linked);
    if (linked)
        LOG_DEBUG("Shader program successfully linked\n");
    else
    {
        shader_log(m_GLProgram);
        m_LastError = 3;
        return 0;
    }
    //----------------------------------------------------------
	update_uniform_info();
	//----------------------------------------------------------

    return 1;
}
//-------------------------------------------------------------------------------------------
u32 COpenGLShaderProgram::getUniformInfoCount()
{
	return m_Uniforms.size();
}
//-------------------------------------------------------------------------------------------
const SShaderUniformInfo* COpenGLShaderProgram::getUniformsInfo()
{
	return m_Uniforms.data();
}
//-------------------------------------------------------------------------------------------
s32  COpenGLShaderProgram::getUniformLocation(const char* UniformName)
{
	return glGetUniformLocation(m_GLProgram, UniformName);
	/*
    auto it = m_UniformNames.find(UniformName);
    if(it != m_UniformNames.end())
        return it->second;
    return -1;*/
}
//-------------------------------------------------------------------------------------------
void COpenGLShaderProgram::setUniform(s32 uniform_location, u32 uniform_type, u32 uniform_count, const void * data)
{
	switch (uniform_type)
	{
	case ESUT_TEXTURE_1D:
	case ESUT_TEXTURE_2D:
	case ESUT_TEXTURE_3D:
			glUniform1iv(uniform_location, uniform_count, static_cast<const GLint*>(data));
		break;
	case ESUT_FLOAT1:
			glUniform1fv(uniform_location, uniform_count, static_cast<const GLfloat*>(data));
		break;
	case ESUT_FLOAT2:
			glUniform2fv(uniform_location, uniform_count, static_cast<const GLfloat*>(data));
		break;
	case ESUT_FLOAT3:
			glUniform3fv(uniform_location, uniform_count, static_cast<const GLfloat*>(data));
		break;
	case ESUT_FLOAT4:
			glUniform4fv(uniform_location, uniform_count, static_cast<const GLfloat*>(data));
		break;
	case ESUT_FLOAT_MAT4:
			glUniformMatrix4fv(uniform_location, uniform_count, false, static_cast<const GLfloat*>(data));
		break;
	}
}
//-------------------------------------------------------------------------------------------
void COpenGLShaderProgram::setUniform(const char* uniform_name, u32 uniform_type, u32 uniform_count, const void* data)
{
	setUniform(getUniformLocation(uniform_name), uniform_type, uniform_count, data);
}
//-------------------------------------------------------------------------------------------
void COpenGLShaderProgram::bind()
{
    m_OGLRenderer->bindShaderProgram(this);
}
//-------------------------------------------------------------------------------------------
u32 COpenGLShaderProgram::getLastError()
{
    return m_LastError;
}
//-------------------------------------------------------------------------------------------
u32 COpenGLShaderProgram::getProgramID()
{
    return m_GLProgram;
}
//-------------------------------------------------------------------------------------------
void COpenGLShaderProgram::update_uniform_info()
{
	m_UniformNames.clear();

	//load uniforms info...
	s32 UniformCount, UniformMaxNameSize;
	glGetProgramiv(m_GLProgram, GL_ACTIVE_UNIFORMS, &UniformCount);
	glGetProgramiv(m_GLProgram, GL_ACTIVE_UNIFORM_MAX_LENGTH, &UniformMaxNameSize);
	//..if any exist
	if (UniformCount > 0)
	{
		GLchar* UniformNameBuf = new GLchar[UniformMaxNameSize];

		GLint  UniformSize, UniformLocation;
		GLenum UniformType;
		GLsizei UniformNameBufLen;

		for (int index = 0; index < UniformCount; index++)
		{
			glGetActiveUniform(m_GLProgram, 
				(GLuint)index, 
				UniformMaxNameSize, 
				&UniformNameBufLen, 
				&UniformSize, 
				&UniformType, 
				UniformNameBuf);

			UniformLocation = glGetUniformLocation(m_GLProgram, UniformNameBuf);

			//strip array index for nonstruct uniforms
			std::string UniformName(UniformNameBuf);
			size_t idx_epos = UniformName.find_last_of("]");
			if (idx_epos != UniformName.npos && idx_epos == (UniformName.size()-1) )
				UniformName.erase(idx_epos-2);

			u32 EngineUniformType = 0, EngineUniformSize = 0;
			//to novaengine type
			switch (UniformType)
			{
			case GL_FLOAT:
				EngineUniformType = ESUT_FLOAT1; EngineUniformSize =  4; break;
			case GL_FLOAT_VEC2:
				EngineUniformType = ESUT_FLOAT2; EngineUniformSize =  8; break;
			case GL_FLOAT_VEC3:
				EngineUniformType = ESUT_FLOAT3; EngineUniformSize = 12; break;
			case GL_FLOAT_VEC4:
				EngineUniformType = ESUT_FLOAT4; EngineUniformSize = 16; break;
			case GL_INT:
				EngineUniformType = ESUT_INT1;	 EngineUniformSize = 4; break;
			case GL_INT_VEC2:
				EngineUniformType = ESUT_INT2;   EngineUniformSize = 8; break;
			case GL_INT_VEC3:
				EngineUniformType = ESUT_INT3;   EngineUniformSize = 12; break;
			case GL_INT_VEC4:
				EngineUniformType = ESUT_INT4;   EngineUniformSize = 16; break;
			
			case GL_FLOAT_MAT2:
				EngineUniformType = ESUT_FLOAT_MAT2;   EngineUniformSize = 16; break;
			case GL_FLOAT_MAT3:
				EngineUniformType = ESUT_FLOAT_MAT3;   EngineUniformSize = 36; break;
			case GL_FLOAT_MAT4:
				EngineUniformType = ESUT_FLOAT_MAT4;   EngineUniformSize = 64; break;

			case GL_SAMPLER_1D:
				EngineUniformType = ESUT_TEXTURE_1D;   EngineUniformSize = 4; break;
			case GL_SAMPLER_2D:
				EngineUniformType = ESUT_TEXTURE_2D;   EngineUniformSize = 4; break;
			case GL_SAMPLER_3D:
				EngineUniformType = ESUT_TEXTURE_3D;   EngineUniformSize = 4; break;

			default:
				EngineUniformType = UniformType << 16; 
				break;
			}
			//GL_SAMPLER_2D

			m_UniformNames.insert(std::unordered_map<std::string, GLint>::value_type(UniformName, UniformLocation));
			m_Uniforms.push_back(SShaderUniformInfo(UniformName,UniformLocation,UniformSize,EngineUniformType,EngineUniformSize * UniformSize));
		}
		delete[] UniformNameBuf;
	}
	for (auto it = m_Uniforms.begin(); it != m_Uniforms.end(); it++)
		LOG_DEBUG("Located uniform \"%s\" loc: %d type: 0x%x count: %d datasize:%d\n", (*it).name.c_str(), (*it).location, (*it).type, (*it).count, (*it).size);
	//----------------------------------------------------------
}
//-------------------------------------------------------------------------------------------
void COpenGLShaderProgram::shader_log(GLuint source)
{
    int   LogLen   = 0;
    int   Len      = 0;

    glGetShaderiv(source, GL_INFO_LOG_LENGTH, &LogLen);

    if(LogLen > 1)
    {
        char* Log = new char[LogLen];
        glGetShaderInfoLog(source, LogLen, &Len, Log);
        LOG_ERROR("Shaders log : %s\n",Log);
        delete[] Log;
    }
}
//-------------------------------------------------------------------------------------------
}
}
