#ifndef ISCENECAMERA_H
#define ISCENECAMERA_H

#include "ISceneNode.h"
#include "NTypes.h"

namespace novaengine
{
namespace scene
{

class ISceneCamera : public ISceneNode
{
public:
    ISceneCamera(ISceneManager* smgr,f32 p_FOV = 90,f32 p_zNear = 1,f32 p_zFar = 1000,ISceneNode* p_Parent = 0,s32 p_ID = -1,core::vector3f p_Position = core::vector3f(0,0,0),core::vector3f p_Rotation = core::vector3f(0,0,0)) :
        ISceneNode(smgr,p_Parent,p_ID,p_Position,p_Rotation),
        FieldOfView(p_FOV),
        zNear(p_zNear),
        zFar(p_zFar),
        UpVector(0,1,0),
        TargetPosition(0,0,-1),
        TargetNode(NULL)
    {

    }


    virtual void setFieldOfView(f32 new_FieldOfView = 90)
    {
        FieldOfView = new_FieldOfView;
        update_projection = true;
    }
    virtual f32 getFieldOfView()
    {
        return FieldOfView;
    }

    virtual void setZNear(f32 new_zNear = 1)
    {
        zNear = new_zNear;
        update_projection = true;
    }
    virtual f32  getZNear()
    {
        return zNear;
    }

    virtual void setZFar(f32 new_zFar = 1000)
    {
        zFar = new_zFar;
        update_projection = true;
    }
    virtual f32  getZFar()
    {
        return zFar;
    }

    virtual void setTarget(core::vector3f new_TargetPosition)
    {
        TargetPosition = new_TargetPosition;
    }
    virtual void setTarget(ISceneNode* new_TargetNode,core::vector3f TargetRelativePosition = core::vector3f(0,0,0))
    {
        TargetNode        = new_TargetNode;
        TargetPosition    = TargetRelativePosition;
        update_projection = true;
    }
    virtual core::vector3f getTarget()
    {
        return TargetPosition;
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

    virtual core::matrixf getPerspectiveProjectionMatrix()
    {
        UpdateProjectionsMatrix();
        return PerspectiveProjectionMatrix;
    }
    virtual void usePerspectiveProjectionMatrix()
    {
        SceneManager->getRenderer()->setTransform(getPerspectiveProjectionMatrix(),renderer::EMT_PROJECTION);
    }

    virtual core::matrixf getOrthographicProjectionMatrix()
    {
        UpdateProjectionsMatrix();
        return OrthographicProjectionMatrix;
    }
    virtual void useOrthographicProjectionMatrix()
    {
        SceneManager->getRenderer()->setTransform(getOrthographicProjectionMatrix(),renderer::EMT_PROJECTION);
    }

    virtual core::matrixf get2DViewMatrix()
    {
        core::matrixf ViewMatrix;
        ViewMatrix.setTranslate(-getAbsolutePosition());
        return ViewMatrix;
    }
    virtual void use2DViewMatrix()
    {
        SceneManager->getRenderer()->setTransform(get2DViewMatrix(),renderer::EMT_VIEW);
    }

    virtual core::matrixf get3DViewMatrix()
    {
        core::matrixf look_at_matrix;

        if(TargetNode)
        {
            core::vector3f RelativePosition = TargetPosition;
            TargetNode->getAbsoluteTransformation().rotate_vector(RelativePosition);

            look_at_matrix.buildLookAtMatrix(getAbsolutePosition(),TargetNode->getAbsolutePosition() + RelativePosition,UpVector);
        }
        else
        {
            look_at_matrix.buildLookAtMatrix(getAbsolutePosition(),TargetPosition,UpVector);
        }

        return look_at_matrix;
    }
    virtual void use3DViewMatrix()
    {
        SceneManager->getRenderer()->setTransform(get3DViewMatrix(),renderer::EMT_VIEW);
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
    virtual void setLookAtMatrix()
    {
        ViewMatrix.buildLookAtMatrix(Position,TargetPosition,UpVector);
    }
    //------------------------------------------------------------------------------------

    virtual core::vector2s getSceneCoordFromCursorCoord(core::vector2s CursorPos)
    {
        core::vector2s ScenePos = core::vector2s(getAbsolutePosition().x,getAbsolutePosition().y);
        core::rectu RendererViewPort_rect = SceneManager->getRenderer()->getViewport();
        core::dim2u RendererViewPort(RendererViewPort_rect.width(),RendererViewPort_rect.height());

        f32 modif_x = f32(RendererViewPort.width)  / f32(ViewPort.width  );
        f32 modif_y = f32(RendererViewPort.height) / f32(ViewPort.height );

        ScenePos.x +=  (CursorPos.x - (float)(RendererViewPort.width  / 2)) / modif_x;
        ScenePos.y += -(CursorPos.y - (float)(RendererViewPort.height / 2)) / modif_y;
        return ScenePos;
    }
    virtual core::vector2s getCursorCoordFromSceneCoord(core::vector3f ScenePos)
    {
        return core::vector2s(ScenePos.x,ScenePos.y);
    }

    virtual s32 getNodeType()
    {
        return ENT_CAMERA;
    }

protected:

    core::vector3f UpVector;
    core::vector3f TargetPosition;
    ISceneNode*    TargetNode;

    core::matrixf OrthographicProjectionMatrix;
    core::matrixf PerspectiveProjectionMatrix;

    bool update_projection;
    core::dim2u ViewPort;

    //Updated

    u32           ProjectionMatrixType;
    core::matrixf ProjectionMatrix;
    core::matrixf ViewMatrix;


private:

    void UpdateProjectionsMatrix()
    {
        core::rectu RendererViewPort_rect = SceneManager->getRenderer()->getViewport();

        if(ViewPort.width != RendererViewPort_rect.width() || ViewPort.height != RendererViewPort_rect.height() || update_projection)
        {
            ViewPort.width  = RendererViewPort_rect.width();
            ViewPort.height = RendererViewPort_rect.height();

            //-----------------------------------------------
            OrthographicProjectionMatrix.makeIndetify();
            OrthographicProjectionMatrix.buildOrthographicProjectionMatrix(-(ViewPort.width/1920)/2,(ViewPort.width/1080)/2,-(ViewPort.height/1920)/2,(ViewPort.height/1080)/2,-zFar,zFar);

            PerspectiveProjectionMatrix.makeIndetify();
            PerspectiveProjectionMatrix.buildPerspectiveProjectionMatrix( FieldOfView, (float)ViewPort.width/(float)ViewPort.height, zNear, zFar);
            //-----------------------------------------------
            update_projection = false;
        }
    }





};

typedef ISceneCamera* snCamera;

}
}


#endif // ICAMERASCENENODE_H
