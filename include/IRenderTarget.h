#ifndef IRENDERTARGET_H
#define IRENDERTARGET_H

#include "ERendererEnums.h"
#include "ITexture.h"
#include "Object.h"

namespace novaengine
{
namespace renderer
{

class IRenderTarget : public Object
{
public:
		virtual void setTexture(renderer::ITexture* Target,u32 AttachTo) = 0;
		
		virtual bool isOk() = 0;
private:
};

}
}
#endif
