#ifndef NETYPES_H
#define NETYPES_H

#include <cstddef>

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
