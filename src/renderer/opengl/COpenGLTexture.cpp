#include "COpenGLTexture.h"

namespace novaengine
{
namespace renderer
{


COpenGLTexture::COpenGLTexture(IRenderer* OGLRenderer_,IImage* Image,STextureParameters TextureParameters_):
    TextureImg(NULL)
{
    setObjectName("texture");
    setResourceName(Image->getResourceName());

    OpenGLRenderer = OGLRenderer_;
    TextureParameters= TextureParameters_;
    GLTexture = 0;
    glGenTextures(1,&GLTexture);
    glBindTexture(GL_TEXTURE_2D,GLTexture);

    GLenum GL_PixelFormat;
    GLenum GL_InternalFormat;
    u32 PixelFormat = Image->getPixelFormat().PixelFormat;

    switch(PixelFormat)
    {
    case EPF_R8G8B8:
        GL_PixelFormat      = GL_RGB;
        GL_InternalFormat   = GL_RGBA;
        break;

    case EPF_R8G8B8A8:
        GL_PixelFormat      = GL_RGBA;
        GL_InternalFormat   = GL_RGBA;
        break;

    case EPF_B8G8R8:
        GL_PixelFormat      = GL_BGR;
        GL_InternalFormat   = GL_RGBA;
        break;

    case EPF_B8G8R8A8:
        GL_PixelFormat      = GL_BGRA;
        GL_InternalFormat   = GL_RGBA;
        break;
    case EPF_DEPTH:
        GL_PixelFormat      = GL_DEPTH_COMPONENT;
        GL_InternalFormat   = GL_DEPTH_COMPONENT;
        break;
    }



    if(TextureParameters_.Generate_mipmaps)
        gluBuild2DMipmaps(GL_TEXTURE_2D,GL_InternalFormat,Image->getImageDimension().width ,Image->getImageDimension().height,GL_PixelFormat,GL_UNSIGNED_BYTE,Image->lock());
    else
        glTexImage2D(GL_TEXTURE_2D,0,GL_InternalFormat,Image->getImageDimension().width ,Image->getImageDimension().height,0,GL_PixelFormat,GL_UNSIGNED_BYTE,Image->lock());

    setTextureParameter(ETP_MAG_FILTER,TextureParameters.Texture_mag_filter);
    setTextureParameter(ETP_MIN_FILTER,TextureParameters.Texture_min_filter);
    setTextureParameter(ETP_WRAP_X,TextureParameters.Texture_wrap_x);
    setTextureParameter(ETP_WRAP_Y,TextureParameters.Texture_wrap_y);

    OpenGLRenderer->bindTexture(NULL,0);

    TextureDimension = Image->getImageDimension();
}
//-------------------------------------------------------------------------------------------
COpenGLTexture::~COpenGLTexture()
{
    glDeleteTextures(1,&GLTexture);

    if(TextureImg)
        TextureImg->destroy();
}
//-------------------------------------------------------------------------------------------
void COpenGLTexture::setTextureParameter(u32 Parameter,u32 value)
{
    OpenGLRenderer->bindTexture(this,0);
    switch(Parameter)
    {
    case ETP_MAG_FILTER:
    case ETP_MIN_FILTER:
    {
        GLenum gl_param_name = (Parameter == ETP_MAG_FILTER) ? GL_TEXTURE_MAG_FILTER : GL_TEXTURE_MIN_FILTER;
        GLenum gl_value = 0;

        switch(value)
        {
        case ETPF_LINEAR:
            gl_value = GL_LINEAR;
            break;
        case ETPF_NEAREST:
            gl_value = GL_NEAREST;
            break;
        case ETPF_LINEAR_MIPMAP_LINEAR:
            gl_value = GL_LINEAR_MIPMAP_LINEAR;
            break;
        case ETPF_LINEAR_MIPMAP_NEAREST:
            gl_value = GL_LINEAR_MIPMAP_NEAREST;
            break;
        case ETPF_NEAREST_MIPMAP_LINEAR:
            gl_value = GL_NEAREST_MIPMAP_LINEAR;
            break;
        case ETPF_NEAREST_MIPMAP_NEAREST:
            gl_value = GL_NEAREST_MIPMAP_NEAREST;
            break;
        default:
            gl_value = GL_LINEAR;
            value    = ETPF_LINEAR;
        }

        glTexParameteri(GL_TEXTURE_2D,gl_param_name,gl_value);

        (Parameter == ETP_MAG_FILTER)?
        TextureParameters.Texture_mag_filter = value:
                                               TextureParameters.Texture_min_filter = value;
    }
    break;
    case ETP_WRAP_X:
    case ETP_WRAP_Y:
    {
        GLenum gl_param_name = (Parameter == ETP_WRAP_X) ? GL_TEXTURE_WRAP_S : GL_TEXTURE_WRAP_T;
        GLenum gl_value = (value == ETPW_NOT_REPEAT) ? GL_CLAMP : GL_REPEAT;
        glTexParameteri(GL_TEXTURE_2D,gl_param_name,gl_value);

        if(value!=ETPW_NOT_REPEAT && value != ETPW_REPEAT)
            value = ETPW_REPEAT;

        (Parameter == ETP_WRAP_X)?
        TextureParameters.Texture_wrap_x = value:
                                           TextureParameters.Texture_wrap_y = value;
    }
    break;
    }
}
//-------------------------------------------------------------------------------------------
u32  COpenGLTexture::getTextureParameter(u32 Parameter)
{
    switch(Parameter)
    {
    case ETP_MAG_FILTER:
        return TextureParameters.Texture_mag_filter;
    case ETP_MIN_FILTER:
        return TextureParameters.Texture_min_filter;
    case ETP_WRAP_X:
        return TextureParameters.Texture_wrap_x;
    case ETP_WRAP_Y:
        return TextureParameters.Texture_wrap_y;
    case ETP_GENERATE_MIPMAPS:
        return TextureParameters.Generate_mipmaps;
    default:
        return 0;
    }
}
//-------------------------------------------------------------------------------------------
core::dim2u COpenGLTexture::getTextureDimension()
{
    return TextureDimension;
}
void COpenGLTexture::bind()
{
    getRenderer()->bindTexture(this,0);
}
IImage* COpenGLTexture::lock()
{
    if(TextureImg != NULL)
        return TextureImg;

    u8* pixels = new u8[TextureDimension.width * TextureDimension.width * 4]; //!< 4 - because we using RGBA format 1 byte per color,include alpha....

    glBindTexture(GL_TEXTURE_2D,GLTexture);
    glGetTexImage(GL_TEXTURE_2D,0,GL_RGBA,GL_UNSIGNED_BYTE,pixels);

    TextureImg = new CImage(EPF_R8G8B8A8,TextureDimension,pixels);
    delete[] pixels;
    return TextureImg;
}
//-------------------------------------------------------------------------------------------
void COpenGLTexture::unlock()
{
    if(TextureImg == NULL)
        return;
    glBindTexture(GL_TEXTURE_2D,GLTexture);
    glTexSubImage2D(GL_TEXTURE_2D,0,0,0,TextureDimension.width,TextureDimension.height,GL_RGBA,GL_UNSIGNED_BYTE,TextureImg->lock());

    TextureImg->destroy();
    TextureImg = NULL;
}
//-------------------------------------------------------------------------------------------
GLuint COpenGLTexture::getTexture()
{
    return GLTexture;
}
//-------------------------------------------------------------------------------------------
}
}
