#ifndef IFILE_H
#define IFILE_H

#include "Object.h"
#include "NTypes.h"

namespace novaengine
{
namespace io
{
enum E_FILE_POINTER_TYPE
{
    EFPT_VIRTUAL,
    EFPT_NATIVE
};
enum E_FILE_SOURCE_TYPE
{
    EFST_NATIVE,
    EFST_STREAM
};
class IFile : public Object
{
public:
    virtual int read(void* ptr,size_t Len) = 0;
    virtual int read(void* ptr,size_t pos,size_t Len) = 0;
    virtual int write(const void* ptr,size_t Len) = 0;
    virtual int write(const void* ptr,size_t pos,size_t Len) = 0;
    virtual void seek(size_t pos) = 0;

    virtual int read_string(char* out_string,size_t size) = 0;
    virtual int read_string(size_t pos,char* out_string,size_t size) = 0;

    virtual int getSourceType() = 0;
    virtual int getFilePointerType() = 0;

    virtual size_t getSize() = 0;
    /// \return Return in file position
    virtual size_t getPos()     = 0;
    /// \return Return in file position before last read/write/seek calls...
    virtual size_t getLastPos() = 0;

    virtual const char* getName() = 0;
    virtual const char* getDir()  = 0;



    virtual void* getNativePointer() = 0;

protected:
private:

};

}
}


#endif // IFILE_H
