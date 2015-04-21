#include "CSceneManager.h"

#include "nodes\CSceneEmptyNode.h"
#include "nodes\CSceneCamera.h"
#include "nodes\CSceneSprite.h"
#include "nodes\CSceneStaticMesh.h"

namespace novaengine
{
namespace scene
{

CSceneManager::CSceneManager(renderer::IRenderer* p_Renderer,IEventManager* p_EventManager):
    Renderer(p_Renderer),
    EventManager(p_EventManager)
{
    RootScene = new CScene(this);
    setActiveScene(RootScene);
}

CSceneManager::~CSceneManager()
{
    RootScene->release();
}
ISceneNode*   CSceneManager::createEmptyNode(ISceneNode* parent, s32 ID, core::vector3f pos)
{
    return new CSceneEmptyNode(this,parent,ID,pos);
}
ISceneCamera* CSceneManager::createSceneCamera(ISceneNode* parent,s32 ID,core::vector3f pos)
{
    return new CSceneCamera(this,parent,ID,pos);
}
ISceneSprite* CSceneManager::createSceneSprite(renderer::ITexture* SpriteTexture,core::rectf Source,ISceneNode* parent,s32 ID,core::vector3f position,core::vector3f rotation,core::vector3f scale)
{
    return new CSceneSprite(this,SpriteTexture,Source,parent,ID,position,rotation,scale);
}
ISceneStaticMesh*   CSceneManager::createSceneStaticMesh(renderer::IStaticMesh* Mesh,ISceneNode* parent,s32 ID,core::vector3f position,core::vector3f rotation,core::vector3f scale)
{
    return new CSceneStaticMesh(this,Mesh,parent,ID,position,rotation,scale);
}
void CSceneManager::setActiveScene(IScene* Scene)
{
    if(Scene)
        ActiveScene = Scene;
    else
        ActiveScene = RootScene;
}
IScene* CSceneManager::getActiveScene()
{
    return ActiveScene;
}
IScene* CSceneManager::getRootScene()
{
    return RootScene;
}

void CSceneManager::updateActiveScene()
{
    ActiveScene->update();
}
void CSceneManager::animateActiveScene(f32 dt)
{
     ActiveScene->animate(dt);
}
void CSceneManager::renderActiveScene()
{
    ActiveScene->render();
}
renderer::IRenderer* CSceneManager::getRenderer()
{
    return Renderer;
}
IEventManager* CSceneManager::getEventManager()
{
    return EventManager;
}
}
}
