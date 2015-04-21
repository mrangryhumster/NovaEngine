#ifndef IEVENTMANAGER_H
#define IEVENTMANAGER_H

#include "Object.h"
#include "NTypes.h"
#include "NExTypes.h"

#include "SEvent.h"
#include "IEventListener.h"

namespace novaengine
{
class IEventManager : public IEventListener
{
    public:

        virtual void registerEventHandler(IEventListener* proc)   = 0;
        virtual void unregisterEventHandler(IEventListener* proc) = 0;

        virtual bool               getKeyState(u32 Keycode)          = 0;
        virtual bool               getPointerKeyState(u32 Keycode)   = 0;
        virtual core::vector2<s32> getLastPointerPos()               = 0;

    protected:
    private:

};
}
#endif // IEVENTMANAGER_H
