#ifndef COMPILECONFIG_H
#define COMPILECONFIG_H

//------------Windows
#if defined(__WIN32)

#if defined(__WIN64)
#endif // __WIN64

//------------Apple
#elif defined(__APPLE__)

//------------Linux
#elif defined(__lunux)

//------------Android(wow ^_^)
#elif defined(__ANDROID__)

#endif

#define NE_INCLUDE_LIBPNG

#ifdef NE_INCLUDE_GLEW
    #define GLEW_STATIC
#endif // INCLUDE_GLEW




#endif // COMPILECONFIG_H
