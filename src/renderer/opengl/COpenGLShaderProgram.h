#ifndef COPENGLSHADERPROGRAM_H
#define COPENGLSHADERPROGRAM_H

#include "NovaEngine.h"

#include "IShaderProgram.h"
#include "NLog.h"
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

    void setUniform_Texture         (const char* UniformName,u32 TextureNumber);
    void setUniform_Texture_Array   (const char* UniformName,u32 count,u32* TextureNumbers);

    void setUniform_Matrix          (const char* UniformName,core::matrixf matrix);
    void setUniform_Matrix_Array    (const char* UniformName,u32 count,core::matrixf* matrices);

    void setUniform_f(const char* UniformName,f32,f32,f32,f32);
    void setUniform_f(const char* UniformName,f32,f32,f32);
    void setUniform_f(const char* UniformName,f32,f32);
    void setUniform_f(const char* UniformName,f32);
    void setUniformArray_f(const char* UniformName,u32 count,u32 format,f32* array);

    void setUniform_s(const char* UniformName,s32,s32,s32,s32);
    void setUniform_s(const char* UniformName,s32,s32,s32);
    void setUniform_s(const char* UniformName,s32,s32);
    void setUniform_s(const char* UniformName,s32);
    void setUniformArray_s(const char* UniformName,u32 count,u32 format,s32* array);

    //New api
    virtual s32  getUniformLocation(const char* UniformName);

    virtual void bindUniform_TextureUnit(s32 uniform_location,u32 TextureUnit);
    virtual void bindUniform_s32(s32 uniform_location,u32 data_count,u32 uniform_format,s32* data);
    virtual void bindUniform_u32(s32 uniform_location,u32 data_count,u32 uniform_format,u32* data);
    virtual void bindUniform_f32(s32 uniform_location,u32 data_count,u32 uniform_format,f32* data);

    void bind();

    u32 getProgramID();

protected:
private:
    void shader_log(GLuint source);

    GLuint VertexShader;
    GLuint FragmentShader;
    GLuint Program;

};

}
}

#endif // COPENGLSHADERPROGRAM_H
