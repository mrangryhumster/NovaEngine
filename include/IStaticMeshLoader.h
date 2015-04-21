#ifndef ISTATICMESHIMPORTER_H
#define ISTATICMESHIMPORTER_H

#include "IFile.h"
#include "IStaticMesh.h"

namespace novaengine
{

class IStaticMeshLoader : public Object
{
public:

    virtual bool isSupported(const char* file_extension) = 0;
    virtual bool isSupported(io::IFile* file) = 0;

    virtual renderer::IStaticMesh* LoadStaticMesh(const char* path) = 0;
    virtual renderer::IStaticMesh* LoadStaticMesh(io::IFile* file,const char* hint) = 0;

protected:
private:
};

}
#endif // IMESHIMPORTER_H
