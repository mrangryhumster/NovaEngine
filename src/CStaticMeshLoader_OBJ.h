#ifndef CSTATICMESHLOADER_OBJ_H
#define CSTATICMESHLOADER_OBJ_H

#include "CompileConfig.h"

#ifdef NE_INCLUDE_STATICMESH_LOADER_OBJ
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


    io::IFileSystem* FileSystem;
    IResourceManager* ResourceManager;

};

}
#endif // NE_INCLUDE_STATICMESH_LOADER_OBJ
#endif // CSTATICMESHLOADER_OBJ_H
