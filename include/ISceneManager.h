#ifndef ISceneManager_H
#define ISceneManager_H

#include "Object.h"
#include "ne_types.h"

#include "IRenderer.h"
#include "IEventManager.h"

#include "ITexture.h"
#include "IStaticMesh.h"

#include "IScene.h"

namespace novaengine
{
namespace scene
{

enum E_PROJECTION_MATRIX
{
    EPM_UNDEFINED    = 0,
    EPM_ORTHO        = 1,
    EPM_PERSPECTIVE  = 2,
};
enum E_VIEW_MATRIX
{
    EVM_UNDEFINED = 0,
    EVM_2D        = 1,
    EVM_3D        = 2,
};
enum E_NODE_TYPE
{
    ENT_CAMERA           = 0,
    ENT_EMPTY            = 1,
    ENT_DEFAULT          = 2,
    ENT_SPRITE           = 3,

    ENT_UNKNOW           = 4,

    ENT_COUNT            = 5,
};

//Default nodes
class IScene;
class ISceneNode;
class IScene2DNode;
class ISceneCamera;
class ISceneSprite;
class ISceneStaticMesh;
//Default animators

class ISceneManager : public Object
{
public:

    virtual ISceneNode*         createEmptyNode(ISceneNode* parent = 0, s32 ID = -1, core::vector3f pos = core::vector3f(0,0,0)) = 0;
    virtual ISceneCamera*       createSceneCamera(ISceneNode* parent = 0,s32 ID = -1,core::vector3f pos = core::vector3f(0,0,0)) = 0;
    virtual ISceneSprite*       createSceneSprite(renderer::ITexture* SpriteTexture,core::rectf Sourse = core::rectf(0,0,1,1),ISceneNode* parent = 0,s32 ID = -1,core::vector3f position = core::vector3f(0,0,0),core::vector3f rotation = core::vector3f(0,0,0),core::vector3f scale = core::vector3f(1,1,1)) = 0;
    virtual ISceneStaticMesh*   createSceneStaticMesh(renderer::IStaticMesh* Mesh,ISceneNode* parent = 0,s32 ID = -1,core::vector3f position = core::vector3f(0,0,0),core::vector3f rotation = core::vector3f(0,0,0),core::vector3f scale = core::vector3f(1,1,1)) = 0;

    virtual void setActiveScene(IScene* scene = 0) = 0;
    virtual IScene* getActiveScene() = 0;
    virtual IScene* getRootScene() = 0;

    virtual void updateActiveScene()                  = 0;
    virtual void animateActiveScene(f32 deltatime_ms) = 0;
    virtual void renderActiveScene()                  = 0;

    virtual renderer::IRenderer* getRenderer() = 0;
    virtual IEventManager*       getEventManager() = 0;

protected:
private:
};

}
}


#endif // ISceneManager_H
