#include "NLogger.h"
#include "ne_types.h"

#include <stdio.h>
#include <string.h>

#include <wchar.h>

#ifdef __ANDROID__
#include <android/log.h>
#endif

namespace novaengine
{
namespace log
{
CLogger::CLogger()
{
    log_level = ELL_INFO;
}
void CLogger::log(u32 p_log_level,const char* message,va_list args)
{
    if(p_log_level < log_level)
        return;

#ifdef __ANDROID__
    int android_ll = ANDROID_LOG_DEFAULT;
    switch(p_log_level)
    {
    case ELL_ENGINE_DEBUG:
        android_ll = ANDROID_LOG_VERBOSE;
        break;
    case ELL_DEBUG:
        android_ll = ANDROID_LOG_DEBUG;
        break;
    case ELL_INFO:
        android_ll = ANDROID_LOG_INFO;
        break;
    case ELL_WARNING:
        android_ll = ANDROID_LOG_WARN;
        break;
    case ELL_ERROR:
        android_ll = ANDROID_LOG_ERROR;
        break;
    case ELL_FATAL_ERROR:
        android_ll = ANDROID_LOG_FATAL;
        break;
    }
    __android_log_vprint(android_ll,"NovaEngine", message, args);
#else
    vprintf(message, args);
#endif
}

void CLogger::log_message(u32 p_log_level,const char*    message ...)
{
    va_list args;
    va_start(args, message);
    log(p_log_level,message, args);
    va_end(args);
}


void CLogger::log_engine_debug(const char* message ...)
{
    va_list args;
    va_start(args, message);
    log(ELL_ENGINE_DEBUG,message,args);
    va_end(args);
}
void CLogger::log_debug(const char* message ...)
{
    va_list args;
    va_start(args, message);
    log(ELL_DEBUG,message,args);
    va_end(args);
}
void CLogger::log_info(const char* message ...)
{
    va_list args;
    va_start(args, message);
    log(ELL_INFO,message,args);
    va_end(args);
}
void CLogger::log_warning(const char* message ...)
{
    va_list args;
    va_start(args, message);
    log(ELL_WARNING,message,args);
    va_end(args);
}
void CLogger::log_error(const char* message ...)
{
    va_list args;
    va_start(args, message);
    log(ELL_ERROR,message,args);
    va_end(args);
}
void CLogger::log_fatal_error(const char* message ...)
{
    va_list args;
    va_start(args, message);
    log(ELL_FATAL_ERROR,message,args);
    va_end(args);
}

void CLogger::set_log_level(u32 p_log_level)
{
    log_level = p_log_level;
}
u32  CLogger::get_log_level()
{
    return log_level;
}
}
}

