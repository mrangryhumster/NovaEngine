#include "CStaticMesh.h"

namespace novaengine
{

namespace renderer
{

CStaticMesh::CStaticMesh()
{

}
//--------------------------------------------------------------------------------------------------------
CStaticMesh::~CStaticMesh()
{
    if(MeshBuffers.size() > 0)
    {
        u32 BuffersCount = MeshBuffers.size();
        for(u32 i = 0; i < BuffersCount; i++)
            MeshBuffers[i]->release();
        MeshBuffers.clear();
    }
}
//--------------------------------------------------------------------------------------------------------
void CStaticMesh::addMeshBuffer(IMeshBuffer* new_meshbuffer)
{
    if(new_meshbuffer)
        new_meshbuffer->capture();
    MeshBuffers.push_back(new_meshbuffer);
}
//--------------------------------------------------------------------------------------------------------
void CStaticMesh::removeMeshBuffer(u32 id)
{
    if(id >= MeshBuffers.size())
        return;
    MeshBuffers[id]->release();
    MeshBuffers.erase(MeshBuffers.begin() + id);
}
//--------------------------------------------------------------------------------------------------------
IMeshBuffer* CStaticMesh::getMeshBuffer(u32 id)
{
    if(id >= MeshBuffers.size())
        return NULL;
    return MeshBuffers[id];
}
//--------------------------------------------------------------------------------------------------------
u32  CStaticMesh::getMeshBuffersCount()
{
    return MeshBuffers.size();
}
//--------------------------------------------------------------------------------------------------------
std::vector<IMeshBuffer*> CStaticMesh::getMeshBuffers()
{
    return MeshBuffers;
}
//--------------------------------------------------------------------------------------------------------
IStaticMesh* CStaticMesh::getCopy()
{
    return NULL;
}
//--------------------------------------------------------------------------------------------------------
}
}
