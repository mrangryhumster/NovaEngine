#ifndef CSCENEMANAGER_H
#define CSCENEMANAGER_H

#include "ISceneManager.h"
#include "IRenderer.h"

#include "CScene.h"

namespace novaengine
{
namespace scene
{

class CSceneManager : public ISceneManager
{
public:
    CSceneManager(renderer::IRenderer* Renderer,IEventManager* EventManager);
    virtual ~CSceneManager();

    ISceneNode*         createEmptyNode(ISceneNode* parent = 0, s32 ID = -1, core::vector3f pos = core::vector3f(0,0,0));
    ISceneCamera*       createSceneCamera(ISceneNode* parent = 0,s32 ID = -1,core::vector3f pos = core::vector3f(0,0,0));
    ISceneSprite*       createSceneSprite(renderer::ITexture* SpriteTexture,core::rectf Sourse = core::rectf(0,0,1,1),ISceneNode* parent = 0,s32 ID = -1,core::vector3f position = core::vector3f(0,0,0),core::vector3f rotation = core::vector3f(0,0,0),core::vector3f scale = core::vector3f(1,1,1));
    ISceneStaticMesh*   createSceneStaticMesh(renderer::IStaticMesh* Mesh,ISceneNode* parent = 0,s32 ID = -1,core::vector3f position = core::vector3f(0,0,0),core::vector3f rotation = core::vector3f(0,0,0),core::vector3f scale = core::vector3f(1,1,1));

    void setActiveScene(IScene*);
    IScene* getActiveScene();
    IScene* getRootScene();

    void updateActiveScene();
    void animateActiveScene(f32 dt);
    void renderActiveScene();

    renderer::IRenderer* getRenderer();
    IEventManager*       getEventManager();
protected:
private:
    IScene*      ActiveScene;
    IScene*      RootScene;

    core::dim2u RenderGridResolution;

    //Projections
    core::matrixf PerspectiveProjection;
    core::matrixf OrthographicProjection;
    //LookAtMatrix
    core::matrixf ViewMatrix_2d;
    core::matrixf ViewMatrix_3d;

    renderer::IRenderer* Renderer;
    IEventManager*       EventManager;
};
}
}
#endif // CSCENEMANAGER_H
