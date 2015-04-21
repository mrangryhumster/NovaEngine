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

COpenGLShaderProgram::COpenGLShaderProgram()
{
    VertexShader     = glCreateShader(GL_VERTEX_SHADER);
    FragmentShader   = glCreateShader(GL_FRAGMENT_SHADER);
    Program          = glCreateProgram();

    setVertexShaderSource(Default_VertexShader_Source);
    setFragmentShaderSource(Default_FragmentShader_Source);

}

COpenGLShaderProgram::~COpenGLShaderProgram()
{
    glDeleteShader(VertexShader);
    glDeleteShader(FragmentShader);
    glDeleteProgram(Program);
}

void COpenGLShaderProgram::setVertexShaderSource(const char* source)
{
    glShaderSource(VertexShader     ,1,&source,NULL);
}
void COpenGLShaderProgram::setFragmentShaderSource(const char* source)
{
    glShaderSource(FragmentShader   ,1,&source,NULL);
}

bool COpenGLShaderProgram::compile()
{
    glCompileShaderARB(VertexShader);
    glCompileShaderARB(FragmentShader);
    //----------------------------------------------------------
    GLint compiled;
    glGetShaderiv(VertexShader, GL_COMPILE_STATUS, &compiled);
    if (compiled)
        LOG_DEBUG("Vertex shader successfully compiled\n");
    else
    {
        shader_log(VertexShader);
        return 0;
    }
    //----------------------------------------------------------
    glGetShaderiv(FragmentShader, GL_COMPILE_STATUS, &compiled);
    if (compiled)
        LOG_DEBUG("Fragment shader successfully compiled\n");
    else
    {
        shader_log(FragmentShader);
        return 0;
    }
    //----------------------------------------------------------
    glAttachShader(Program,VertexShader);
    glAttachShader(Program,FragmentShader);
    glLinkProgram(Program);
    //----------------------------------------------------------
    GLint linked;
    glGetProgramiv(Program, GL_LINK_STATUS, &linked);
    if (linked)
        LOG_DEBUG("Shader program successfully linked\n");
    else
    {
        shader_log(Program);
        return 0;
    }
    //----------------------------------------------------------
    return 1;
}
s32  COpenGLShaderProgram::getUniformLocation(const char* UniformName)
{
    return glGetUniformLocation(Program,UniformName);
}
void COpenGLShaderProgram::bindUniform_TextureUnit(s32 uniform_location,u32 TextureUnit)
{
    glUniform1i(uniform_location,TextureUnit);
}
void COpenGLShaderProgram::bindUniform_s32(s32 uniform_location,u32 data_count,u32 uniform_format,s32* data)
{
    switch(uniform_format)
    {
    case ESUF_VEC_1:
        glUniform1iv(uniform_location,data_count,data);
        break;
    case ESUF_VEC_2:
        glUniform2iv(uniform_location,data_count,data);
        break;
    case ESUF_VEC_3:
        glUniform3iv(uniform_location,data_count,data);
        break;
    case ESUF_VEC_4:
        glUniform4iv(uniform_location,data_count,data);
        break;
    default:
        LOG_ENGINE_DEBUG("unsupported uniform_format\n");
    }
}
void COpenGLShaderProgram::bindUniform_u32(s32 uniform_location,u32 data_count,u32 uniform_format,u32* data)
{
    switch(uniform_format)
    {
    case ESUF_VEC_1:
        glUniform1uiv(uniform_location,data_count,data);
        break;
    case ESUF_VEC_2:
        glUniform2uiv(uniform_location,data_count,data);
        break;
    case ESUF_VEC_3:
        glUniform3uiv(uniform_location,data_count,data);
        break;
    case ESUF_VEC_4:
        glUniform4uiv(uniform_location,data_count,data);
        break;
    default:
        LOG_ENGINE_DEBUG("unsupported uniform_format\n");
    }
}
void COpenGLShaderProgram::bindUniform_f32(s32 uniform_location,u32 data_count,u32 uniform_format,f32* data)
{
   switch(uniform_format)
    {
    case ESUF_VEC_1:
        glUniform1fv(uniform_location,data_count,data);
        break;
    case ESUF_VEC_2:
        glUniform2fv(uniform_location,data_count,data);
        break;
    case ESUF_VEC_3:
        glUniform3fv(uniform_location,data_count,data);
        break;
    case ESUF_VEC_4:
        glUniform4fv(uniform_location,data_count,data);
        break;
    case ESUF_MAT_3:
        glUniformMatrix3fv(uniform_location,data_count,false,data);
        break;
    case ESUF_MAT_4:
        glUniformMatrix4fv(uniform_location,data_count,false,data);
        break;
    default:
        LOG_ENGINE_DEBUG("unsupported uniform_format\n");
    }
}
void COpenGLShaderProgram::bind()
{
    getRenderer()->bindShaderProgram(this);
}

void COpenGLShaderProgram::shader_log(GLuint source)
{
    int   LogLen   = 0;
    int   Len      = 0;
    char *Log;

    glGetShaderiv(source, GL_INFO_LOG_LENGTH, &LogLen);

    if(LogLen > 1)
    {
        Log = new char[LogLen];
        glGetShaderInfoLog(source, LogLen, &Len, Log);
        LOG_ERROR("Shaders log : %s\n",Log);
        delete[] Log;
    }
}

u32 COpenGLShaderProgram::getProgramID()
{
    return Program;
}

}
}
