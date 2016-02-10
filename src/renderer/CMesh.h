#ifndef CMESH_H
#define CMESH_H

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

    u32  getMeshUnitsCount();

    void addMeshUnit(SMeshUnit MeshUnit);
    void addMeshUnit(IVertexBuffer* VertexBuffer,IMaterial* Material);
    void removeMeshUnit(u32 id);

    SMeshUnit getMeshUnit(u32 id);
    IVertexBuffer* getVertexBuffer(u32 id);
    IMaterial* getMaterial(u32 id);

    IStaticMesh* getCopy();

protected:
private:

    u32                    MeshUnitsCount;
    std::vector<SMeshUnit> MeshUnits;

};

}
}
#endif // CMESH_H
