#ifndef IFILESYSTEM_H
#define IFILESYSTEM_H

#include "Object.h"
#include "IFile.h"

namespace novaengine
{
namespace io
{

class IFileSystem : public Object
    {
    public:
        virtual IFile* open_file  (const char* filename, bool NativeFile = false) = 0;
        virtual IFile* open_stream(const char* filename, u8* filedata, u32 DataSize) = 0;

        virtual IFile* create(const char* filename,bool NativeFile = false) = 0;
        virtual void   save(const char* filename,IFile* File)               = 0;

        //Using Native load method by default
        //deprecated
        virtual void useNativeFileLoadMethod(bool mode) = 0;

    protected:
    private:
};

}
}
#endif // IFILESYSTEM_H
