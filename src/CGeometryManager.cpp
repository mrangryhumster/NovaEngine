#include "CGeometryManager.h"

#include "renderer\CVertexBuffer.h"

namespace novaengine
{

CGeometryManager::CGeometryManager(IResourceManager* ResourceManager_param):
    ResourceManager(ResourceManager_param)
{

}
//--------------------------------------------------------------------------------------------------------
CGeometryManager::~CGeometryManager()
{
    //dtor
}
//--------------------------------------------------------------------------------------------------------
renderer::IStaticMesh* CGeometryManager::createCubeMesh()
{
    core::vector3f Positions[24];
    core::vector2f TexCoords[24];
    core::vector3f Normals[24];
    u16 indices[36] = {0};

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

    indices[0] = 0;
    indices[1] = 1;
    indices[2] = 2;
    indices[3] = 2;
    indices[4] = 3;
    indices[5] = 0;

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

    indices[6]  = 4;
    indices[7]  = 5;
    indices[8]  = 6;
    indices[9]  = 6;
    indices[10] = 7;
    indices[11] = 4;

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

    indices[12] = 8;
    indices[13] = 9;
    indices[14] = 10;
    indices[15] = 10;
    indices[16] = 11;
    indices[17] = 8;

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

    indices[18]  = 12;
    indices[19]  = 13;
    indices[20]  = 14;
    indices[21]  = 14;
    indices[22]  = 15;
    indices[23]  = 12;

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

    indices[24] = 16;
    indices[25] = 17;
    indices[26] = 18;
    indices[27] = 18;
    indices[28] = 19;
    indices[29] = 16;

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

    indices[30]  = 20;
    indices[31]  = 21;
    indices[32]  = 22;
    indices[33]  = 22;
    indices[34]  = 23;
    indices[35]  = 20;

    renderer::IVertexBuffer* VertexBuffer = renderer::getRenderer()->GenVertexBuffer();
//REPAIR
//    VertexBuffer->addPositions(Positions,24);
//    VertexBuffer->addUVs(TexCoords,24);
//    VertexBuffer->addNormals(Normals,24);
//    VertexBuffer->addIndices(indices,36);
//    VertexBuffer->setBufferType(renderer::EVBT_RAWDATA);
//    VertexBuffer->setVertexFormat(renderer::EVA_POSITION | renderer::EVA_TEXCOORD | renderer::EVA_NORMAL);
 //   VertexBuffer->setPrimitiveType(renderer::EPT_TRIANGLE);


    //----------------
    renderer::IStaticMesh* Mesh = ResourceManager->createStaticMesh();
    Mesh->addMeshUnit(VertexBuffer,0);
    VertexBuffer->release();
    //----------------

    return Mesh;
}
//--------------------------------------------------------------------------------------------------------
renderer::IStaticMesh* CGeometryManager::createGridMesh(core::dim2f CellSize,core::dim2u CellCount)
{
    //----------------------------
    renderer::IVertexBuffer* VertexBuffer = renderer::getRenderer()->GenVertexBuffer();
    if(VertexBuffer == NULL)
        return NULL;
    //----------------------------
    //VertexBuffer->getVerticesArray()->reserve(CellCount.width * CellCount.height * 4);


    u16* indices = new u16[CellCount.width * CellCount.height * 6];

    u32 vp = 0;
    u32 ip = 0;

    for(u32 y_tile = 0; y_tile < CellCount.height; y_tile++)
    {
        for(u32 x_tile = 0; x_tile < CellCount.width; x_tile++)
        {
            renderer::SVertex Quad[4];

            Quad[0].Position.set((x_tile * CellSize.width),(y_tile * CellSize.height)+CellSize.height,0);
            Quad[1].Position.set((x_tile * CellSize.width),(y_tile * CellSize.height),0);
            Quad[2].Position.set((x_tile * CellSize.width)+CellSize.width,(y_tile * CellSize.height),0);
            Quad[3].Position.set((x_tile * CellSize.width)+CellSize.width,(y_tile * CellSize.height)+CellSize.height,0);

            Quad[0].TexCoord.set(0,0);
            Quad[1].TexCoord.set(0,1);
            Quad[2].TexCoord.set(1,1);
            Quad[3].TexCoord.set(1,0);

            Quad[0].Normal = Quad[1].Normal = Quad[2].Normal = Quad[3].Normal = core::vector3f(0,0,1);

            indices[ip + 0] = vp + 0;
            indices[ip + 1] = vp + 1;
            indices[ip + 2] = vp + 2;
            indices[ip + 3] = vp + 2;
            indices[ip + 4] = vp + 3;
            indices[ip + 5] = vp + 0;

            vp+=4;
            ip+=6;
        }
    }
//REPAIR
//    VertexBuffer->setIndices(indices,ip);
//    VertexBuffer->setPrimitiveType(renderer::EPT_TRIANGLE);
//    VertexBuffer->setVertexFormat(renderer::EVA_POSITION | renderer::EVA_TEXCOORD | renderer::EVA_NORMAL);

    //Mesh->moveVertices(core::vector3<f32>(-(CellSize.width*CellCount.width)/2,-(CellSize.height*CellCount.height)/2,0));

    VertexBuffer->setBufferType(renderer::EVBT_RAWDATA);

    delete[] indices;

    //----------------
    renderer::IStaticMesh* Mesh = ResourceManager->createStaticMesh();
    Mesh->addMeshUnit(VertexBuffer,0);
    VertexBuffer->release();
    //----------------

    return Mesh;
}
//--------------------------------------------------------------------------------------------------------
renderer::IStaticMesh* CGeometryManager::createSphereMesh(float Radius,unsigned int Resolution)
{
    if(Resolution < 4)
        Resolution = 4;

    u32 VCount = (Resolution+1) * (Resolution+1);
    u32 ICount = (Resolution*Resolution) * 6;

    core::vector3f* Positions    = new core::vector3f[VCount];
    core::vector2f* UVs          = new core::vector2f[VCount];
    core::vector3f* Normals      = new core::vector3f[VCount];

    u16* Indices              = new u16[ICount];

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
            UVs[v].set(float(x) / Resolution,float(y) / Resolution);
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

    renderer::IVertexBuffer* va = renderer::getRenderer()->GenVertexBuffer();

//REPAIR
    //va->addVertices(vertex,VCount);
    //va->setPositions(Positions  ,VCount);
    //va->setUVs      (UVs        ,VCount);
    //va->setNormals  (Normals    ,VCount);

    //va->setIndices(Indices,ICount);

    //va->setBufferType(renderer::EVBT_RAWDATA);
    //va->setVertexFormat(renderer::EVA_POSITION | renderer::EVA_TEXCOORD  | renderer::EVA_NORMAL);
    //va->setPrimitiveType(renderer::EPT_TRIANGLE);

    delete[] Positions;
    delete[] UVs;
    delete[] Normals;

    delete[] Indices;

    //----------------
    renderer::IStaticMesh* Mesh = ResourceManager->createStaticMesh();
    Mesh->addMeshUnit(va,0);
    va->release();
    //----------------

    return Mesh;
}
//--------------------------------------------------------------------------------------------------------
}
