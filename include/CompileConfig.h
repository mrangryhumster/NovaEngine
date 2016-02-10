#if defined(_WIN32)
#define _NE_WIN32_PLATFORM
#endif // _WIN32

#if defined(__APPLE__)
#define _NE_APPLE_PLATFORM
#endif // __APPLE__

#if defined(__linux__)
#define _NE_LINUX_PLATFORM
#endif // __linux__

#if defined(__ANDROID__)
#define _NE_ANDROID_PLATFORM
#endif // __ANDROID__


//Resource loader buildin
//Image
#define _NE_INCLUDE_LIBPNG
//StaticMesh
#define _NE_INCLUDE_STATICMESH_LOADER_OBJ
//Font
#define _NE_INCLUDE_FONT_LOADER_FNT
//-----------------------

//Internal stuff
#if defined(_NE_WIN32_PLATFORM)
#define	_NOVAENGINE_API __declspec(dllexport)
#else 
#define _NOVAENGINE_API
#endif // _WIN32
//----------------------
