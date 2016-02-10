#ifndef MEMORYWATCHER_H
#define MEMORYWATCHER_H

#include <stdlib.h>

#include <memory>
#include <vector>

namespace memory
{
//------------------------------------
struct SMemoryBlock
{
    void* pointer;
    unsigned int size;
    bool array;
};

class CMemoryWatcher
{
public:
    ~CMemoryWatcher()
    {
        printf("\n\nUnreleased memory : %u\n",BytesUsed);
    }

    void register_block(void* ptr,unsigned int size,bool array)
    {
        if(DontWatch)
            return;

        DontWatch=true;
        SMemoryBlock mb;
        mb.pointer = ptr;
        mb.size = size;
        mb.array = array;
        BytesUsed+=size;
        MemoryAllocated.push_back(mb);
        DontWatch=false;
    }

    void unregister_block(void* ptr,bool array)
    {
        if(DontWatch)
            return;

        DontWatch=true;
        unsigned int size = MemoryAllocated.size();
        for(unsigned int i = 0; i < size; i++)
        {
            if(MemoryAllocated[i].pointer == ptr)
            {
                if(MemoryAllocated[i].array == array)
                {
                    BytesUsed-=MemoryAllocated[i].size;
                    MemoryAllocated.erase(MemoryAllocated.begin() + i);
                }
                else
                {
                    printf("warning using wrong combination of new/delete(new_array:%d del_array:%d)\n",MemoryAllocated[i].array,array);
                }
                break;
            }
        }
        DontWatch=false;
    }
    unsigned int getAllocatedMemorySize()
    {
        return BytesUsed;
    }
private:
    std::vector<SMemoryBlock> MemoryAllocated;
    unsigned int BytesUsed;
    bool DontWatch;
};
static CMemoryWatcher MemoryWatcher;
//------------------------------------
}

void* operator new(size_t size)
{
    void* pointer = malloc(size);
    memory::MemoryWatcher.register_block(pointer,size,false);
    return pointer;
}

void operator delete(void *p)
{
    memory::MemoryWatcher.unregister_block(p,false);
    free(p);
}
//------------------------------------
void * operator new[](size_t size)
{
    void* pointer = malloc(size);
    memory::MemoryWatcher.register_block(pointer,size,true);
    return pointer;
}

void operator delete[](void *p)
{
    memory::MemoryWatcher.unregister_block(p,true);
    free(p);
}
//------------------------------------

#endif // MEMORYWATCHER_H
