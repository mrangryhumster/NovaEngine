#ifndef ISCENE2DNODE_H
#define ISCENE2DNODE_H

#include "Object.h"
#include "NTypes.h"

#include "ISceneNode.h"


namespace novaengine
{
namespace scene
{

class IScene2DNode : public ISceneNode
{
public:
    IScene2DNode(ISceneManager* smgr,ISceneNode* parent = 0,s32 ID = -1,core::vector3f position = core::vector3f(0,0,0),core::vector3f rotation= core::vector3f(0,0,0),core::vector3f scale= core::vector3f(1,1,1)):
    ISceneNode(smgr,parent,ID,position,rotation,scale) {}

    virtual ~IScene2DNode() {}

    //----------------------------------Position
    virtual void set2dPosition(core::vector2s v_position)
    {
        Position = core::vector3f((f32)v_position.x,(f32)v_position.y,0);
        update_transform = true;
    }
    virtual core::vector2s get2dPosition()
    {
        return core::vector2s((s32)Position.x,(s32)Position.y);
    }

    virtual core::vector2s get2dAbsolutePosition()
    {
        if(Parent)
        {
            core::vector3f parent_absolute_pos = Parent->getAbsolutePosition();
            return (get2dPosition() + core::vector2s((s32)parent_absolute_pos.x,(s32)parent_absolute_pos.y));
        }
        return get2dPosition();
    }

    virtual void setScreenPosition(core::vector2f screen_pos)
    {
        core::dim2u ScreenRes = SceneManager->getRenderer()->getViewport().dimension();
        core::vector3f newPos;
        newPos.x = screen_pos.x * (ScreenRes.width/2);
        newPos.y = screen_pos.y * (ScreenRes.height/2);
        setPosition(newPos);
    }

    virtual core::vector2f getScreenPosition()
    {
        core::dim2u ScreenRes = SceneManager->getRenderer()->getViewport().dimension();
        core::vector2f Pos(getPosition().x,getPosition().y);
        Pos.x /= (ScreenRes.width/2);
        Pos.y /= (ScreenRes.height/2);
        return Pos;
    }


protected:
private:

};

typedef IScene2DNode* s2dNode;

}
}
#endif // ISCENE2DNODE_H
