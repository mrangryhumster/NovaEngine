#ifndef CMESHOBJIMPORTER_H
#define CMESHOBJIMPORTER_H

#include "IFileSystem.h"
#include "IStaticMeshImporter.h"
#include "log/NLog.h"

namespace novaengine
{

class CStaticMeshImporter_OBJ : public IStaticMeshImporter
{
public:

    CStaticMeshImporter_OBJ(io::IFileSystem* fs);
    virtual ~CStaticMeshImporter_OBJ();

    bool isSupported(const char* file_extension);
    bool isSupported(io::IFile* file);

    renderer::IStaticMesh* LoadMesh(const char* path);
    renderer::IStaticMesh* LoadMesh(io::IFile* file,const char* hint);

    renderer::IStaticMesh* LoadOBJ(io::IFile* file);

protected:
private:

io::IFileSystem* FileSystem;

};

}

#endif // CMESHOBJIMPORTER_H
