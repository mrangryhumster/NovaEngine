
#pragma once

#include <stddef.h>

//--------------------------
//fixed size types
typedef std::int8_t     s8;
typedef std::uint8_t	u8;

typedef std::int16_t    s16;
typedef std::uint16_t   u16;

typedef std::int32_t    s32;
typedef std::uint32_t   u32;

typedef std::int64_t    s64;
typedef std::uint64_t   u64;
//--------------------------
//floating point types
typedef float           f32;
typedef double          f64;
//--------------------------

typedef

//additional shit
enum NE_TYPE
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

inline size_t ne_sizeof(NE_TYPE type)
{
	switch (type)
	{
	case NTYPE_s8:
	case NTYPE_u8:
		return 1;
	case NTYPE_s16:
	case NTYPE_u16:
		return 2;
	case NTYPE_s32:
	case NTYPE_u32:
		return 4;
	case NTYPE_s64:
	case NTYPE_u64:
		return 8;

	case NTYPE_f32:
		return 4;
		break;
	case NTYPE_f64:
		return 8;
		break;
	}
	return 1;
}

