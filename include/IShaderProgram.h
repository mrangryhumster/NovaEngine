#ifndef ISHADERPROGRAM_H
#define ISHADERPROGRAM_H

#include "Object.h"


namespace novaengine
{
namespace renderer
{

enum E_SHADER_UNIFORM_TYPE
{
	//new
	ESUT_FLOAT1,
	ESUT_FLOAT2,
	ESUT_FLOAT3,
	ESUT_FLOAT4,

	ESUT_INT1,
	ESUT_INT2,
	ESUT_INT3,
	ESUT_INT4,

	ESUT_FLOAT_MAT2,
	ESUT_FLOAT_MAT3,
	ESUT_FLOAT_MAT4,
	
	ESUT_TEXTURE_1D,
	ESUT_TEXTURE_2D,
	ESUT_TEXTURE_3D,
};

struct SShaderUniformInfo
{
	SShaderUniformInfo(const std::string p_name,const s32  p_location,const u32 p_count,const u32 p_type,const size_t p_size) :
		name(p_name),location(p_location), type(p_type), count(p_count), size(p_size) {}
	const std::string  name;
	const s32		   location;
	const u32	       type;
	const u32	       count;
	const size_t       size;
};

class IShaderProgram : public Object
{
public:
	//--------------------
    virtual void setVertexShaderSource(const char* )    = 0;
    virtual void setFragmentShaderSource(const char*)   = 0;
	virtual bool build() = 0;
	//--------------------
	virtual u32                       getUniformInfoCount() = 0;
	virtual const SShaderUniformInfo* getUniformsInfo() = 0;
	//--------------------
    virtual s32  getUniformLocation(const char* UniformName) = 0;

	//universal
	virtual void setUniform(s32 uniform_location,u32 uniform_type,u32 uniform_count,const void* data) = 0;
	virtual void setUniform(const char* uniform_name, u32 uniform_type, u32 uniform_count, const void* data) = 0;
	
	//--------------------
    virtual void bind() = 0;
	
	//depricated
	//TODO remove getLastError	
    virtual u32 getLastError() = 0;

protected:
private:
};

}
}
#endif // ISHADERPROGRAM_H
