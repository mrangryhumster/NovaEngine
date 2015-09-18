#ifndef OBJECT_H
#define OBJECT_H

#include "NTypes.h"
#include <string.h>
#include <stdexcept>

#define NOVAENGINE_OBJECT_START_REFERENCECOUNT_VAL 1

namespace novaengine
{
class Object
{
public:

    /// \brief Default constructor
    /// Set ReferenceCounter in 1,ObjectName in "NotDefined"
    Object() :
        ObjectFreeze(false),
        ReferenceCount(NOVAENGINE_OBJECT_START_REFERENCECOUNT_VAL)
    {
        ObjectName = new char[8];
        memcpy(ObjectName,"unknown",8);
    }

    virtual ~Object()
    {
        delete[] ObjectName;
    }
    /// \brief Freeze ReferenceCounter of object
    /// if Freeze(true) then all
    /// release() and capture() calls will not effect on ReferenceCount
    /// But destroy() still work
    inline void Freeze(bool state) const
    {
        ObjectFreeze = state;
    }
    /// \brief  Return true if ObjectFreeze
    /// \return true if ObjectFreeze
    inline bool isFreeze() const
    {
        return ObjectFreeze;
    }
    /// \brief Increment referencecount
    inline void capture() const
    {
        if(not ObjectFreeze)
        {
            ReferenceCount++;
        }
    }
    /// \brief decrement referencecount and delete if referencecount < 1
    /// Decrement referencecount and delete object if ReferenceCount < 1
    inline void release() const
    {
        if(not ObjectFreeze)
        {
            ReferenceCount--;
            if(ReferenceCount < 1)
            {
                destroy();
            }
        }
    }
    /// \brief decrement referencecount (no delete if referencecount < 1)
    inline void release_next() const
    {
        if(not ObjectFreeze)
        {
            ReferenceCount--;
        }
    }

    /// \brief Destroy object
    /// Destroy object instantly(ignoring ReferenceCount)
    inline void destroy() const
    {
        delete this;
    }
    /// \brief Return ReferenceCount
    /// \return ReferenceCount
    inline int getReferenceCount() const
    {
        return ReferenceCount;
    }

    /// \brief Set name for this object
    /// \param name Set name for this object
    inline void setObjectName(const char* Name)
    {

        delete[] ObjectName;
        ObjectName = new char[strlen(Name)+1];
        strcpy(ObjectName,Name);
    }

    /// \brief Get object name
    /// \return return object name
    inline const char* getObjectName()
    {
        return ObjectName;
    }

    /// \brief Get object state
    /// \return 1 if this != NULL else 0
    inline int is_null() const
    {
        if(not this)
            return 1;
        else
            return 0;
    }

protected:
    Object(Object&){};
    mutable bool ObjectFreeze;
    mutable int  ReferenceCount;
    char*  ObjectName;
};

}
#endif // OBJECT_H
