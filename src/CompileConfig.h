#ifndef COMPILECONFIG_H
#define COMPILECONFIG_H

//------------Windows
#if defined(_WIN32)

#include <windows.h>

#if defined(_WIN64)
#endif // __WIN64

//------------Apple
#elif defined(__APPLE__)

//------------Linux
#elif defined(__linux__)

//------------Android(wow ^_^)
#elif defined(__ANDROID__)

#endif



#ifdef NE_INCLUDE_GLEW
    #define GLEW_STATIC
#endif // INCLUDE_GLEW


#define NE_INCLUDE_LIBPNG
#define NE_INCLUDE_STATICMESH_LOADER_OBJ
#define NE_INCLUDE_FONT_LOADER_FNT





#endif // COMPILECONFIG_H
