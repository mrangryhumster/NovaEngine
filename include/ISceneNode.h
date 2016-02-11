#ifndef ISCENENODE_H
#define ISCENENODE_H

#include <vector>

#include "Object.h"
#include "NTypes.h"

#include "ISceneManager.h"
#include "IScene.h"

#include "ISceneNodeAnimator.h"

namespace novaengine
{
namespace scene
{

class ISceneNode : public Object
{
public:

    ISceneNode(ISceneManager* smgr,ISceneNode* p_Parent = NULL,s32 p_ID = -1,core::vector3f p_Position = core::vector3f(0,0,0),core::vector3f p_Rotation= core::vector3f(0,0,0),core::vector3f p_Scale = core::vector3f(1,1,1)):
        SceneManager(smgr),
        Position(p_Position),
        Rotation(p_Rotation),
        Scale(p_Scale),
        Scene(NULL),
        Parent(NULL),
        NodeID(p_ID),
        Visible(true),
        update_transform(1)
    {
        setParent(p_Parent);
    }

    virtual ~ISceneNode()
    {
        removeAllChilds();
        removeAllAnimators();
        UnRegisterNode();
    }

    //----------------------------------Position
    virtual void setPosition(core::vector3f v_position)
    {
        Position = v_position;
        update_transform = true;
    }
    virtual core::vector3f getPosition()
    {
        return Position;
    }
    virtual core::vector3f getAbsolutePosition()
    {
        if(Parent)
        {
            return (getAbsoluteTransformation().getTranslate());
        }
        return Position;
    }
    //----------------------------------Rotation
    virtual void setRotation(core::vector3f v_rotation)
    {
        Rotation = v_rotation;
        update_transform = true;
    }
    virtual core::vector3f getRotation()
    {
        return Rotation;
    }
    virtual core::vector3f getAbsoluteRotation()
    {
        if(Parent)
            return Rotation + Parent->getAbsoluteRotation();
        return Rotation;
    }
    //----------------------------------Scale
    virtual void setScale(core::vector3f v_scale)
    {
        Scale = v_scale;
        update_transform = true;
    }
    virtual core::vector3f getScale()
    {
        return Scale;
    }
    virtual core::vector3f getAbsoluteScale()
    {
        if(Parent)
            return (Scale + Parent->getAbsoluteScale());
        return Scale;
    }
    //----------------------------------getTransformation
    virtual core::matrixf getRelativeTransformation()
    {
        if(update_transform)
        {
            RelativeTransformation.makeIdentify();
            RelativeTransformation.setTranslate(Position);
            RelativeTransformation.setRotationDegrees(Rotation);
            RelativeTransformation.setScale(Scale);
            update_transform = false;
        }
        return RelativeTransformation;
    }
    virtual core::matrixf getAbsoluteTransformation()
    {
        if(Parent)
            return getAbsoluteTransformation()*getRelativeTransformation();
        return getRelativeTransformation();
    }
    //----------------------------------Rel
    virtual void setParent(ISceneNode* newParent)
    {
        if(newParent == Parent)
            return;

        capture();

        if(Parent)
            Parent->removeChild(this);

        Parent = newParent;

        if(Parent)
            Parent->addChild(this);

        release();
    }

    virtual ISceneNode* getParent()
    {
        return Parent;
    }

    virtual ISceneNode** getChilds()
    {
        return &Childs.front();
    }

    virtual void addChild(ISceneNode* Child)
    {
        if(Child)
        {
            if(Child->getScene() != Scene)
                Child->RegisterNode(Scene);

            Child->Parent = this;
            Child->capture();
            Childs.push_back(Child);
        }
    }
    virtual void removeChild(ISceneNode* Child)
    {
        if(Child)
        {
            u32 ListSize = Childs.size();

            for(u32 i = 0; i < ListSize; i++)
            {
                if(Child == Childs[i])
                {
                    Childs[i]->Parent = NULL;
                    Childs[i]->release();
                    Childs.erase(Childs.begin() + i);
                }
            }
        }
    }
    virtual void removeAllChilds()
    {
        u32 ListSize = Childs.size();

        if(ListSize == 0)
            return;

        for(u32 i = 0; i < ListSize; i++)
        {
            Childs[i]->Parent = NULL;
            Childs[i]->release();
        }
        Childs.clear();
    }
    //----------------------------------Animators
    virtual void addAnimator(ISceneNodeAnimator* Animator)
    {
        Animator->capture();
        Animators.push_back(Animator);
    }
    virtual ISceneNodeAnimator** getAnimators()
    {
        return &Animators.front();
    }
    virtual void removeAllAnimators()
    {
        u32 ListSize = Animators.size();
        for(u32 i = 0; i < ListSize;i++)
            Animators[i]->release();
        Animators.clear();
    }
    //----------------------------------NodeID
    virtual void setNodeID(s32 id)
    {
        NodeID = id;
    }
    virtual s32 getNodeID()
    {
        return NodeID;
    }
    //----------------------------------NodeType
    virtual s32 getNodeType()
    {
        return ENT_UNKNOW;
    }
    //----------------------------------Visible
    virtual void setVisible(bool _visible)
    {
        Visible = _visible;
    }
    virtual bool isVisible()
    {
        return Visible;
    }
    //----------------------------------Registering node
    virtual void OnRegisterNode()
    {

    }
    virtual void OnUnRegisterNode()
    {

    }

    virtual void RegisterNode(IScene* newScene = 0,bool allow_callback = true)
    {

        if(newScene == NULL)
            newScene = SceneManager->getRootScene();
        if(newScene == Scene)
            return;

        capture();

        if(Scene)
            UnRegisterNode();
        if(newScene && allow_callback)
            newScene->RegisterNode(this,false);

        Scene = newScene;
        RegisterChilds(Scene);

        release();
        OnRegisterNode();
    }

    virtual void UnRegisterNode(bool allow_callback = true)
    {
        if(Scene)
        {
            if(Parent)
                Parent->removeChild(this);

            if(allow_callback)
                Scene->UnRegisterNode(this,false);

            UnRegisterChilds();

            Scene = NULL;
            OnUnRegisterNode();
        }
    }

    virtual IScene* getScene()
    {
        return Scene;
    }

    virtual void update()
    {
        //do smth
    }

    virtual void animate(f32 dt)
    {
        u32 ListSize = Animators.size();
        for(u32 i = 0; i < ListSize; i++)
            if(Animators[i]->isActive())
                Animators[i]->animate(this,dt);
    }

    virtual void render()  = 0;

protected:

    ISceneManager* SceneManager;

//Transform
    core::matrixf  AbsoluteTranformation;
    core::matrixf  RelativeTransformation;
    core::vector3f Position;
    core::vector3f Rotation;
    core::vector3f Scale;
//Node relative
    IScene*     Scene;
    ISceneNode* Parent;
    std::vector<ISceneNode*> Childs;

//Node animators
    std::vector<ISceneNodeAnimator*> Animators;

//Node prop
    s32  NodeID;
    s32  NodeType;
    bool Visible;

    bool update_transform;

private:

    void RegisterChilds(IScene* newScene)
    {
        u32 ListSize = Childs.size();
        if(ListSize > 0)
            for(u32 i = 0; i < ListSize; i++)
                Childs[i]->RegisterNode(newScene);
    }

    void UnRegisterChilds()
    {
        u32 ListSize = Childs.size();
        if(ListSize > 0)
            for(u32 i = 0; i < ListSize; i++)
            {
                Scene->UnRegisterNode(Childs[i],false);
                Childs[i]->UnRegisterChilds();
                Childs[i]->Scene = NULL;
            }
    }

};

typedef ISceneNode* sNode;
}
}
#endif // ISCENENODE_H
