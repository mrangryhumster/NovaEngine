#ifndef CFILESYSTEM_H
#define CFILESYSTEM_H

#include "IFileSystem.h"

#include "CVirtualFile.h"
#include "CNativeFile.h"

#include "NLogger.h"

#include <stdio.h>
#include <cstdlib>

#define MAX_PATH   260
#define MAX_DRIVE  3
#define MAX_DIR    256


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

    protected:
    private:

		//os specific
		void getAbsolutePath(std::string& RelativePath,std::string& AbsolutePath,std::string& AbsoluteDirectoryPath);
};

}
}

#endif // CFILESYSTEM_H
