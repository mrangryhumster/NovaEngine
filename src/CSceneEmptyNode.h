#ifndef CSCENEEMPTYNODE_H
#define CSCENEEMPTYNODE_H

#include "ISceneNode.h"

namespace novaengine
{
namespace scene
{

class CSceneEmptyNode : public ISceneNode
{
public:
    CSceneEmptyNode(ISceneManager* smgr,ISceneNode* p_Parent = NULL,s32 p_ID = -1,core::vector3f p_Position = core::vector3f(0,0,0)) :
        ISceneNode(smgr,p_Parent,p_ID,p_Position)
    {}
    virtual ~CSceneEmptyNode()
    {}

    s32 getNodeType()
    {
        return ENT_EMPTY;
    }

    virtual void render(){};

protected:
private:
};

}
}

#endif // CSCENEEMPTYNODE_H
