#ifndef ISCENECAMERA_H
#define ISCENECAMERA_H

#include "ISceneNode.h"
#include "ne_types.h"

namespace novaengine
{
namespace scene
{

class ISceneCamera : public ISceneNode
{
public:
    ISceneCamera(ISceneManager* smgr,ISceneNode* p_Parent = 0,s32 p_ID = -1,core::vector3f p_Position = core::vector3f(0,0,0),core::vector3f p_Rotation = core::vector3f(0,0,0)) :
        ISceneNode(smgr,p_Parent,p_ID,p_Position,p_Rotation),
        UpVector(0,1,0),
        Target(0,0,-1)
    {

    }
    virtual void setTarget(core::vector3f p_Target)
    {
        Target = p_Target;
    }
    virtual core::vector3f getTarget()
    {
        return Target;
    }

    virtual void setUpVector(core::vector3f new_UpVector)
    {
        UpVector = new_UpVector;
    }
    virtual core::vector3f getUpVector()
    {
        return UpVector;
    }

    virtual void setActive()
    {
        if(Scene)
            Scene->setActiveCamera(this);
    }
    virtual bool isActive()
    {
        if(Scene)
            return (Scene->getActiveCamera() == this);
        return false;
    }
    //------------------------------------------------------------------------------------
    virtual void setPerspectiveProjectionMatrix(f32 p_FOV = 90,f32 p_Aspect = 1.333333f,f32 p_zNear = 1.f,f32 p_zFar = 1000.f)
    {
        ProjectionMatrix.buildPerspectiveProjectionMatrix(p_FOV,p_Aspect,p_zNear,p_zFar);
    }
    virtual void setOrthographicProjectionMatrix(float p_Left = -1.f,float p_Right = 1.f,float p_Bottom = -1.f,float p_Top = 1.f,float p_zNear = -1.f,float p_zFar = 1.f)
    {
        ProjectionMatrix.buildOrthographicProjectionMatrix(p_Left,p_Right,p_Bottom,p_Top,p_zNear,p_zFar);
    }
    virtual void updateLookAtMatrix()
    {
        ViewMatrix.buildLookAtMatrix(getAbsolutePosition(), Target, UpVector);
    }
    //------------------------------------------------------------------------------------
    virtual core::matrixf getProjectionMatrix()
    {
        return ProjectionMatrix;
    }
    virtual core::matrixf getViewMatrix()
    {
        return ViewMatrix;
    }
    //------------------------------------------------------------------------------------
    virtual s32 getNodeType()
    {
        return ENT_CAMERA;
    }

protected:

    core::vector3f UpVector;
    core::vector3f Target;
    //Updated
    u32           ProjectionMatrixType;
    core::matrixf ProjectionMatrix;
    core::matrixf ViewMatrix;


private:
};

typedef ISceneCamera* snCamera;

}
}


#endif // ICAMERASCENENODE_H
