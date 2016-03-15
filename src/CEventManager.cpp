#include "CEventManager.h"
namespace novaengine
{

CEventManager::CEventManager()
{
    for(u32 i = 0; i < KEY_COUNT; i++)
        KeyStates[i] = false;

    LastPointerPos.set(0,0);

	m_EngineEventProc_LastID = 0;
	m_UserEventProc_LastID = 0;
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
	
	//First send all events to engine subsystems
	for (auto it = m_EngineEventProc_List.begin(); it != m_EngineEventProc_List.end(); ++it)
		if (((*it).m_ProcUnitMask & event.event_type))
		{
			if ((*it).m_ProcUnitType == EEPUT_CLASS)
				(*it).m_ProcUnit_Class->OnEvent(event);
			else
				(*it).m_ProcUnit_Func(event);
		}
	//second to user listeners
	for (auto it = m_UserEventProc_List.begin(); it != m_UserEventProc_List.end(); ++it)
		if (((*it).m_ProcUnitMask & event.event_type))
		{
			if ((*it).m_ProcUnitType == EEPUT_CLASS)
				(*it).m_ProcUnit_Class->OnEvent(event);
			else
				(*it).m_ProcUnit_Func(event);
		}

	return 0;
}

u32 CEventManager::registerEventListener(IEventListener* p_Class, u32 p_EventMask, u32 p_ProcPool)
{
	SEventProcessingUnit EPU;
	EPU.m_ProcUnitType = EEPUT_CLASS;
	EPU.m_ProcUnitMask = p_EventMask;
	EPU.m_ProcUnit_Class = p_Class;
	
	if (p_ProcPool == 0)
	{
		EPU.m_ProcUnitID = m_UserEventProc_LastID++;
		m_UserEventProc_List.push_back(EPU);
	}
	else
	{
		EPU.m_ProcUnitID = m_EngineEventProc_LastID++;
		m_EngineEventProc_List.push_back(EPU);
	}
	return EPU.m_ProcUnitID;
}

u32 CEventManager::registerEventListener(std::function<void(SEvent&)> p_Function, u32 p_EventMask, u32 p_ProcPool)
{
	SEventProcessingUnit EPU;
	EPU.m_ProcUnitType = EEPUT_FUNCTION;
	EPU.m_ProcUnitMask = p_EventMask;
	EPU.m_ProcUnit_Func = p_Function;

	if (p_ProcPool == 0)
	{
		EPU.m_ProcUnitID = m_UserEventProc_LastID++;
		m_UserEventProc_List.push_back(EPU);
	}
	else
	{
		EPU.m_ProcUnitID = m_EngineEventProc_LastID++;
		m_EngineEventProc_List.push_back(EPU);
	}
	return EPU.m_ProcUnitID;
}

void CEventManager::unregisterEventListener(u32 p_ListenerID, u32 p_ProcPool)
{
	if (p_ProcPool == 0)
	{
		auto match = std::remove_if(m_UserEventProc_List.begin(), m_UserEventProc_List.end(),
			[p_ListenerID](const SEventProcessingUnit& EPU)
		{
			return (EPU.m_ProcUnitID == p_ListenerID);
		});

			if (match == m_UserEventProc_List.end())
				LOG_ERROR("Cannot unregister event listener : listener not exist\n");
			else
				m_UserEventProc_List.erase(match);
	}
	else
	{
		auto match = std::remove_if(m_EngineEventProc_List.begin(), m_EngineEventProc_List.end(),
			[p_ListenerID](const SEventProcessingUnit& EPU)
		{
			return (EPU.m_ProcUnitID == p_ListenerID);
		});

		if (match == m_EngineEventProc_List.end())
			LOG_ERROR("Cannot unregister event listener : listener not exist\n");
		else
			m_EngineEventProc_List.erase(match);
	}
}

void CEventManager::unregisterAllEventListeners(u32 p_ProcPool)
{
	if (p_ProcPool == 0)
	{
		m_UserEventProc_List.clear();
	}
	else
	{
		m_EngineEventProc_List.clear();
	}
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

std::vector<SEventProcessingUnit>& CEventManager::getPoolRef(u32 p_ProcPool)
{
	// TODO: вставьте здесь оператор return
	return m_UserEventProc_List;
}

}
