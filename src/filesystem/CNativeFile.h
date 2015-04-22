#ifndef CNATIVEFILE_H
#define CNATIVEFILE_H



#include "IFile.h"

#include <stdio.h>
#include "log\NLogger.h"

namespace novaengine
{
namespace io
{


class CNativeFile : public IFile
{
public:

    CNativeFile(void* file_pointer,size_t file_size,size_t file_pos,const char* name = NULL)
    {
        FileSize    = file_size;
        FilePos     = file_pos;
        LastFilePos = file_pos;
        FilePointer = (FILE*)file_pointer;

        if(name)
        {
            int len = strlen(name);
            FileName = new char[len];
            memcpy(FileName,name,sizeof(char)*len);
        }
    }
    virtual ~CNativeFile()
    {
        if(FilePointer)
            fclose(FilePointer);
        if(FileName)
            delete[] FileName;
    }

    int read(void* ptr,size_t Len)
    {
        if(Len+FilePos > FileSize)
            return 0;

        size_t Readed = fread(ptr,Len,1,FilePointer);
        LastFilePos = FilePos;
        FilePos+=Len;

        return Readed;
    }

    int read(void* ptr,size_t pos,size_t Len)
    {
        if(pos >= FileSize || (Len+pos) > FileSize)
            return 0;

        fseek(FilePointer,pos,SEEK_SET);
        size_t Readed = fread(ptr,Len,1,FilePointer);
        fseek(FilePointer,FilePos,SEEK_SET);

        return Readed;
    }
    int write(const void* ptr,size_t Len)
    {

        size_t Writed = fwrite(ptr,Len,1,FilePointer);

        update_size();
        LastFilePos = FilePos;
        FilePos+=Len;

        return Writed;
    }

    int write(const void* ptr,size_t pos,size_t Len)
    {

        fseek(FilePointer,pos,SEEK_SET);
        size_t Writed = fwrite(ptr,Len,1,FilePointer);
        fseek(FilePointer,FilePos,SEEK_SET);

        update_size();

        return Writed;
    }

    int read_string(char* out_string,size_t size)
    {
        LastFilePos = FilePos;
        size_t file_string_len = 0;
        while(FilePos < FileSize)
        {
            out_string[file_string_len] = fgetc(FilePointer);
            if(out_string[file_string_len] == '\n'  || file_string_len >= size)
            {
               out_string[file_string_len] = '\0';
               FilePos++;
               file_string_len++;
               break;
            }
            FilePos++;
            file_string_len++;
        }
        seek(FilePos);
        return file_string_len;
    }

    int read_string(size_t pos,char* out_string,size_t size)
    {
        if(pos >= FileSize)
            return 0;

        size_t TmpFilePos = pos;
        size_t file_string_len = 0;

        seek(pos);
        while(TmpFilePos < FileSize)
        {
            out_string[file_string_len] = fgetc(FilePointer);
            if(out_string[file_string_len] == '\n'  || file_string_len >= size)
            {
               out_string[file_string_len] = '\0';
               TmpFilePos++;
               file_string_len++;
               break;
            }
            TmpFilePos++;
            file_string_len++;
        }
        seek(FilePos);
        return file_string_len;
    }

    void seek(size_t pos)
    {
        if(pos > FileSize)
            return;
        LastFilePos = FilePos;
        FilePos = pos;
        fseek(FilePointer,pos,SEEK_SET);
    }

    int getFilePointerType()
    {
        return EFPT_NATIVE;
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
        return FilePointer;
    }

    char* getName()
    {
        return FileName;
    }

protected:
private:

    void update_size()
    {
        fseek (FilePointer, 0, SEEK_END);
        FileSize = ftell(FilePointer);
        fseek (FilePointer, FilePos, SEEK_SET);
    }

    size_t FileSize;
    size_t FilePos;
    size_t LastFilePos;
    FILE* FilePointer;
    char* FileName;

};

}
}


#endif // CNATIVEFILE_H
