#ifndef IMATERIAL_H
#define IMATERIAL_H

#include "Resource.h"
#include "ITexture.h"

namespace novaengine
{
namespace renderer
{

enum E_RENDERER_MATERIAL_TYPE
{
    ERMT_DEFAULT = 0,
    ERMT_COUNT   = 1,
};
enum E_MATERIAL_TEXTURE_NUM
{
    EMTN_TEXTURE_0     = 0,
    EMTN_TEXTURE_1     = 1,
    EMTN_TEXTURE_2     = 2,
    EMTN_TEXTURE_3     = 3,
    EMTN_TEXTURE_4     = 4,
    EMTN_TEXTURE_5     = 5,
    EMTN_TEXTURE_6     = 6,
    EMTN_TEXTURE_7     = 7,
    EMTN_TEXTURE_COUNT = 8,
};
class IMaterial : public Resource
{
public:
    virtual void        setTexture(ITexture* texture,u32 texture_id = 0) = 0;
    virtual ITexture*   getTexture(u32 texture_id = 0) = 0;

    virtual void            setAmbientColor(core::color4f) = 0;
    virtual core::color4f   getAmbientColor()   = 0;
    virtual void            setDiffuseColor(core::color4f) = 0;
    virtual core::color4f   getDiffuseColor()   = 0;
    virtual void            setSpecularColor(core::color4f) = 0;
    virtual core::color4f   getSpecularColor()  = 0;
    virtual void            setShininess(f32)   = 0;
    virtual f32             getShininess()      = 0;

    virtual void setType(u32) = 0;
    virtual u32  getType()    = 0;

    virtual void bind() = 0;

protected:
private:
};

}
}
#endif // IMATERIAL_H
