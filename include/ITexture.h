#ifndef ITEXTURE_H
#define ITEXTURE_H

#include "ne_types.h"
#include "Resource.h"
#include "IImage.h"
#include "STextureParameters.h"

namespace novaengine
{
namespace renderer
{

class ITexture : public Resource
{
public:

    virtual void setTextureParameter(u32 Parameter,u32 value) = 0;
    virtual u32  getTextureParameter(u32 Parameter) = 0;

    virtual core::dim2u getTextureDimension() = 0;

    virtual void bind() = 0;
    virtual IImage* lock() = 0;
    virtual void unlock()  = 0;

protected:
private:
};

}
}


#endif // ITEXTURE_H
