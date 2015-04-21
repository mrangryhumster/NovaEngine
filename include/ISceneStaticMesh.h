#ifndef ISCENESTATICMESH_H
#define ISCENESTATICMESH_H

#include "ISceneNode.h"
#include "IStaticMesh.h"

namespace novaengine
{
namespace scene
{

class ISceneStaticMesh : public ISceneNode
{
    public:
        ISceneStaticMesh(ISceneManager* smgr,ISceneNode* parent = 0,s32 ID = -1,core::vector3f position = core::vector3f(0,0,0),core::vector3f rotation= core::vector3f(0,0,0),core::vector3f scale= core::vector3f(1,1,1)):
        ISceneNode(smgr,parent,ID,position,rotation,scale)
        {
        }

        virtual void   setMesh(renderer::IStaticMesh*) = 0;
        virtual renderer::IStaticMesh* getMesh()       = 0;

    protected:
    private:
};

}
}
#endif // ISCENESTATICMESH_H
