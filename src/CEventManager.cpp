#include "CEventManager.h"
namespace novaengine
{

CEventManager::CEventManager()
{
    for(u32 i = 0; i < KEY_COUNT; i++)
        KeyStates[i] = false;

    LastPointerPos.set(0,0);
}

CEventManager::~CEventManager()
{
    //dtor
}

bool CEventManager::pushEvent(SEvent event)
{
    if(event.event_type == EET_KEYBOARD_EVENT)
        KeyStates[event.keyboard.keycode] = event.keyboard.key_state;
    if(event.event_type == EET_POINTER_EVENT)
    {
        LastPointerPos.set(event.pointer.x,event.pointer.y);
        if(event.pointer.event_type == EPET_BUTTON)
        {
        PointerKeyStates[event.pointer.keycode] = event.pointer.key_state;
        }
    }
    u32 list_size = EventHandlers.size();
    for(u32 i = 0; i < list_size; i++)
        EventHandlers[i]->OnEvent(event);
    
	return 0;
}

void CEventManager::registerEventListener(IEventListener* proc)
{
    if(proc != NULL)
        EventHandlers.push_back(proc);
}

void CEventManager::unregisterEventListener(IEventListener* proc)
{
	//  new algorithm
	EventHandlers.erase(std::remove(EventHandlers.begin(), EventHandlers.end(), proc), EventHandlers.end());
	/*  old algorithm
	u32 list_size = EventHandlers.size();
    for(u32 i = 0; i < list_size; i++)
        if(EventHandlers[i] == proc)
            EventHandlers.erase(EventHandlers.begin() + i);
	*/
}
bool CEventManager::getPointerKeyState(u32 Keycode)
{
    return PointerKeyStates[Keycode];
}
bool CEventManager::getKeyState(u32 Keycode)
{
    return KeyStates[Keycode];
}

core::vector2<s32> CEventManager::getLastPointerPos()
{
    return LastPointerPos;
}

}
