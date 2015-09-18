#ifndef IEVENTMANAGER_H
#define IEVENTMANAGER_H

#include "Object.h"
#include "NTypes.h"
#include "NExTypes.h"

#include "SEvent.h"
#include "IEventListener.h"

namespace novaengine
{
class IEventManager : public Object
{
    public:

        virtual bool pushEvent(SEvent incoming_event) = 0;

        virtual void registerEventListener(IEventListener* proc)   = 0;
        virtual void unregisterEventListener(IEventListener* proc) = 0;

        virtual bool               getKeyState(u32 Keycode)          = 0;
        virtual bool               getPointerKeyState(u32 Keycode)   = 0;
        virtual core::vector2<s32> getLastPointerPos()               = 0;

    protected:
    private:

};
}
#endif // IEVENTMANAGER_H
