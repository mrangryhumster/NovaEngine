#include "CFileSystem.h"

namespace novaengine
{
namespace io
{
CFileSystem::CFileSystem()
{
}
//--------------------------------------------------------------------------------------------------------
CFileSystem::~CFileSystem()
{
    //dtor
}
//--------------------------------------------------------------------------------------------------------
IFile* CFileSystem::open_file(const char* filename,bool NativeFile)
{

    FILE* stream = fopen(filename,"rb");

    if(!stream)
    {
        LOG_ERROR("Can't open file: %s\n",filename);
        return NULL;
    }
    else
        LOG_ENGINE_DEBUG("Opened file: %s\n",filename);

    u32  FileLenght   = 0;
    u32  FilePosition = 0;

    fseek (stream, 0, SEEK_END);
    FileLenght = ftell(stream);
    fseek (stream, 0, SEEK_SET);

    //Load file in ram and use(Virtual) or use directly from hdd(Native)
    if(NativeFile)
    {
        return new CNativeFile(stream,FileLenght,FilePosition,filename);
    }
    else
    {
        u8* FileData = new u8[FileLenght];

        fread(FileData,FileLenght,1,stream);

        fclose(stream);

        return  new CVirtualFile(FileLenght,FilePosition,FileData,filename);
    }
}
//--------------------------------------------------------------------------------------------------------
IFile* CFileSystem::open_stream(const char* filename, u8* filedata, u32 datasize)
{
    u32  FileLenght   = datasize;
    u32  FilePosition = 0       ;

    return new CVirtualFile(FileLenght,FilePosition,filedata,filename);
}
//--------------------------------------------------------------------------------------------------------
IFile* CFileSystem::create(const char* filename,bool NativeFile)
{
    FILE* stream = fopen(filename,"wb");
    if(!stream)
    {
        LOG_ERROR("Can't open file: %s\n",filename);
        return NULL;
    }

    //Load file in ram and use(Virtual) or use directly from hdd(Native)
    if(NativeFile)
    {
        return new CNativeFile(stream,0,0,filename);
    }
    else
    {
        fclose(stream);
        return  new CVirtualFile(0,0,0,filename);
    }
}
//--------------------------------------------------------------------------------------------------------
void   CFileSystem::save(const char* filename,IFile* File)
{
    if(File->getFilePointerType() == EFPT_VIRTUAL)
    {
        FILE* stream = fopen(filename,"wb");
        if(!stream)
        {
            LOG_ERROR("Can't open file: %s\n",filename);
            return ;
        }
        fwrite(File->getNativePointer(),File->getSize(),1,stream);
        fclose(stream);
    }
    else
    {
       return;
    }
}
//--------------------------------------------------------------------------------------------------------
}
}
