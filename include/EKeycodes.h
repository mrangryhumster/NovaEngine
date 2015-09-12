#ifndef EKEYCODES_H
#define EKEYCODES_H


namespace novaengine
{


enum E_KEY_STATE
{
    EKS_UP      = 0x0,
    EKS_DOWN    = 0x1,
};


enum MOUSE_KEY_CODE
{
    KEY_MOUSE_UNKNOW    = 0x0,
    KEY_MOUSE_LEFT      = 0x1,
    KEY_MOUSE_RIGHT     = 0x2,
    KEY_MOUSE_MIDDLE    = 0x3,
    KEY_MOUSE_WHELL     = 0x4,
    KEY_MOUSE_COUNT     = 0x5,
};

enum E_KEY_CODE
{
    KEY_BACKSPACE        = 0x08,
    KEY_TAB              = 0x09,
    KEY_RETURN           = 0x0D,
    KEY_SHIFT            = 0x0f,
    KEY_CTRL             = 0x10,
    KEY_ALT              = 0x11,
    KEY_CAPSLOCK         = 0x14,
    KEY_PAUSE            = 0x19,

    KEY_ESCAPE           = 0x1B,
    KEY_SPACE            = 0x20,
    KEY_PAGEUP           = 0x21,
    KEY_PAGEDOWN         = 0x22,
    KEY_END              = 0x23,
    KEY_HOME             = 0x24,
    KEY_LEFT             = 0x25,
    KEY_UP               = 0x26,
    KEY_RIGHT            = 0x27,
    KEY_DOWN             = 0x28,
    KEY_SELECT           = 0x29,
    KEY_PRINT            = 0x2A,
    //KEY_EXECUTE          = 0x2B,
    KEY_PRINTSCREEN      = 0x2C,
    KEY_INSERT           = 0x2D,
    KEY_DELETE           = 0x2E,
    KEY_HELP             = 0x30,

    KEY_KEY_0            = 0x30,
    KEY_KEY_1            = 0x31,
    KEY_KEY_2            = 0x32,
    KEY_KEY_3            = 0x33,
    KEY_KEY_4            = 0x34,
    KEY_KEY_5            = 0x35,
    KEY_KEY_6            = 0x36,
    KEY_KEY_7            = 0x37,
    KEY_KEY_8            = 0x38,
    KEY_KEY_9            = 0x39,
    KEY_KEY_A            = 0x41,
    KEY_KEY_B            = 0x42,
    KEY_KEY_C            = 0x43,
    KEY_KEY_D            = 0x44,
    KEY_KEY_E            = 0x45,
    KEY_KEY_F            = 0x46,
    KEY_KEY_G            = 0x47,
    KEY_KEY_H            = 0x48,
    KEY_KEY_I            = 0x49,
    KEY_KEY_J            = 0x4A,
    KEY_KEY_K            = 0x4B,
    KEY_KEY_L            = 0x4C,
    KEY_KEY_M            = 0x4D,
    KEY_KEY_N            = 0x4E,
    KEY_KEY_O            = 0x4F,
    KEY_KEY_P            = 0x50,
    KEY_KEY_Q            = 0x51,
    KEY_KEY_R            = 0x52,
    KEY_KEY_S            = 0x53,
    KEY_KEY_T            = 0x54,
    KEY_KEY_U            = 0x55,
    KEY_KEY_V            = 0x56,
    KEY_KEY_W            = 0x57,
    KEY_KEY_X            = 0x58,
    KEY_KEY_Y            = 0x59,
    KEY_KEY_Z            = 0x5A,

    KEY_WINDOWS          = 0x5B,

    KEY_APPS             = 0x5D,

    KEY_NUM_0          = 0x60,  // Num key 0 key
    KEY_NUM_1          = 0x61,  // Num key 1 key
    KEY_NUM_2          = 0x62,  // Num key 2 key
    KEY_NUM_3          = 0x63,  // Num key 3 key
    KEY_NUM_4          = 0x64,  // Num key 4 key
    KEY_NUM_5          = 0x65,  // Num key 5 key
    KEY_NUM_6          = 0x66,  // Num key 6 key
    KEY_NUM_7          = 0x67,  // Num key 7 key
    KEY_NUM_8          = 0x68,  // Num key 8 key
    KEY_NUM_9          = 0x69,  // Num key 9 key

    KEY_MULTIPLY         = 0x6A,
    KEY_PLUS             = 0x6B,
    KEY_MINUS            = 0x6D,
    KEY_DECIMAL          = 0x6E,
    KEY_DIVIDE           = 0x6F,

    KEY_F1               = 0x70,  // F1 key
    KEY_F2               = 0x71,  // F2 key
    KEY_F3               = 0x72,  // F3 key
    KEY_F4               = 0x73,  // F4 key
    KEY_F5               = 0x74,  // F5 key
    KEY_F6               = 0x75,  // F6 key
    KEY_F7               = 0x76,  // F7 key
    KEY_F8               = 0x77,  // F8 key
    KEY_F9               = 0x78,  // F9 key
    KEY_F10              = 0x79,  // F10 key
    KEY_F11              = 0x7A,  // F11 key
    KEY_F12              = 0x7B,  // F12 key
    KEY_F13              = 0x7C,  // F13 key
    KEY_F14              = 0x7D,  // F14 key
    KEY_F15              = 0x7E,  // F15 key
    KEY_F16              = 0x7F,  // F16 key

    KEY_NUMLOCK          = 0x90,
    KEY_SCROLLOCK        = 0x91,

    KEY_LSHIFT           = 0xA0,
    KEY_RSHIFT           = 0xA1,
    KEY_LCTRL            = 0xA2,
    KEY_RCTRL            = 0xA3,
    KEY_LALT             = 0xA4,
    KEY_RALT             = 0xA5,



    KEY_UNKNOW           = 0x00,
    KEY_COUNT            = 0xFF,
    KEY_NO_KEY           = 0xFF,
};


}

#endif

