#ifndef SEVENT_H
#define SEVENT_H

#include "NTypes.h"
namespace novaengine
{
//----------------------------------
enum E_EVENT_TYPE
{
    EET_POINTER_EVENT   = 1 ,
    EET_KEYBOARD_EVENT      ,
    EET_WINDOW_EVENT        ,
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
