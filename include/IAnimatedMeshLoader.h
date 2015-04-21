#ifndef IANIMATEDMESHIMPORTER_H
#define IANIMATEDMESHIMPORTER_H

#include "Object.h"
#include "IFile.h"
#include "IAnimatedMesh.h"

namespace novaengine
{

class IAnimatedMeshLoader : public Object
{
public:

    virtual bool isSupported(const char* file_extension) = 0;
    virtual bool isSupported(io::IFile* file) = 0;

    virtual renderer::IAnimatedMesh* LoadAnimatedMesh(const char* path) = 0;
    virtual renderer::IAnimatedMesh* LoadAnimatedMesh(io::IFile* file,const char* hint) = 0;

protected:
private:
};

}
#endif // IANIMATEDMESHIMPORTER_H
