#ifndef CIMAGEPNGIMPORTER_H
#define CIMAGEPNGIMPORTER_H

#include "CompileConfig.h"

#ifdef NE_INCLUDE_LIBPNG
#include "IImageLoader.h"
#include "IFileSystem.h"
#include "IFile.h"
#include "log/NLogger.h"


#include <cstring>
//#include "png.h"

namespace novaengine
{

class CImagePNGLoader : public IImageLoader
{
public:
    CImagePNGLoader(io::IFileSystem* fs);
    virtual ~CImagePNGLoader();

    bool isSupported(const char* file_extension);
    bool isSupported(io::IFile* file);

    renderer::IImage* LoadImage(const char* path);
    renderer::IImage* LoadImage(io::IFile* file);

    renderer::IImage* LoadPNG(io::IFile* file);

protected:
private:
    io::IFileSystem* FileSys;
};

}
#endif // INCLUDE_LIBPNG
#endif // CImagePNGLoader_H
