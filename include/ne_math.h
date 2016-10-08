#ifndef NEMATH_H
#define NEMATH_H

#include <cmath>
#include "ne_types.h"


namespace novaengine
{
namespace math
{


#ifdef PI
#undef PI
#endif // PI
#ifndef PI
//float PI
const f32 PI = 3.14159265359f;
#endif // PI

#ifdef PI64
#undef PI64
#endif // PI64

#ifndef PI64
//double PI
const f64 PI64 = 3.1415926535897932384626433832795028841971693993751; //MOAR PI!!! MOAR !!!
#endif // PI64

#ifdef FLT_EPSILON
#undef FLT_EPSILON
#endif // FLT_EPSILON

#ifndef FLT_EPSILON
const f64 FLT_EPSILON = 1E-5;
#endif // FLT_EPSILON

#ifdef DBL_EPSILON
#undef DBL_EPSILON
#endif // DBL_EPSILON

#ifndef DBL_EPSILON
const f64 DBL_EPSILON = 1E-9;
#endif // FLT_EPSILON

//! DEG <-> RAD

//32bit conversion
const f32 DEGTORAD = PI / 180.0f;
const f32 RADTODEG = 180.0f / PI;

//64bit conversion
const f64 DEGTORAD64 = PI64 / 180.0;
const f64 RADTODEG64 = 180.0 / PI64;

//------------------------------
inline f32 RadToDeg(const f32 radians)
{
    return RADTODEG * radians;
}

inline f64 RadToDeg(const f64 radians)
{
    return RADTODEG64 * radians;
}

inline f32 DegToRad(const f32 degrees)
{
    return DEGTORAD * degrees;
}

inline f64 DegToRad(const f64 degrees)
{
    return DEGTORAD64 * degrees;
}
//-----------------------------
inline bool is_equalf(const f32 val_1,const f32 val_2,const f64 epsilon = FLT_EPSILON)
{
    if(std::abs((double)val_1 - val_2) < epsilon)
        return true;
    else
        return false;
}

inline bool is_equald(const f64 val_1,const f64 val_2,const f64 epsilon = DBL_EPSILON)
{

    if(std::abs((double)val_1 - val_2) < epsilon)
        return true;
    else
        return false;
}

template<typename type_>
inline bool is_equal(const type_ val_1,const type_ val_2,const type_ epsilon)
{
    if(std::abs((double)val_1 - val_2) < epsilon)
        return true;
    else
        return false;
}

inline bool is_zerof(const f32 val)
{
    return is_equalf(val,0.0f);
}

inline bool is_zerod(const f64 val)
{
    return is_equald(val,0.0);
}

template<typename type_>
inline bool is_zero(const type_ val)
{
    return is_equal<type_>(val,0.0f,FLT_EPSILON);
}

template<typename type_>
inline type_ max_t(type_ first,type_ second)
{
    return (first > second) ? first : second;
}

template<typename type_>
inline type_ max_t(type_ first,type_ second,type_ third)
{
    return (max_t(first,second) > third) ? max_t(first,second) : third;
}

template<typename type_>
inline type_ min_t(type_ first,type_ second)
{
    return (first < second) ? first : second;
}

template<typename type_>
inline type_ min_t(type_ first,type_ second,type_ third)
{
    return (min_t(first,second) < third) ? min_t(first,second) : third;
}

}
}

#endif // NEMATH_H
