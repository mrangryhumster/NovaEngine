#ifndef ISHADERPROGRAM_H
#define ISHADERPROGRAM_H

#include "Object.h"
#include "NExTypes.h"

namespace novaengine
{
namespace renderer
{
enum E_SHADER_UNIFORM_FORMAT
{
    ESUF_VEC_1,
    ESUF_VEC_2,
    ESUF_VEC_3,
    ESUF_VEC_4,
    ESUF_MAT_3,
    ESUF_MAT_4,
};

class IShaderProgram : public Object
{
public:

    virtual void setVertexShaderSource(const char* )    = 0;
    virtual void setFragmentShaderSource(const char*)   = 0;

    virtual bool compile() = 0;


    virtual s32  getUniformLocation(const char* UniformName) = 0;

    virtual void bindUniform_TextureUnit(s32 uniform_location,u32 TextureUnit) = 0;

    virtual void bindUniform_s32(s32 uniform_location,u32 data_count,u32 uniform_format,s32* data) = 0;
    virtual void bindUniform_u32(s32 uniform_location,u32 data_count,u32 uniform_format,u32* data) = 0;
    virtual void bindUniform_f32(s32 uniform_location,u32 data_count,u32 uniform_format,f32* data) = 0;


    virtual void bind() = 0;

    virtual u32 getLastError() = 0;

protected:
private:
};

}
}
#endif // ISHADERPROGRAM_H
