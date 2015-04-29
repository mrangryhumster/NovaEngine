#ifndef CSCENE_H
#define CSCENE_H

#include "IScene.h"
#include "ISceneManager.h"
#include "ISceneNode.h"
#include "ISceneCamera.h"

#include <vector>
#include <list>

namespace novaengine
{
namespace scene
{

class CScene : public IScene
{
public:
    CScene(ISceneManager* smgr);
    virtual ~CScene();

    //NodeList
    void RegisterNode(ISceneNode*,bool allow_callback = true);
    void UnRegisterNode(ISceneNode*,bool allow_callback = true);

    ISceneNode** getNodeList();
    u32  getNodeCount();
    void eraseScene();
    ISceneNode* getNodeByID(s32 id);

    //Camera
    void setActiveCamera(ISceneCamera*);
    ISceneCamera* getActiveCamera();

    //Scene
    void setActive();
    bool isActive();
    //Render
    void update();
    void animate(f32);
    void render();

protected:
private:
    //internal func
    bool IsRegistred(ISceneNode* SceneNode);
    std::vector<ISceneNode*>* getNodeListByType(u32);

    void rebuild_SceneNode_List();

    //SceneManager
    ISceneManager* SceneManager;

    //Node lists
    std::vector<ISceneNode*> SceneNode_Types_list[ENT_COUNT];
    //FullNodeList
    std::vector<ISceneNode*> SceneNode_Full_list;
    //SceneCamera
    ISceneCamera* SceneCamera;

};
}
}
#endif // CSCENE_H
