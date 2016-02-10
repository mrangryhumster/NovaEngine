#ifndef COPENGLTEXTURE_H
#define COPENGLTEXTURE_H


#include "NovaEngine.h"

#include "NTypes.h"
#include "ITexture.h"
#include "STextureParameters.h"

#include "renderer/opengl/COpenGLRenderer.h"
#include "renderer/CImage.h"

#include <GL/gl.h>

namespace novaengine
{
namespace renderer
{

class COpenGLTexture : public ITexture
{
public:
    COpenGLTexture(IRenderer* OGLRenderer,IImage* Image,STextureParameters TextureParameters);
    virtual ~COpenGLTexture();

    void setTextureParameter(u32 Parameter,u32 value);
    u32  getTextureParameter(u32 Parameter);
    core::dim2u getTextureDimension();

    void bind();
    IImage* lock();
    void unlock();

    GLuint getTexture();



protected:
private:
    IRenderer* OpenGLRenderer;

    STextureParameters  TextureParameters;
    core::dim2u         TextureDimension;
    GLuint              GLTexture;

    IImage* TextureImg; //!< Fills by using lock(), need to get pixles pixels of the current texture.Will be delete after call unlock().
};

}
}



#endif // COPENGLTEXTURE_H
