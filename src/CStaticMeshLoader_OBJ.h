#pragma once

#include "CompileConfig.h"

#ifdef _NE_INCLUDE_MESH_LOADER_OBJ

#include "IFileSystem.h"
#include "IStaticMeshLoader.h"
#include "IResourceManager.h"

#include "NLogger.h"

#include <vector>
#include <map>

namespace novaengine
{

class CStaticMeshLoader_OBJ : public IStaticMeshLoader
{
public:

    CStaticMeshLoader_OBJ(io::IFileSystem* fs,IResourceManager* rm);
    virtual ~CStaticMeshLoader_OBJ();

    bool isSupported(const char* file_extension);
    bool isSupported(io::IFile* file);

    renderer::IStaticMesh* loadStaticMesh(const char* path);
    renderer::IStaticMesh* loadStaticMesh(io::IFile* file,const char* hint);

    renderer::IStaticMesh* LoadOBJ(io::IFile* file);

protected:
private:


    void read_mtl_file(std::map<std::string,renderer::IMeshBuffer*>&,const char*);


    io::IFileSystem*  m_FileSystem;
    IResourceManager* m_ResourceManager;

};

}
#endif // _NE_INCLUDE_MESH_LOADER_OBJ

