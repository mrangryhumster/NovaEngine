#ifndef IMATERIAL_H
#define IMATERIAL_H

#include "Resource.h"
#include "IShaderProgram.h"
#include "ITexture.h"

namespace novaengine
{
namespace renderer
{

class IMaterial : public Resource
{
public:

	virtual void setTexture(ITexture* texture,u32 texture_unit) = 0;

	//not recomended to use size = 0 in this case will be used size = uniform_size that may throw error if data[size] < uniform_size
	virtual void setValue(const std::string uniform_name    , const void* data, const size_t size = 0) = 0;
	virtual void setValue(const s32         uniform_location, const void* data, const size_t size = 0) = 0;

    virtual void            setShaderProgram(IShaderProgram*) = 0;
    virtual IShaderProgram* getShaderProgram() = 0;
	
	virtual void build() = 0;
    virtual void bind() = 0;

	
protected:
private:
};

}
}
#endif // IMATERIAL_H
