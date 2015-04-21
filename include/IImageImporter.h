#ifndef IIMAGEIMPORTER_H
#define IIMAGEIMPORTER_H

#include "Object.h"
#include "IFile.h"
#include "IImage.h"

namespace novaengine
{

class IImageLoader : public Object
{
public:

    virtual bool isSupported(const char* file_extension) = 0;
    virtual bool isSupported(io::IFile* file) = 0;

    virtual renderer::IImage* LoadImage(const char* path) = 0;
    virtual renderer::IImage* LoadImage(io::IFile* file) = 0;

protected:
private:
};

}

#endif // IIMAGEIMPORTER_H
