#ifndef COPENGLSHADERPROGRAM_H
#define COPENGLSHADERPROGRAM_H

#include "NovaEngine.h"

#include "IShaderProgram.h"
#include "NLogger.h"
#include <GL\glew.h>

namespace novaengine
{
namespace renderer
{

class COpenGLShaderProgram : public IShaderProgram
{
public:
    COpenGLShaderProgram();
    virtual ~COpenGLShaderProgram();

    void setVertexShaderSource(const char* );
    void setFragmentShaderSource(const char*);

    bool compile();

    virtual s32  getUniformLocation(const char* UniformName);

    virtual void bindUniform_TextureUnit(s32 uniform_location,u32 TextureUnit);
    virtual void bindUniform_s32(s32 uniform_location,u32 data_count,u32 uniform_format,s32* data);
    virtual void bindUniform_u32(s32 uniform_location,u32 data_count,u32 uniform_format,u32* data);
    virtual void bindUniform_f32(s32 uniform_location,u32 data_count,u32 uniform_format,f32* data);

    void bind();
    u32 getLastError();

    u32 getProgramID();

protected:
private:
    void shader_log(GLuint source);

    GLuint VertexShader;
    GLuint FragmentShader;
    GLuint Program;

    u32 LastError;

};

}
}

#endif // COPENGLSHADERPROGRAM_H
