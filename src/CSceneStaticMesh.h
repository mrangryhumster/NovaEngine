#ifndef CSCENESTATICMESH_H
#define CSCENESTATICMESH_H

#include "ISceneStaticMesh.h"
#include "NLogger.h"

namespace novaengine
{
namespace scene
{

class CSceneStaticMesh : public ISceneStaticMesh
{
public:

    CSceneStaticMesh(ISceneManager* smgr,renderer::IStaticMesh* mesh,ISceneNode* parent = 0,s32 ID = -1,core::vector3f position = core::vector3f(0,0,0),core::vector3f rotation= core::vector3f(0,0,0),core::vector3f scale= core::vector3f(1,1,1)):
        ISceneStaticMesh(smgr,parent,ID,position,rotation,scale),
        StaticMesh(NULL)
    {
        setMesh(mesh);
    }

    virtual ~CSceneStaticMesh()
    {
        if(StaticMesh)
        {
            StaticMesh->release();
        }
    }

    void   setMesh(renderer::IStaticMesh* NewMesh)
    {
        if(StaticMesh)
            StaticMesh->release();
        if(NewMesh)
            NewMesh->capture();
        StaticMesh = NewMesh;
    }
    renderer::IStaticMesh* getMesh()
    {
        return StaticMesh;
    }

    s32 getNodeType()
    {
        return ENT_DEFAULT;
    }

    void render()
    {
        renderer::IRenderer* Renderer = SceneManager->getRenderer();

        if(!isVisible())
            return;
        if(!StaticMesh || StaticMesh->getMeshBuffersCount() == 0)
            return;

        Renderer->setTransform(getAbsoluteTransformation(),renderer::EMT_MODEL);
        //------------------------------------------------------
        u32 MeshUnitsCount = StaticMesh->getMeshBuffersCount();
        for(u32 i = 0; i < MeshUnitsCount; i++)
        {
            if(StaticMesh->getMeshBuffer(i)->getMaterial())
            {
                Renderer->bindMaterial(StaticMesh->getMeshBuffer(i)->getMaterial());
            }
            Renderer->drawMeshBuffer(StaticMesh->getMeshBuffer(i));
        }
        //------------------------------------------------------
    }

protected:
private:
    renderer::IStaticMesh* StaticMesh;
};


}
}


#endif // CSCENESTATICMESH_H
