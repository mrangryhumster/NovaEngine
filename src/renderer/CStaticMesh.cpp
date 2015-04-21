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

void CStaticMesh::addMeshUnit(SMeshUnit new_unit)
{
    if(new_unit.VertexBuffer != NULL)
        new_unit.VertexBuffer->capture();
    else
        return;

    if(new_unit.Material)
        new_unit.Material->capture();

    MeshUnits.push_back(new_unit);
}

void CStaticMesh::addMeshUnit(IVertexBuffer* new_vertexbuffer,IMaterial* new_material)
{
    SMeshUnit new_unit;
    new_unit.VertexBuffer = new_vertexbuffer;
    new_unit.Material     = new_material;

    addMeshUnit(new_unit);
}

void CStaticMesh::removeMeshUnit(u32 id)
{
    if(id >= MeshUnits.size())
        return;

    MeshUnits[id].VertexBuffer->release();
    if(MeshUnits[id].Material)
        MeshUnits[id].Material->release();

    MeshUnits.erase(MeshUnits.begin()+id);
}

u32  CStaticMesh::getMeshUnitsCount()
{
    return MeshUnits.size();
}

const SMeshUnit* CStaticMesh::getMeshUnit(u32 id)
{
    if(id >= MeshUnits.size())
        return NULL;
    return &MeshUnits[id];
}

IVertexBuffer* CStaticMesh::getVertexBuffer(u32 id)
{
    if(id >= MeshUnits.size())
        return NULL;
    return MeshUnits[id].VertexBuffer;
}

IMaterial* CStaticMesh::getMaterial(u32 id)
{
    if(id >= MeshUnits.size())
        return NULL;
    return MeshUnits[id].Material;
}
void CStaticMesh::setVertexBuffer(u32 id,IVertexBuffer* new_vertexbuffer)
{
    if(id >= MeshUnits.size())
        return;
    SMeshUnit* MeshUnit = &MeshUnits[id];

    if(MeshUnit == NULL)
        return;

    if(new_vertexbuffer == NULL)
    {
        removeMeshUnit(id);
    }
    else
    {
        new_vertexbuffer->capture();
        MeshUnit->VertexBuffer->release();
        MeshUnit->VertexBuffer = new_vertexbuffer;
    }
}
void CStaticMesh::setMaterial(u32 id,IMaterial* new_material)
{
    if(id >= MeshUnits.size())
        return;
    SMeshUnit* MeshUnit = &MeshUnits[id];

    if(MeshUnit->Material)
        MeshUnit->Material->release();
    if(new_material)
        new_material->capture();
    MeshUnit->Material = new_material;

}
IStaticMesh* CStaticMesh::getCopy()
{
    return NULL;
}

}
}
