#include "CGeometryManager.h"

#include "CMeshBuffer.h"

namespace novaengine
{

	CGeometryManager::CGeometryManager(IResourceManager* ResourceManager_param) :
		ResourceManager(ResourceManager_param)
	{

	}
	//-----------------------------------------------------------------------------------------------
	CGeometryManager::~CGeometryManager()
	{
		//dtor
	}
	//-----------------------------------------------------------------------------------------------
	renderer::IStaticMesh* CGeometryManager::createCubeMesh(f32 Size)
	{
		const core::vector3f Positions[24] = {
			{-Size,-Size, Size },{ Size,-Size, Size },{ Size, Size, Size },{-Size, Size, Size }, //front
			{ Size,-Size,-Size },{-Size,-Size,-Size },{-Size, Size,-Size },{ Size, Size,-Size }, //back
			{-Size, Size, Size },{ Size, Size, Size },{ Size, Size,-Size },{-Size, Size,-Size }, //top
			{ Size,-Size, Size },{-Size,-Size, Size },{-Size,-Size,-Size },{ Size,-Size,-Size }, //bottom
			{-Size,-Size,-Size },{-Size,-Size, Size },{-Size, Size, Size },{-Size, Size,-Size }, //left
			{ Size,-Size, Size },{ Size,-Size,-Size },{ Size, Size,-Size },{ Size, Size, Size }  //right
		};
		const core::vector2f TexCoords[24] = {
			{ 0, 0 },{ 1, 0 },{ 1, 1 },{ 0, 1 },
			{ 0, 0 },{ 1, 0 },{ 1, 1 },{ 0, 1 },
			{ 0, 0 },{ 1, 0 },{ 1, 1 },{ 0, 1 },
			{ 0, 0 },{ 1, 0 },{ 1, 1 },{ 0, 1 },
			{ 0, 0 },{ 1, 0 },{ 1, 1 },{ 0, 1 },
			{ 0, 0 },{ 1, 0 },{ 1, 1 },{ 0, 1 },
		};
		const core::vector3f Normals[24] = {
			{ 0, 0, 1},{ 0, 0, 1},{ 0, 0, 1},{ 0, 0, 1},
			{ 0, 0,-1},{ 0, 0,-1},{ 0, 0,-1},{ 0, 0,-1},
			{ 0, 1, 0},{ 0, 1, 0},{ 0, 1, 0},{ 0, 1, 0},
			{ 0,-1, 0},{ 0,-1, 0},{ 0,-1, 0},{ 0,-1, 0},
			{-1, 0, 0},{-1, 0, 0},{-1, 0, 0},{-1, 0, 0},
			{ 1, 0, 0},{ 1, 0, 0},{ 1, 0, 0},{ 1, 0, 0}
		};
		u32 Indices[36] = {
			 0, 1, 2, 2, 3, 0,//front
			 4, 5, 6, 6, 7, 4,//back
			 8, 9,10,10,11, 8,//top
			12,13,14,14,15,12,//bottom
			16,17,18,18,19,16,//left
			20,21,22,22,23,20 //right
		};


		renderer::IMeshBuffer* MeshBuffer = renderer::getRenderer()->createMeshBuffer();

		MeshBuffer->lock();

		s32 PositionsBufferIdx = MeshBuffer->createBuffer(0, renderer::EVA_POSITION);
		s32 NormalBufferIdx = MeshBuffer->createBuffer(1, renderer::EVA_NORMAL);
		s32 TexCoordBufferIdx = MeshBuffer->createBuffer(5, renderer::EVA_TEXCOORD);

		MeshBuffer->addBufferData(PositionsBufferIdx, Positions, 24 * sizeof(core::vector3f));
		MeshBuffer->addBufferData(NormalBufferIdx, Normals, 24 * sizeof(core::vector3f));
		MeshBuffer->addBufferData(TexCoordBufferIdx, TexCoords, 24 * sizeof(core::vector2f));

		MeshBuffer->setIndicesBufferData(Indices, 36 * sizeof(u32));
		MeshBuffer->setIndicesBuffer32(true);

		MeshBuffer->setPrimitiveType(renderer::EPT_TRIANGLE);

		MeshBuffer->unlock();

		//----------------
		renderer::IStaticMesh* Mesh = ResourceManager->createStaticMesh();
		Mesh->addMeshBuffer(MeshBuffer);
		MeshBuffer->release();
		//----------------
		LOG_ENGINE_DEBUG("geometry:generated shape:cube\n");
		return Mesh;
	}
	//-----------------------------------------------------------------------------------------------
	renderer::IStaticMesh* CGeometryManager::createGridMesh(core::dim2f CellSize, core::dim2u CellCount)
	{
		renderer::IMeshBuffer* MeshBuffer = renderer::getRenderer()->createMeshBuffer();
		MeshBuffer->lock();
		//----------------------------
		s32 Position_buffer_idx = MeshBuffer->createBuffer(0, renderer::EVA_POSITION);
		s32 TexCoord_buffer_idx = MeshBuffer->createBuffer(1, renderer::EVA_TEXCOORD);
		//----------------------------

		u32* Indices = new u32[CellCount.width * CellCount.height * 6];

		u32 VCount = 0;
		u32 ICount = 0;

		core::vector3f Shift(-(((f32)CellCount.width / 2) * CellSize.width), 0, -(((f32)CellCount.width / 2) * CellSize.width));

		for (u32 y_tile = 0; y_tile < CellCount.height; y_tile++)
		{
			for (u32 x_tile = 0; x_tile < CellCount.width; x_tile++)
			{
				core::vector3f Positions[4];
				core::vector2f TexCoords[4];

				Positions[0].set((x_tile * CellSize.width), 0, (y_tile * CellSize.height) + CellSize.height).add(Shift);
				Positions[1].set((x_tile * CellSize.width), 0, (y_tile * CellSize.height)).add(Shift);
				Positions[2].set((x_tile * CellSize.width) + CellSize.width, 0, (y_tile * CellSize.height)).add(Shift);
				Positions[3].set((x_tile * CellSize.width) + CellSize.width, 0, (y_tile * CellSize.height) + CellSize.height).add(Shift);

				TexCoords[0].set(0, 0);
				TexCoords[1].set(0, 1);
				TexCoords[2].set(1, 1);
				TexCoords[3].set(1, 0);

				Indices[ICount + 0] = VCount + 0;
				Indices[ICount + 1] = VCount + 3;
				Indices[ICount + 2] = VCount + 2;
				Indices[ICount + 3] = VCount + 2;
				Indices[ICount + 4] = VCount + 1;
				Indices[ICount + 5] = VCount + 0;

				VCount += 4;
				ICount += 6;

				MeshBuffer->addBufferData(Position_buffer_idx, Positions, 4 * sizeof(core::vector3f));
				MeshBuffer->addBufferData(TexCoord_buffer_idx, TexCoords, 4 * sizeof(core::vector2f));
			}
		}

		MeshBuffer->setIndicesBufferData(Indices, ICount * sizeof(u32));
		MeshBuffer->setIndicesBuffer32(true);

		MeshBuffer->setPrimitiveType(renderer::EPT_TRIANGLE);

		delete[] Indices;
		MeshBuffer->unlock();
		//----------------
		renderer::IStaticMesh* Mesh = ResourceManager->createStaticMesh();
		Mesh->addMeshBuffer(MeshBuffer);
		MeshBuffer->release();
		//----------------
		LOG_ENGINE_DEBUG("geometry:generated shape:grid vcount:%d icount:%d\n", VCount, ICount);
		return Mesh;
	}
	//-----------------------------------------------------------------------------------------------
	renderer::IStaticMesh* CGeometryManager::createSphereMesh(f32 Radius, u32 Resolution)
	{
		if (Resolution < 4)
			Resolution = 4;

		u32 VCount = (Resolution + 1) * (Resolution + 1);
		u32 ICount = (Resolution*Resolution) * 6;

		core::vector3f* Positions = new core::vector3f[VCount];
		core::vector2f* TexCoords = new core::vector2f[VCount];
		core::vector3f* Normals = new core::vector3f[VCount];

		u32* Indices = new u32[ICount];

		//(x, y, z) = (sin(Pi * m/M) cos(2Pi * n/N), sin(Pi * m/M) sin(2Pi * n/N), cos(Pi * m/M))
		u32 v = 0;
		for (u32 x = 0; x < Resolution + 1; x++)
		{
			f32 AngleX = 2 * math::PI64 * x / Resolution;

			for (u32 y = 0; y < Resolution + 1; y++)
			{

				float  AngleY = math::PI64 * y / Resolution;

				core::vector3<f32> result_vertex(sinf(AngleY) * cosf(AngleX) * Radius, cosf(AngleY)* Radius, sinf(AngleX) * sinf(AngleY)* Radius);
				Positions[v].set(result_vertex);
				TexCoords[v].set(float(x) / Resolution, float(y) / Resolution);
				Normals[v] = result_vertex.normalize();
				v++;
			}
		}

		v = 0;

		for (u32 x = 0; x < Resolution; x++)
		{
			for (u32 y = 0; y < Resolution; y++)
			{
				u32 vertex_num = x + (y*(Resolution + 1));
				Indices[v] = vertex_num;
				Indices[v + 1] = vertex_num + (Resolution + 1);
				Indices[v + 2] = vertex_num + (Resolution + 2);
				Indices[v + 3] = vertex_num + (Resolution + 2);
				Indices[v + 4] = vertex_num + 1;
				Indices[v + 5] = vertex_num;
				v += 6;
			}
		}

		renderer::IMeshBuffer* MeshBuffer = renderer::getRenderer()->createMeshBuffer();

		MeshBuffer->lock();

		s32 Position_buffer_idx = MeshBuffer->createBuffer(0, renderer::EVA_POSITION);
		s32 TexCoord_buffer_idx = MeshBuffer->createBuffer(1, renderer::EVA_TEXCOORD);


		MeshBuffer->setBufferData(Position_buffer_idx, Positions, VCount * sizeof(core::vector3f));
		MeshBuffer->setBufferData(TexCoord_buffer_idx, TexCoords, VCount * sizeof(core::vector2f));

		MeshBuffer->setIndicesBufferData(Indices, ICount * sizeof(u32));
		MeshBuffer->setIndicesBuffer32(true);

		MeshBuffer->setPrimitiveType(renderer::EPT_TRIANGLE);

		MeshBuffer->unlock();


		delete[] Positions;
		delete[] TexCoords;
		delete[] Normals;

		delete[] Indices;

		//----------------
		renderer::IStaticMesh* Mesh = ResourceManager->createStaticMesh();
		Mesh->addMeshBuffer(MeshBuffer);
		MeshBuffer->release();
		//----------------


		LOG_ENGINE_DEBUG("geometry:generated shape:sphere vcount:%d icount:%d\n", VCount, ICount);
		return Mesh;
	}
	//-----------------------------------------------------------------------------------------------
}
