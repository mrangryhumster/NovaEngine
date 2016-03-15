#ifndef SEVENT_H
#define SEVENT_H

#include "NTypes.h"
namespace novaengine
{
//----------------------------------
enum E_EVENT_TYPE
{
	EET_UNKNOWN_EVENT   = 1,
	EET_CUSTOM_EVENT    = 2,
	EET_WINDOW_EVENT    = 4,
	EET_RENDERER_EVENT  = 8,
    EET_POINTER_EVENT   = 16,
    EET_KEYBOARD_EVENT  = 32,
};
//----------------------------------
enum E_POINTER_EVENT_TYPE
{
    EPET_MOVE       = 1 ,
    EPET_BUTTON         ,
    EPET_WHELL          ,
};
//----------------------------------
enum E_WINDOW_EVENT_TYPE
{
    EWET_UNKNOWN    = 0 ,
    EWET_INIT_DONE      ,
};
//---------------------------------
struct SPointerEvent
{
    E_POINTER_EVENT_TYPE event_type;
    s32 x;
    s32 y;
    u16  keycode;
    bool key_state;
};
//----------------------------------
struct SKeyboardEvent
{
    u16  keycode;
    bool key_state;
    u16  unicode;
    u16  native_keycode;

    bool shift;
    bool ctrl;
    bool alt;

};
//----------------------------------
struct SWindowEvent
{
    E_WINDOW_EVENT_TYPE event_type;
};
//----------------------------------
struct SEvent
{
    E_EVENT_TYPE event_type;

    union
    {
    SPointerEvent  pointer;
    SKeyboardEvent keyboard;
    SWindowEvent   window;
    };
};
}
#endif // SEVENT_H
