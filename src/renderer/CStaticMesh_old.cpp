#include "CStaticMesh.h"

namespace novaengine
{

namespace renderer
{

CStaticMesh::CStaticMesh()
{

}

CStaticMesh::~CStaticMesh()
{
    if(MeshUnits.size() > 0)
    {
        u32 UnitsCount = MeshUnits.size();
        for(u32 i = 0; i < UnitsCount; i++)
        {
            MeshUnits[i].VertexBuffer->release();
            if(MeshUnits[i].Material)
                MeshUnits[i].Material->release();
        }
        MeshUnits.clear();
    }
}


u32  CStaticMesh::getMeshUnitsCount()
{
    return MeshUnitsCount;
}

void CStaticMesh::addMeshUnit(SMeshUnit MeshUnit)
{
    if(MeshUnit.VertexBuffer == NULL)
        return;

    MeshUnit.VertexBuffer->capture();
    if(MeshUnit.Material)
        MeshUnit.Material->capture();

    MeshUnits.push_back(MeshUnit);

    MeshUnitsCount = MeshUnits.size();
}

void CStaticMesh::addMeshUnit(IVertexBuffer* VertexBuffer,IMaterial* Material)
{
    if(!VertexBuffer)
        return;

    VertexBuffer->capture();
    if(Material)
        Material->capture();

    SMeshUnit NewMeshUnit;
    NewMeshUnit.VertexBuffer = VertexBuffer;
    NewMeshUnit.Material     = Material;

    MeshUnits.push_back(NewMeshUnit);

    MeshUnitsCount = MeshUnits.size();
}

void CStaticMesh::removeMeshUnit(u32 id)
{
    if(id > MeshUnitsCount)
        return ;

    SMeshUnit MeshUnit = MeshUnits[id];
    MeshUnit.VertexBuffer->release();
    if(MeshUnit.Material)
        MeshUnit.Material->release();
    MeshUnits.erase(MeshUnits.begin()+id);

    MeshUnitsCount = MeshUnits.size();
}

SMeshUnit CStaticMesh::getMeshUnit(u32 id)
{
    if(id > MeshUnitsCount)
        return SMeshUnit();
    return MeshUnits[id];
}

IVertexBuffer* CStaticMesh::getVertexBuffer(u32 id)
{
    if(id > MeshUnitsCount)
        return NULL;
    return MeshUnits[id].VertexBuffer;
}

IMaterial* CStaticMesh::getMaterial(u32 id)
{
    if(id > MeshUnitsCount)
        return NULL;
    return MeshUnits[id].Material;
}

IStaticMesh* CStaticMesh::getCopy()
{
    return NULL;
}

}
}
