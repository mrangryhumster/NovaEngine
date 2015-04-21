#ifndef ISTATICMESH_H
#define ISTATICMESH_H

#include "Object.h"
#include "Resource.h"

#include "IVertexBuffer.h"
#include "IMaterial.h"


namespace novaengine
{
namespace renderer
{

struct SMeshUnit
{
    SMeshUnit():
        VertexBuffer(NULL),
        Material(NULL)
        {}
    IVertexBuffer* VertexBuffer;
    IMaterial*     Material;
};

class IStaticMesh : public Resource
{
    public:
        virtual void addMeshUnit(SMeshUnit )                = 0;
        virtual void addMeshUnit(IVertexBuffer*,IMaterial*) = 0;

        virtual void removeMeshUnit(u32 id)                 = 0;
        virtual u32  getMeshUnitsCount()                    = 0;

        virtual const SMeshUnit* getMeshUnit(u32 id)        = 0;

        virtual IVertexBuffer*  getVertexBuffer(u32 id) = 0;
        virtual IMaterial*      getMaterial(u32 id)     = 0;

        virtual void setVertexBuffer(u32 id,IVertexBuffer*) = 0;
        virtual void setMaterial(u32 id,IMaterial*)         = 0;

        virtual IStaticMesh* getCopy() = 0;

    protected:
    private:

};

}
}
#endif // IMESH_H
