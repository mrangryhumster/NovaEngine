#ifndef CMATERIAL_H
#define CMATERIAL_H

#include "NovaEngine.h"

#include "NTypes.h"
#include "IMaterial.h"
#include "ITexture.h"

namespace novaengine
{
namespace renderer
{

class CMaterial : public IMaterial
{
public:
    CMaterial():
        AmbientColor(1,1,1,1),
        DiffuseColor(1,1,1,1),
        SpecularColor(1,1,1,1)
    {
        setType(ERMT_DEFAULT);


        for(u32 i = 0; i < EMTN_TEXTURE_COUNT; i++)
            MaterialTextureList[i] = nullptr;


    }
    virtual ~CMaterial()
    {
        for(u32 i = 0; i < EMTN_TEXTURE_COUNT; i++)
                if(MaterialTextureList[i])
                    MaterialTextureList[i]->release();
    }

    void  setTexture(ITexture* NewTexture,u32 texture_id = 0)
    {
        ITexture* Texture = MaterialTextureList[texture_id];

        if(Texture)
            Texture->release();
        if(NewTexture)
            NewTexture->capture();

        MaterialTextureList[texture_id] = NewTexture;

    }
    ITexture*   getTexture(u32 texture_id = 0)
    {
        return MaterialTextureList[texture_id];
    }

    void  setAmbientColor(core::color4f Color)
    {
        AmbientColor = Color;
    }
    core::color4f   getAmbientColor()
    {
        return AmbientColor;
    }

    void  setDiffuseColor(core::color4f Color)
    {
        DiffuseColor = Color;
    }
    core::color4f   getDiffuseColor()
    {
        return DiffuseColor;
    }

    void  setSpecularColor(core::color4f Color)
    {
        SpecularColor = Color;
    }
    core::color4f   getSpecularColor()
    {
        return SpecularColor;
    }

    void setShininess(f32 p_Shininess)
    {
        Shininess = p_Shininess;
    }

    f32 getShininess()
    {
        return Shininess;
    }

    void setType(u32 newType)
    {
        MaterialType = newType;
    }
    u32 getType()
    {
        return MaterialType;
    }

    void bind()
    {
        getRenderer()->bindMaterial(this);
    }

protected:
private:
    u32 MaterialType;
    ITexture* MaterialTextureList[EMTN_TEXTURE_COUNT];
    core::color4f AmbientColor;
    core::color4f DiffuseColor;
    core::color4f SpecularColor;
    f32 Shininess;
};

}
}
#endif // CMATERIAL_H
