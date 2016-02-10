#ifndef ISCENESPRITE_H
#define ISCENESPRITE_H

#include "IScene2DNode.h"

namespace novaengine
{
namespace scene
{

class ISceneSprite : public IScene2DNode
{
public:
    ISceneSprite(ISceneManager* smgr,ISceneNode* parent = 0,s32 ID = -1,core::vector3f position = core::vector3f(0,0,0),core::vector3f rotation= core::vector3f(0,0,0),core::vector3f scale= core::vector3f(1,1,1)):
        IScene2DNode(smgr,parent,ID,position,rotation,scale) {}

    virtual void setTexture(renderer::ITexture* texture) = 0;
    virtual renderer::ITexture* getTexture() = 0;

    virtual void setTextureSourceRect(core::rectf) = 0;
    virtual core::rectf getTextureSourceRect() = 0;

protected:
private:
};

}
}

#endif // ISCENESPRITE_H
