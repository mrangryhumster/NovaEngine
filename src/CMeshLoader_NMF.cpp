#include "CMeshLoader_NMF.h"
#define _NE_INCLUDE_MESH_LOADER_NMF
#ifdef _NE_INCLUDE_MESH_LOADER_NMF

#include "CStaticMesh.h"
#include "CMeshBuffer.h"

namespace novaengine
{

	CMeshLoader_NMF::CMeshLoader_NMF(io::IFileSystem* FileSystem, IResourceManager* ResourceManager) :
		m_FileSystem(FileSystem),
		m_ResourceManager(ResourceManager)
	{
		//ctor
	}

	CMeshLoader_NMF::~CMeshLoader_NMF()
	{
		//dtor
	}

	bool CMeshLoader_NMF::isSupported(const char * file_extension)
	{
		if (strcmp(file_extension, "nmf") == 0)
			return 1;
		return 0;
	}

	bool CMeshLoader_NMF::isSupported(io::IFile * file)
	{
		S_NMF_Header NMF_Header;
		file->read(&NMF_Header, sizeof(S_NMF_Header));
		file->seek(file->getLastPos());
		if (NMF_Header.magic == NMF_MAGIC)
			return true;
		return false;
	}

	renderer::IStaticMesh * CMeshLoader_NMF::loadStaticMesh(const char * path)
	{
		io::IFile* mesh_file = m_FileSystem->open_file(path);

		if (!mesh_file)
			return nullptr;

		renderer::IStaticMesh* mesh = LoadNMF(mesh_file);

		mesh_file->release();

		if (!mesh)
			return nullptr;

		return mesh;
	}

	renderer::IStaticMesh * CMeshLoader_NMF::loadStaticMesh(io::IFile * file, const char * hint)
	{
		u32 FileStart = file->getPos();
		renderer::IStaticMesh* mesh = LoadNMF(file);
		file->seek(FileStart);
		return mesh;
	}

	renderer::IStaticMesh * CMeshLoader_NMF::LoadNMF(io::IFile * file)
	{
		S_NMF_Header NMF_Header;
		file->read(&NMF_Header, sizeof(S_NMF_Header));

		if (NMF_Header.magic != NMF_MAGIC)
		{
			LOG_ERROR("Failed to load mesh - corrupted data[not nmf]\n");
			return nullptr;
		}
		else
		{
			LOG_ENGINE_DEBUG("Detected NMF version : 0x%.4x\n", NMF_Header.version);
		}

		if (NMF_Header.version > 0x0001)
		{
			LOG_ERROR("Failed to load mesh - unsupported format version\n");
			return nullptr;
		}

		renderer::IStaticMesh* StaticMesh = m_ResourceManager->createStaticMesh();
		if (NMF_Header.version == 0x0001)
		{

			//read file description
			S_NMF_Description_v0001 NMF_Desc;
			file->read(&NMF_Desc, sizeof(S_NMF_Description_v0001));
			LOG_ENGINE_DEBUG("NMF verticles : %d\n", NMF_Desc.vertices_count);

			renderer::IMeshBuffer* MeshBuffer = m_ResourceManager->createMeshBuffer();

			MeshBuffer->lock();

			s32 PositionsBufferIdx = -1;
			s32 NormalsBufferIdx = -1;
			s32 BinormalsBufferIdx = -1;
			s32 TangentBufferIdx = -1;
			s32 ColorsBufferIdx = -1;
			s32 TexcoordsBufferIdx = -1;

			if (NMF_Desc.contain_flags & NMF_HAS_POSITIONS_DATA)
			{
				LOG_ENGINE_DEBUG(" - NMF_HAS_POSITIONS_DATA\n");
				PositionsBufferIdx = MeshBuffer->createBuffer(0, renderer::EVA_POSITION);
				MeshBuffer->resizeBuffer(PositionsBufferIdx, sizeof(core::vector3f) * NMF_Desc.vertices_count);
				file->read(MeshBuffer->getBufferData(PositionsBufferIdx), sizeof(core::vector3f) * NMF_Desc.vertices_count);
			}
			if (NMF_Desc.contain_flags & NMF_HAS_NORMALS_DATA)
			{
				LOG_ENGINE_DEBUG(" - NMF_HAS_NORMALS_DATA\n");
				NormalsBufferIdx = MeshBuffer->createBuffer(1, renderer::EVA_NORMAL);
				MeshBuffer->resizeBuffer(NormalsBufferIdx, sizeof(core::vector3f) * NMF_Desc.vertices_count);
				file->read(MeshBuffer->getBufferData(NormalsBufferIdx), sizeof(core::vector3f) * NMF_Desc.vertices_count);
			}
			if (NMF_Desc.contain_flags & NMF_HAS_BINORMALS_DATA)
			{
				LOG_ENGINE_DEBUG(" - NMF_HAS_BINORMALS_DATA\n");
				BinormalsBufferIdx = MeshBuffer->createBuffer(2, renderer::EVA_BINORMAL);
				MeshBuffer->resizeBuffer(BinormalsBufferIdx, sizeof(core::vector3f) * NMF_Desc.vertices_count);
				file->read(MeshBuffer->getBufferData(BinormalsBufferIdx), sizeof(core::vector3f) * NMF_Desc.vertices_count);
			}
			if (NMF_Desc.contain_flags & NMF_HAS_TANGENTS_DATA)
			{
				LOG_ENGINE_DEBUG(" - NMF_HAS_TANGENTS_DATA\n");
				TangentBufferIdx = MeshBuffer->createBuffer(3, renderer::EVA_TANGENT);
				MeshBuffer->resizeBuffer(TangentBufferIdx, sizeof(core::vector3f) * NMF_Desc.vertices_count);
				file->read(MeshBuffer->getBufferData(TangentBufferIdx), sizeof(core::vector3f) * NMF_Desc.vertices_count);
			}
			if (NMF_Desc.contain_flags & NMF_HAS_COLORS_DATA)
			{
				LOG_ENGINE_DEBUG(" - NMF_HAS_COLORS_DATA\n");
				ColorsBufferIdx = MeshBuffer->createBuffer(4, renderer::EVA_COLOR);
				MeshBuffer->resizeBuffer(ColorsBufferIdx, sizeof(core::color4f) * NMF_Desc.vertices_count);
				file->read(MeshBuffer->getBufferData(ColorsBufferIdx), sizeof(core::color4f) * NMF_Desc.vertices_count);
			}
			if (NMF_Desc.contain_flags & NMF_HAS_TEXCOORDS_DATA)
			{
				LOG_ENGINE_DEBUG(" - NMF_HAS_TEXCOORDS_DATA\n");
				TexcoordsBufferIdx = MeshBuffer->createBuffer(5, renderer::EVA_TEXCOORD);
				MeshBuffer->resizeBuffer(TexcoordsBufferIdx, sizeof(core::vector2f) * NMF_Desc.vertices_count);
				file->read(MeshBuffer->getBufferData(TexcoordsBufferIdx), sizeof(core::vector2f) * NMF_Desc.vertices_count);
			}

			if (NMF_Desc.contain_flags & NMF_HAS_INDICES_DATA)
			{
				LOG_ENGINE_DEBUG(" - NMF_HAS_INDICES_DATA\n");
				MeshBuffer->resizeIndicesBuffer(sizeof(u32) * NMF_Desc.indices_count);
				file->read(MeshBuffer->getIndicesBufferData(), sizeof(u32) * NMF_Desc.indices_count);

				MeshBuffer->setIndicesBuffer32(true);
			}
			MeshBuffer->setPrimitiveType(renderer::EPT_TRIANGLE);
			MeshBuffer->unlock();

			StaticMesh->addMeshBuffer(MeshBuffer);
			MeshBuffer->release();
		}
		return StaticMesh;
	}

}
#endif // _NE_INCLUDE_MESH_LOADER_NMF
