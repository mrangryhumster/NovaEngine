#ifndef CSTATICMESH_H
#define CSTATICMESH_H

#include "IStaticMesh.h"
#include "IVertexBuffer.h"
#include "IMaterial.h"
#include <vector>


namespace novaengine
{
namespace renderer
{

class CStaticMesh : public IStaticMesh
{
public:
    CStaticMesh();
    virtual ~CStaticMesh();

        void addMeshUnit(SMeshUnit);
        void addMeshUnit(IVertexBuffer*,IMaterial*);

        void removeMeshUnit(u32 id);
        u32  getMeshUnitsCount();

        const SMeshUnit* getMeshUnit(u32 id);

        IVertexBuffer*  getVertexBuffer(u32 id);
        IMaterial*      getMaterial(u32 id);

        void setVertexBuffer(u32 id,IVertexBuffer*);
        void setMaterial(u32 id,IMaterial*);

        IStaticMesh* getCopy();

protected:
private:
    std::vector<SMeshUnit>  MeshUnits;
};

}
}
#endif // CMESH_H
