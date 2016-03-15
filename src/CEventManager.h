#ifndef CEVENTMANAGER_H
#define CEVENTMANAGER_H

#include "IEventManager.h"
#include "EKeycodes.h"

#include "NLogger.h"

#include <functional>
#include <algorithm>
#include <vector>

namespace novaengine
{
enum E_EVENT_PROCESSING_UNIT_FLAG
{
	EEPUF_ENGINE_PROC_UNIT = 0,
	EEPUF_USER_PROC_UNIT = 1,
};

enum E_EVENT_PROCESSING_UNIT_TYPE
{
	EEPUT_CLASS = 0,
	EEPUT_FUNCTION = 1,
};

struct SEventProcessingUnit
{
	u32 m_ProcUnitType;
	u32 m_ProcUnitMask;
	u32 m_ProcUnitID;
	//-----------------
	IEventListener*               m_ProcUnit_Class;
	std::function<void(SEvent&)> m_ProcUnit_Func;
	//-----------------
};

class CEventManager : public IEventManager
{
public:
    CEventManager();
    virtual ~CEventManager();

    bool pushEvent(SEvent event);

	u32 registerEventListener(IEventListener* p_Class, u32 p_EventMask = 0xffffffff, u32 p_ProcPool = 0);
	u32 registerEventListener(std::function<void(SEvent&)> p_Function, u32 p_EventMask = 0xffffffff, u32 p_ProcPool = 0);

	void unregisterEventListener(u32 p_ListenerID, u32 p_ProcPool = 0);
	void unregisterAllEventListeners(u32 p_ProcPool = 0);



    bool               getKeyState(u32 Keycode);
    bool               getPointerKeyState(u32 Keycode);
    core::vector2<s32> getLastPointerPos();

protected:
private:

	std::vector<SEventProcessingUnit>& getPoolRef(u32 p_ProcPool);

    bool               KeyStates[KEY_COUNT];
    bool               PointerKeyStates[KEY_MOUSE_COUNT];
    core::vector2<s32> LastPointerPos;
    std::vector<IEventListener*> EventHandlers;

	u32 m_UserEventProc_LastID;
	std::vector<SEventProcessingUnit> m_UserEventProc_List;

	u32 m_EngineEventProc_LastID;
	std::vector<SEventProcessingUnit> m_EngineEventProc_List;
};

}
#endif // CEVENTMANAGER_H
