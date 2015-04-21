#ifndef RESOURCE_H
#define RESOURCE_H

#include "Object.h"
#include "string.h"

namespace novaengine
{

class Resource : public Object
{
    public:
        Resource():
            ResourceLoaded(false),
            ResourceLocked(false)
        {
            ResourceName = new char[8];
            memcpy(ResourceName,"unknown",8);
        }
        virtual ~Resource()
        {
            delete[] ResourceName;
        }

        bool isLoaded()
        {
            return ResourceLoaded;
        }
        void setLoadedFlag(bool flag)
        {
            ResourceLoaded = flag;
        }

        bool isLocked()
        {
            return ResourceLocked;
        }

        void setLockedFlag(bool flag)
        {
            ResourceLocked = flag;
        }

        const char* getResourceName()
        {
            return ResourceName;
        }
        void setResourceName(const char* Name)
        {
            delete[] ResourceName;
            ResourceName = new char[strlen(Name)+1];
            strcpy(ResourceName,Name);
        }

    protected:
    private:
        volatile bool ResourceLoaded;
        volatile bool ResourceLocked;
        char*         ResourceName;
};
}
#endif // RESOURCE_H

