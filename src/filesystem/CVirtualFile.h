#ifndef CVIRTUALFILE_H
#define CVIRTUALFILE_H

#include "IFile.h"

#include <malloc.h>
#include <cstring>
#include <memory.h>

#include "Log\NLogger.h"

namespace novaengine
{
namespace io
{


class CVirtualFile : public IFile
{
public:

    CVirtualFile(size_t size,size_t pos,u8* data,const char* name = NULL)
    {
        FileSize    = size;
        FilePos     = pos;
        LastFilePos = pos;
        FileData    = data;

        if(name)
        {
            int len = strlen(name)+1;
            FileName = new char[len];
            memcpy(FileName,name,sizeof(char)*len);
        }
    }
    virtual ~CVirtualFile()
    {
        if(FileData)
            delete[] FileData;
        if(FileName)
            delete[] FileName;
    }

    int read(void* ptr,size_t Len)
    {
        if(Len+FilePos > FileSize)
            return 0;

        memcpy(ptr,FileData + FilePos,Len);
        LastFilePos = FilePos;
        FilePos+=Len;

        return Len;
    }
    int read(void* ptr,size_t pos,size_t Len)
    {
        if(pos >= FileSize || (Len+pos) > FileSize)
            return 0;

        memcpy(ptr,FileData + pos,Len);

        return Len;
    }
    int write(const void* ptr,size_t Len)
    {

        if(FilePos > FileSize)
            return 0;
        if(FilePos+Len > FileSize)
        {
            size_t newFileLenght = FilePos+Len;
            u8* new_stream = new u8[newFileLenght];
            memcpy(new_stream,FileData,FileSize);
            delete[] FileData;
            FileData = new_stream;
            FileSize = newFileLenght;
        }
        memcpy(FileData + FilePos,ptr,Len);
        LastFilePos = FilePos;
        FilePos+=Len;
        return Len;
    }

    int write(const void* ptr,size_t pos,size_t Len)
    {

        if(pos > FileSize)
            return 0;
        if(pos+Len > FileSize)
        {
            size_t newFileLenght = pos+Len;
            u8* new_stream = new u8[newFileLenght];
            memcpy(new_stream,FileData,FileSize);
            delete[] FileData;
            FileData = new_stream;
            FileSize = newFileLenght;
        }
        memcpy(FileData + pos,ptr,Len);
        return Len;
    }

    int read_string(char* out_string,size_t size)
    {
        LastFilePos = FilePos;
        size_t file_string_len = 0;
        while(FilePos < FileSize)
        {
            out_string[file_string_len] = FileData[FilePos];
            if(out_string[file_string_len] == '\n' || file_string_len >= size)
            {
               out_string[file_string_len] = '\0';
               FilePos++;
               file_string_len++;
               break;
            }
            FilePos++;
            file_string_len++;
        }
        return file_string_len;
    }

    int read_string(size_t pos,char* out_string,size_t size)
    {
        if(pos >= FileSize)
            return 0;

        size_t TmpFilePos = pos;
        size_t file_string_len = 0;
        while(TmpFilePos < FileSize)
        {
            out_string[file_string_len] = FileData[TmpFilePos];
            if(out_string[file_string_len] == '\n' || file_string_len >= size)
            {
               out_string[file_string_len] = '\0';
               TmpFilePos++;
               file_string_len++;
               break;
            }
            TmpFilePos++;
            file_string_len++;
        }
        return file_string_len;
    }

    void seek(size_t pos)
    {
        if(pos > FileSize)
            return;
        LastFilePos = FilePos;
        FilePos = pos;
    }

    int getFilePointerType()
    {
        return EFPT_VIRTUAL;
    }
    size_t getSize()
    {
        return FileSize;
    }
    size_t getPos()
    {
        return FilePos;
    }
    size_t getLastPos()
    {
        return LastFilePos;
    }
    void* getNativePointer()
    {
        return FileData;
    }

    char* getName()
    {
        return FileName;
    }

protected:
private:

    size_t FileSize;
    size_t FilePos;
    size_t LastFilePos;
    u8* FileData;
    char* FileName;

};

}
}


#endif // CFILE_H
