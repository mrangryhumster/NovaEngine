#ifndef ISTATICMESH_H
#define ISTATICMESH_H

#include "Object.h"
#include "Resource.h"

#include "IMeshBuffer.h"
#include "IMaterial.h"


namespace novaengine
{
namespace renderer
{
class IStaticMesh : public Resource
{
    public:

        virtual void addMeshBuffer(IMeshBuffer*) = 0;
        virtual void removeMeshBuffer(u32)       = 0;
        virtual IMeshBuffer* getMeshBuffer(u32)  = 0;

        virtual u32 getMeshBuffersCount()        = 0;

        virtual IStaticMesh* getCopy() = 0;

    protected:
    private:

};

}
}
#endif // IMESH_H
