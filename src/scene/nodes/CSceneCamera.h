#ifndef CSCENECAMERA_H
#define CSCENECAMERA_H

#include "NTypes.h"
#include "ISceneCamera.h"
#include "NLog.h"

namespace novaengine
{
namespace scene
{
class CSceneCamera : public ISceneCamera
{
public:
    CSceneCamera(ISceneManager* smgr,ISceneNode* p_Parent = 0,s32 p_ID = -1,core::vector3f p_Position = core::vector3f(0,0,0),core::vector3f p_Rotation = core::vector3f(0,0,0)) :
        ISceneCamera(smgr,p_Parent,p_ID,p_Position,p_Rotation){}

    virtual void animate(f32 dt)
    {
        ISceneNode::animate(dt);
    }
    virtual void render()
    {
        updateLookAtMatrix();

        SceneManager->getRenderer()->setTransform(ProjectionMatrix,renderer::EMT_PROJECTION);
        SceneManager->getRenderer()->setTransform(ViewMatrix,renderer::EMT_VIEW);
        SceneManager->getRenderer()->setTransform(core::matrixf(),renderer::EMT_MODEL);
    }

protected:
};
}
}


#endif // CSCENECAMERA_H
