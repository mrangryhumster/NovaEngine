#include "CScene.h"

namespace novaengine
{
namespace scene
{


CScene::CScene(ISceneManager* smgr):
    SceneManager(smgr),
    SceneCamera(nullptr)
{
}

CScene::~CScene()
{
    eraseScene();
}

//NodeList
void CScene::RegisterNode(ISceneNode* SceneNode,bool allow_callback)
{
    if(!SceneNode)
        return;

    if(IsRegistred(SceneNode))
        return;

    SceneNode->capture();
    //---------------------------------
    getNodeListByType(SceneNode->getNodeType())->push_back(SceneNode);
    //--------------------------------
    if(allow_callback)
        SceneNode->RegisterNode(this,false);

    rebuild_SceneNode_List();
}
void CScene::UnRegisterNode(ISceneNode* SceneNode,bool allow_callback)
{
    if(SceneNode == NULL || SceneNode->getScene() != this)
        return;

    std::vector<ISceneNode*>* NodeList = getNodeListByType(SceneNode->getNodeType());
    if(NodeList == NULL)
        return;

    u32 ListSize = NodeList->size();
    for(u32 i = 0; i < ListSize; i++)
    {
        if(SceneNode == NodeList->at(i))
        {
            if(allow_callback)
                NodeList->at(i)->UnRegisterNode(false);

            NodeList->at(i)->release();
            NodeList->assign(i,NULL);
            NodeList->erase(NodeList->begin() + i);
        }
    }

    rebuild_SceneNode_List();
}
ISceneNode** CScene::getNodeList()
{
    return &SceneNode_Full_list.front();
}
u32  CScene::getNodeCount()
{
    return SceneNode_Full_list.size();
}
void CScene::eraseScene()
{
    setActiveCamera(NULL);

    for(u32 Type = 0; Type < ENT_COUNT; Type++)
    {

        u32 ListSize = SceneNode_Types_list[Type].size();
        for(u32 i = 0; i < ListSize; i++)
        {
            // Store node in temp varible and set pointer in null
            // because SceneNode::UnRegisterNode recall Scene::UnRegisterNode
            ISceneNode* Node = SceneNode_Types_list[Type][i];
            SceneNode_Types_list[Type][i] = NULL;
            if(Node == NULL)
                break;
            Node->UnRegisterNode(false);
            Node->release();
        }

        SceneNode_Types_list[Type].clear();
    }
    SceneNode_Full_list.clear();
}
ISceneNode* CScene::getNodeByID(s32 id)
{
    u32 ListSize = SceneNode_Full_list.size();

    for(u32 NodeNum = 0; NodeNum < ListSize; NodeNum++)
    {
        if(SceneNode_Full_list[NodeNum]->getNodeID() == id)
            return SceneNode_Full_list[NodeNum];
    }

    return NULL;
}
//Camera
void CScene::setActiveCamera(ISceneCamera* Camera)
{
    if(SceneCamera)
        SceneCamera->release();
    if(Camera)
        Camera->capture();

    SceneCamera = Camera;
}
ISceneCamera* CScene::getActiveCamera()
{
    return SceneCamera;
}
void CScene::setActive()
{
    SceneManager->setActiveScene(this);
}
bool CScene::isActive()
{
    return (SceneManager->getActiveScene() == this)?true:false;
}
void CScene::update()
{
    for(u32 Type = 0; Type < ENT_COUNT; Type++)
    {
        u32 ListSize = SceneNode_Types_list[Type].size();
        for(u32 i = 0; i < ListSize; i++)
            SceneNode_Types_list[Type][i]->update();
    }
}
void CScene::animate(f32 dt)
{
    for(u32 Type = 0; Type < ENT_COUNT; Type++)
    {
        for(auto it = SceneNode_Types_list[Type].begin(); it != SceneNode_Types_list[Type].end(); it++)
            (*it)->animate(dt);
    }
}
void CScene::render()
{
    SceneManager->setActiveScene(this);

    if(SceneCamera)
        SceneCamera->render();

    for(auto it = SceneNode_Types_list[ENT_DEFAULT].begin(); it != SceneNode_Types_list[ENT_DEFAULT].end(); it++)
        (*it)->render();


    /*
    u32 ListSize = SceneNode_Types_list[ENT_DEFAULT].size();
    for(u32 i = 0; i < ListSize; i++)
        SceneNode_Types_list[ENT_DEFAULT][i]->render();

    u32 ListSize = SceneNode_Types_list[ENT_DEFAULT].size();
    for(u32 i = 0; i < ListSize; i++)
        SceneNode_Types_list[ENT_DEFAULT][i]->render();
    */
}
//internal
bool CScene::IsRegistred(ISceneNode* SceneNode)
{
    std::vector<ISceneNode*>* NodeList = getNodeListByType(SceneNode->getNodeType());

    u32 ListSize = NodeList->size();
    for(u32 NodeNum = 0; NodeNum < ListSize; NodeNum++)
    {
        if(NodeList->at(NodeNum) == SceneNode)
            return true;
    }
    return false;
}
std::vector<ISceneNode*>* CScene::getNodeListByType(u32 NodeType)
{
    if(NodeType < (s32)ENT_COUNT)
        return &SceneNode_Types_list[NodeType];
    else
        return &SceneNode_Types_list[ENT_UNKNOW];
}

void CScene::rebuild_SceneNode_List()
{
    SceneNode_Full_list.clear();
    for(u32 Type = 0; Type < ENT_COUNT; Type++)
        SceneNode_Full_list.insert(SceneNode_Full_list.end(),SceneNode_Types_list[Type].begin(),SceneNode_Types_list[Type].end());
}

}
}
