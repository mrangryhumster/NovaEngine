#ifndef IEVENTMANAGER_H
#define IEVENTMANAGER_H

#include "Object.h"
#include "ne_types.h"


#include "SEvent.h"
#include "IEventListener.h"

#include <functional>

namespace novaengine
{
class IEventManager : public Object
{
    public:

        virtual bool pushEvent(SEvent p_Event) = 0;

		//m_ProcPool should be 0, another values reserved for internal uses
		virtual u32 registerEventListener(IEventListener* p_Class, u32 p_EventMask = 0xffffffff, u32 p_ProcPool = 0) = 0;
		virtual u32 registerEventListener(std::function<void(SEvent&)> p_Function, u32 p_EventMask = 0xffffffff, u32 p_ProcPool = 0) = 0;

		virtual void unregisterEventListener(u32 p_ListenerID, u32 p_ProcPool = 0) = 0;
		virtual void unregisterAllEventListeners(u32 p_ProcPool = 0) = 0;


        virtual bool               getKeyState(u32 p_Keycode)          = 0;
        virtual bool               getPointerKeyState(u32 p_Keycode)   = 0;
        virtual core::vector2<s32> getLastPointerPos()                 = 0;

    protected:
    private:

};
}
#endif // IEVENTMANAGER_H
