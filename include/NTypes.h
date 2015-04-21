#ifndef NETYPES_H
#define NETYPES_H

//Need for SEngineConf.ExternalWindowID
#if     defined(NE_WINDOW_WIN32)
#include <windows.h>
typedef HWND         windowid_t_;
#elif   defined(NE_WINDOW_ANDROID)
#include <android_native_app_glue.h>
typedef android_app* windowid_t_;
#else
typedef int          windowid_t_;
#endif // defined

#include <cstddef>

enum NE_TYPES
{
    NTYPE_s8 = 0,
    NTYPE_u8,
    NTYPE_s16,
    NTYPE_u16,
    NTYPE_s32,
    NTYPE_u32,
    NTYPE_s64,
    NTYPE_u64,

    NTYPE_f32,
    NTYPE_f64,
};

typedef char			s8;
typedef unsigned char	u8;

typedef short			s16;
typedef unsigned short	u16;

typedef int				s32;
typedef unsigned int    u32;

typedef long			s64;
typedef unsigned long   u64;
//float
typedef float           f32;
typedef double          f64;
typedef long double     f96;

#endif // NETYPES_H
