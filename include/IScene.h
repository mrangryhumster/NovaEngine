#ifndef ISCENE_H
#define ISCENE_H

#include "NTypes.h"
#include "Object.h"


namespace novaengine
{
namespace scene
{

class ISceneNode;
class ISceneCamera;

class IScene : public Object
{
public:

    //NodeList
    virtual void RegisterNode(ISceneNode*  ,bool recall_node_register_function   = true) = 0;
    virtual void UnRegisterNode(ISceneNode*,bool recall_node_unregister_function = true) = 0;

    virtual ISceneNode** getNodeList()       = 0;
    virtual u32  getNodeCount()              = 0;
    virtual void eraseScene()                = 0;
    virtual ISceneNode* getNodeByID(s32 id)  = 0;

    //Camera
    virtual void setActiveCamera(ISceneCamera*)  = 0;
    virtual ISceneCamera* getActiveCamera()      = 0;

    //Scene
    virtual void setActive() = 0;
    virtual bool isActive()  = 0;
    //Render
    virtual void update()     = 0;
    virtual void animate(f32) = 0;
    virtual void render()     = 0;



protected:
private:
};

}
}

#endif // ISCENE_H
