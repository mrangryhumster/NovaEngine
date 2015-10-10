#include "CGeometryManager.h"

#include "CMeshBuffer.h"

namespace novaengine
{

CGeometryManager::CGeometryManager(IResourceManager* ResourceManager_param):
    ResourceManager(ResourceManager_param)
{

}
//-----------------------------------------------------------------------------------------------
CGeometryManager::~CGeometryManager()
{
    //dtor
}
//-----------------------------------------------------------------------------------------------
renderer::IStaticMesh* CGeometryManager::createCubeMesh()
{
    core::vector3f Positions[24];
    core::vector2f TexCoords[24];
    core::vector3f Normals[24];
    u32 Indices[36];

	
    //front
    Positions[0].set(-1,-1, 1);
    Positions[1].set( 1,-1, 1);
    Positions[2].set( 1, 1, 1);
    Positions[3].set(-1, 1, 1);

    TexCoords[0].set(0,0);
    TexCoords[1].set(1,0);
    TexCoords[2].set(1,1);
    TexCoords[3].set(0,1);

    Normals[0] =
        Normals[1] =
            Normals[2] =
                Normals[3] =
                    core::vector3f(0,0,1);

    Indices[0] = 0;
    Indices[1] = 1;
    Indices[2] = 2;
    Indices[3] = 2;
    Indices[4] = 3;
    Indices[5] = 0;

    //back
    Positions[4].set( 1,-1,-1);
    Positions[5].set(-1,-1,-1);
    Positions[6].set(-1, 1,-1);
    Positions[7].set( 1, 1,-1);

    TexCoords[4].set(0,0);
    TexCoords[5].set(1,0);
    TexCoords[6].set(1,1);
    TexCoords[7].set(0,1);

    Normals[4] =
        Normals[5] =
            Normals[6] =
                Normals[7] =
                    core::vector3f(0,0,-1);

    Indices[6]  = 4;
    Indices[7]  = 5;
    Indices[8]  = 6;
    Indices[9]  = 6;
    Indices[10] = 7;
    Indices[11] = 4;

    //top
    Positions[8].set (-1, 1, 1);
    Positions[9].set ( 1, 1, 1);
    Positions[10].set( 1, 1,-1);
    Positions[11].set(-1, 1,-1);

    TexCoords[8].set (0,0);
    TexCoords[9].set (1,0);
    TexCoords[10].set(1,1);
    TexCoords[11].set(0,1);

    Normals[8] =
        Normals[9] =
            Normals[10] =
                Normals[11] =
                    core::vector3f(0,1,0);

    Indices[12] = 8;
    Indices[13] = 9;
    Indices[14] = 10;
    Indices[15] = 10;
    Indices[16] = 11;
    Indices[17] = 8;

    //bottom
    Positions[12].set( 1,-1, 1);
    Positions[13].set(-1,-1, 1);
    Positions[14].set(-1,-1,-1);
    Positions[15].set( 1,-1,-1);

    TexCoords[12].set(0,0);
    TexCoords[13].set(1,0);
    TexCoords[14].set(1,1);
    TexCoords[15].set(0,1);

    Normals[12] =
        Normals[13] =
            Normals[14] =
                Normals[15] =
                    core::vector3f(0,-1,0);

    Indices[18]  = 12;
    Indices[19]  = 13;
    Indices[20]  = 14;
    Indices[21]  = 14;
    Indices[22]  = 15;
    Indices[23]  = 12;

    //left
    Positions[16].set(-1,-1,-1);
    Positions[17].set(-1,-1, 1);
    Positions[18].set(-1, 1, 1);
    Positions[19].set(-1, 1,-1);

    TexCoords[16].set(0,0);
    TexCoords[17].set(1,0);
    TexCoords[18].set(1,1);
    TexCoords[19].set(0,1);

    Normals[16] =
        Normals[17] =
            Normals[18] =
                Normals[19] =
                    core::vector3f(-1,0,0);

    Indices[24] = 16;
    Indices[25] = 17;
    Indices[26] = 18;
    Indices[27] = 18;
    Indices[28] = 19;
    Indices[29] = 16;

    //right
    Positions[20].set( 1,-1, 1);
    Positions[21].set( 1,-1,-1);
    Positions[22].set( 1, 1,-1);
    Positions[23].set( 1, 1, 1);

    TexCoords[20].set(0,0);
    TexCoords[21].set(1,0);
    TexCoords[22].set(1,1);
    TexCoords[23].set(0,1);

    Normals[20] =
        Normals[21] =
            Normals[22] =
                Normals[23] =
                    core::vector3f(1,0,0);

    Indices[30]  = 20;
    Indices[31]  = 21;
    Indices[32]  = 22;
    Indices[33]  = 22;
    Indices[34]  = 23;
    Indices[35]  = 20;
	
    renderer::IMeshBuffer* MeshBuffer = renderer::getRenderer()->GenMeshBuffer();

    MeshBuffer->addBufferData(renderer::EVA_POSITION,Positions,24 * sizeof(core::vector3f));
	MeshBuffer->addBufferData(renderer::EVA_TEXCOORD,TexCoords,24 * sizeof(core::vector2f));
	MeshBuffer->addBufferData(renderer::EVA_NORMAL  ,Normals  ,24 * sizeof(core::vector3f));

	MeshBuffer->setIndicesBufferData(Indices,36 * sizeof(u32));
    MeshBuffer->setIndicesBufferType(NTYPE_u32);

    MeshBuffer->setPrimitiveType(renderer::EPT_TRIANGLE);
    MeshBuffer->setVertexFormat(renderer::SVertexFormat(renderer::EVA_POSITION | renderer::EVA_TEXCOORD | renderer::EVA_NORMAL));
	
    //----------------
    renderer::IStaticMesh* Mesh = ResourceManager->createStaticMesh();
    Mesh->addMeshBuffer(MeshBuffer);
    MeshBuffer->release();
    //----------------
    LOG_ENGINE_DEBUG("geometry:generated shape:cube\n");
    return Mesh;
}
//-----------------------------------------------------------------------------------------------
renderer::IStaticMesh* CGeometryManager::createGridMesh(core::dim2f CellSize,core::dim2u CellCount)
{
    //----------------------------
    renderer::IMeshBuffer* MeshBuffer = renderer::getRenderer()->GenMeshBuffer();
    //----------------------------

    u32* Indices = new u32[CellCount.width * CellCount.height * 6];

    u32 VCount = 0;
    u32 ICount = 0;

    core::vector3f Shift(-(((f32)CellCount.width/2) * CellSize.width),0,-(((f32)CellCount.width/2) * CellSize.width));

    for(u32 y_tile = 0; y_tile < CellCount.height; y_tile++)
    {
        for(u32 x_tile = 0; x_tile < CellCount.width; x_tile++)
        {
            core::vector3f Positions[4];
            core::vector2f TexCoords[4];

            Positions[0].set((x_tile * CellSize.width),0,(y_tile * CellSize.height)+CellSize.height).add(Shift);
            Positions[1].set((x_tile * CellSize.width),0,(y_tile * CellSize.height)).add(Shift);
            Positions[2].set((x_tile * CellSize.width)+CellSize.width,0,(y_tile * CellSize.height)).add(Shift);
            Positions[3].set((x_tile * CellSize.width)+CellSize.width,0,(y_tile * CellSize.height)+CellSize.height).add(Shift);

            TexCoords[0].set(0,0);
            TexCoords[1].set(0,1);
            TexCoords[2].set(1,1);
            TexCoords[3].set(1,0);

            Indices[ICount + 0] = VCount + 0;
            Indices[ICount + 1] = VCount + 3;
            Indices[ICount + 2] = VCount + 2;
            Indices[ICount + 3] = VCount + 2;
            Indices[ICount + 4] = VCount + 1;
            Indices[ICount + 5] = VCount + 0;

            VCount+=4;
            ICount+=6;

            MeshBuffer->addBufferData(renderer::EVA_POSITION,Positions,4 * sizeof(core::vector3f));
            MeshBuffer->addBufferData(renderer::EVA_TEXCOORD,TexCoords,4 * sizeof(core::vector2f));
        }
    }

    MeshBuffer->setIndicesBufferData(Indices,ICount * sizeof(u32));
    MeshBuffer->setIndicesBufferType(NTYPE_u32);

    MeshBuffer->setPrimitiveType(renderer::EPT_TRIANGLE);
    MeshBuffer->setVertexFormat(renderer::SVertexFormat(renderer::EVA_POSITION | renderer::EVA_TEXCOORD));

    delete[] Indices;

    //----------------
    renderer::IStaticMesh* Mesh = ResourceManager->createStaticMesh();
    Mesh->addMeshBuffer(MeshBuffer);
    MeshBuffer->release();
    //----------------
    LOG_ENGINE_DEBUG("geometry:generated shape:grid vcount:%d icount:%d\n",VCount,ICount);
    return Mesh;
}
//-----------------------------------------------------------------------------------------------
renderer::IStaticMesh* CGeometryManager::createSphereMesh(float Radius,unsigned int Resolution)
{
    if(Resolution < 4)
        Resolution = 4;

    u32 VCount = (Resolution+1) * (Resolution+1);
    u32 ICount = (Resolution*Resolution) * 6;

    core::vector3f* Positions    = new core::vector3f[VCount];
    core::vector2f* TexCoords    = new core::vector2f[VCount];
    core::vector3f* Normals      = new core::vector3f[VCount];

    u32* Indices                 = new u32[ICount];

    //(x, y, z) = (sin(Pi * m/M) cos(2Pi * n/N), sin(Pi * m/M) sin(2Pi * n/N), cos(Pi * m/M))
    u32 v = 0;
    for(u32 x = 0; x < Resolution+1; x++)
    {
        f32 AngleX = 2 * math::PI64 * x / Resolution;

        for(u32 y = 0; y < Resolution+1; y++)
        {

            float  AngleY = math::PI64 * y / Resolution;

            core::vector3<f32> result_vertex(sinf(AngleY) * cosf(AngleX) * Radius, cosf(AngleY)* Radius, sinf(AngleX) * sinf(AngleY)* Radius);
            Positions[v].set(result_vertex);
            TexCoords[v].set(float(x) / Resolution,float(y) / Resolution);
            Normals[v] = result_vertex.normalize();
            v++;
        }
    }

    v = 0;

    for(u32 x = 0; x < Resolution; x++)
    {
        for(u32 y = 0; y < Resolution; y++)
        {
            u32 vertex_num = x + (y*(Resolution+1));
            Indices[v]   = vertex_num;
            Indices[v+1] = vertex_num+(Resolution+1);
            Indices[v+2] = vertex_num+(Resolution+2);
            Indices[v+3] = vertex_num+(Resolution+2);
            Indices[v+4] = vertex_num+1;
            Indices[v+5] = vertex_num;
            v+=6;
        }
    }

    renderer::IMeshBuffer* MeshBuffer = renderer::getRenderer()->GenMeshBuffer();

    MeshBuffer->setBufferData(renderer::EVA_POSITION,Positions,VCount * sizeof(core::vector3f));
    MeshBuffer->setBufferData(renderer::EVA_TEXCOORD,TexCoords,VCount * sizeof(core::vector2f));

    MeshBuffer->setIndicesBufferData(Indices,ICount * sizeof(u32));
    MeshBuffer->setIndicesBufferType(NTYPE_u32);

    MeshBuffer->setPrimitiveType(renderer::EPT_TRIANGLE);
    MeshBuffer->setVertexFormat(renderer::SVertexFormat(renderer::EVA_POSITION | renderer::EVA_TEXCOORD));

    delete[] Positions;
    delete[] TexCoords;
    delete[] Normals;

    delete[] Indices;

    //----------------
    renderer::IStaticMesh* Mesh = ResourceManager->createStaticMesh();
    Mesh->addMeshBuffer(MeshBuffer);
    MeshBuffer->release();
    //----------------


    LOG_ENGINE_DEBUG("geometry:generated shape:sphere vcount:%d icount:%d\n",VCount,ICount);
    return Mesh;
}
//-----------------------------------------------------------------------------------------------
}
