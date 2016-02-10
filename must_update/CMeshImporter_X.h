#ifndef CMESHIMPORTER_X_H
#define CMESHIMPORTER_X_H

#include "IFileSystem.h"
#include "IMeshImporter.h"
#include "log/NLog.h"

namespace novaengine
{

class CMeshImporter_X : public IMeshImporter
{
    public:

    CMeshImporter_X(io::IFileSystem* fs);
    virtual ~CMeshImporter_X();

    bool isSupported(const char* file_extension);
    bool isSupported(io::IFile* file);

    renderer::IMesh* LoadMesh(const char* path);
    renderer::IMesh* LoadMesh(io::IFile* file);

    renderer::IMesh* LoadX(io::IFile* file);

    protected:
    private:

    io::IFileSystem* FileSystem;

};

}
#endif // CMESHIMPORTER_X_H
