#ifndef CFILESYSTEM_H
#define CFILESYSTEM_H

#include "IFileSystem.h"

#include "CVirtualFile.h"
#include "CNativeFile.h"

#include "log/NLogger.h"

#include <stdio.h>

namespace novaengine
{
namespace io
{

class CFileSystem : public IFileSystem
{
    public:
        CFileSystem();
        virtual ~CFileSystem();

        IFile* open_file  (const char* filename, bool NativeFile );
        IFile* open_stream(const char* filename, u8* filedata, u32 DataSize);

        IFile* create(const char* filename,bool NativeFile);
        void   save  (const char* filename,IFile* File);

        void useNativeFileLoadMethod(bool mode);

    protected:
    private:
        bool UseNativeFiles;
};

}
}

#endif // CFILESYSTEM_H
