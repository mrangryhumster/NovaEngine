#ifndef CIMAGEIMPORTER_PNG_H
#define CIMAGEIMPORTER_PNG_H

#include "CompileConfig.h"

#ifdef NE_INCLUDE_LIBPNG
#include "IImageLoader.h"
#include "IFileSystem.h"
#include "IFile.h"
#include "log/NLogger.h"

#include <cstring>

namespace novaengine
{

class CImageLoader_PNG : public IImageLoader
{
public:
    CImageLoader_PNG(io::IFileSystem* fs);
    virtual ~CImageLoader_PNG();

    bool isSupported(const char* file_extension);
    bool isSupported(io::IFile* file);

    renderer::IImage* LoadImage(const char* path);
    renderer::IImage* LoadImage(io::IFile* file);

    renderer::IImage* LoadPNG(io::IFile* file);

protected:
private:
    io::IFileSystem* FileSystem;
};

}
#endif // INCLUDE_LIBPNG
#endif // CIMAGEIMPORTER_PNG_H
