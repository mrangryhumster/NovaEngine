#ifndef CEVENTMANAGER_H
#define CEVENTMANAGER_H

#include "IEventManager.h"
#include "EKeycodes.h"

#include "NLogger.h"

#include <algorithm>
#include <vector>

namespace novaengine
{

class CEventManager : public IEventManager
{
public:
    CEventManager();
    virtual ~CEventManager();

    bool pushEvent(SEvent event);

    void registerEventListener(IEventListener* proc);
    void unregisterEventListener(IEventListener* proc);

    bool               getKeyState(u32 Keycode);
    bool               getPointerKeyState(u32 Keycode);
    core::vector2<s32> getLastPointerPos();

protected:
private:

    bool               KeyStates[KEY_COUNT];
    bool               PointerKeyStates[KEY_MOUSE_COUNT];
    core::vector2<s32> LastPointerPos;
    std::vector<IEventListener*> EventHandlers;
};

}
#endif // CEVENTMANAGER_H
