#ifndef CSCENESPRITE_H
#define CSCENESPRITE_H

#include "ISceneManager.h"
#include "ISceneSprite.h"

namespace novaengine
{
namespace scene
{

class CSceneSprite : public ISceneSprite
{
public:

    CSceneSprite(ISceneManager* smgr,renderer::ITexture* SpriteTexture = nullptr,core::rectf Source = core::rectf(0,0,1,1),ISceneNode* parent = 0,s32 ID = -1,core::vector3f position = core::vector3f(0,0,0),core::vector3f rotation= core::vector3f(0,0,0),core::vector3f scale= core::vector3f(1,1,1)):
        ISceneSprite(smgr,parent,ID,position,rotation,scale)
    {
        setTexture(SpriteTexture);
        setTextureSourceRect(Source);
    }
    virtual ~CSceneSprite()
    {}

    void setTexture(renderer::ITexture* _Texture)
    {
        Texture = _Texture;
        rebuild_sprite();
    }
    renderer::ITexture* getTexture()
    {
        return Texture;
    }

    void setTextureSourceRect(core::rectf rect)
    {
        TextureSourceRect = rect;
        rebuild_sprite();
    }
    core::rectf getTextureSourceRect()
    {
        return TextureSourceRect;
    }

    s32 getNodeType()
    {
        return ENT_SPRITE;
    }

    void render()
    {
        renderer::IRenderer* Renderer = SceneManager->getRenderer();

        if(!isVisible())
            return;

        Renderer->setTransform(getAbsoluteTransformation(),renderer::EMT_MODEL);
        Renderer->bindTexture(Texture,0);

        Renderer->drawArrays(0,4,0,sprite_verticles,sprite_texcoords,0,0,renderer::EPT_TRIANGLE_STRIP);
    }

protected:
private:
    void rebuild_sprite()
    {
            float texture_width_half  = 0;
            float texture_height_half = 0;

            if(Texture)
            {
                texture_width_half  = (float)(Texture->getTextureDimension().width ) /2;
                texture_height_half = (float)(Texture->getTextureDimension().height) /2;
            }
            sprite_verticles[0] = core::vector3f( -texture_width_half, -texture_height_half, 0);
            sprite_verticles[1] = core::vector3f( +texture_width_half, -texture_height_half, 0);
            sprite_verticles[2] = core::vector3f( -texture_width_half, +texture_height_half, 0);
            sprite_verticles[3] = core::vector3f( +texture_width_half, +texture_height_half, 0);

            sprite_texcoords[0] = core::vector2f(TextureSourceRect.X1,TextureSourceRect.Y1);
            sprite_texcoords[1] = core::vector2f(TextureSourceRect.X2,TextureSourceRect.Y1);
            sprite_texcoords[2] = core::vector2f(TextureSourceRect.X1,TextureSourceRect.Y2);
            sprite_texcoords[3] = core::vector2f(TextureSourceRect.X2,TextureSourceRect.Y2);
    }

    renderer::ITexture* Texture;
    core::rectf TextureSourceRect;

    core::vector3f sprite_verticles[4];
    core::vector2f sprite_texcoords[4];



};

}
}


#endif // CSCENESPRITE_H
