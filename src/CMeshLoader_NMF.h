#pragma once

#include "CompileConfig.h"
#define _NE_INCLUDE_MESH_LOADER_NMF
#ifdef _NE_INCLUDE_MESH_LOADER_NMF

#include "IFileSystem.h"
#include "IStaticMeshLoader.h"
#include "IResourceManager.h"

#include "NLogger.h"

namespace novaengine
{

#define NMF_MAGIC 0x464D454E

#define NMF_HAS_POSITIONS_DATA 1
#define NMF_HAS_NORMALS_DATA    2
#define NMF_HAS_BINORMALS_DATA  4
#define NMF_HAS_TANGENTS_DATA   8
#define NMF_HAS_COLORS_DATA     16
#define NMF_HAS_TEXCOORDS_DATA  32

#define NMF_HAS_INDICES_DATA   2147483648

	struct S_NMF_Header
	{
		uint32_t magic;
		union
		{
			struct
			{
				uint16_t version;
				uint16_t _reserved;
			};
			uint32_t info;
		};
	};

	struct S_NMF_Description_v0001
	{
		uint32_t vertices_count;
		uint32_t indices_count;
		uint32_t contain_flags;
	};

	class CMeshLoader_NMF : public IStaticMeshLoader
	{
	public:
		CMeshLoader_NMF(io::IFileSystem*, IResourceManager*);
		virtual ~CMeshLoader_NMF();

		bool isSupported(const char* file_extension);
		bool isSupported(io::IFile* file);

		renderer::IStaticMesh* loadStaticMesh(const char* path);
		renderer::IStaticMesh* loadStaticMesh(io::IFile* file, const char* hint);

		renderer::IStaticMesh* LoadNMF(io::IFile* file);

	protected:
	private:

		io::IFileSystem*  m_FileSystem;
		IResourceManager* m_ResourceManager;
	};

}
#endif // _NE_INCLUDE_MESH_LOADER_NMF
