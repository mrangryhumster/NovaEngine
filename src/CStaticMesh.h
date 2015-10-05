#ifndef CSTATICMESH_H
#define CSTATICMESH_H

#include "IStaticMesh.h"
#include "IMeshBuffer.h"
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

    void addMeshBuffer(IMeshBuffer*);
    void removeMeshBuffer(u32);
    IMeshBuffer* getMeshBuffer(u32 );

    u32 getMeshBuffersCount();

    IStaticMesh* getCopy();

    std::vector<IMeshBuffer*> getMeshBuffers(); //?

protected:
private:

    std::vector<IMeshBuffer*> MeshBuffers;

};

}
}
#endif // CMESH_H
