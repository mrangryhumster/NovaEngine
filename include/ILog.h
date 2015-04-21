#ifndef ILOG_H
#define ILOG_H

#include "NTypes.h"

namespace novaengine
{
namespace log
{

enum E_LOG_LEVEL
{
    ELL_ALWAYS_SHOW     =  0,
    ELL_ENGINE_DEBUG ,
    ELL_DEBUG        ,
    ELL_INFO         ,
    ELL_WARNING      ,
    ELL_ERROR        ,
    ELL_FATAL_ERROR  ,
    ELL_NEVER_SHOW   ,
};

class ILog
{
public:

    virtual void log_message(u32 log_level,const char*    message ...) = 0;

    virtual void log_debug(const char* message ...) = 0;
    virtual void log_info(const char* message ...) = 0;
    virtual void log_warning(const char* message ...) = 0;
    virtual void log_error(const char* message ...) = 0;
    virtual void log_fatal_error(const char* message ...) = 0;

    virtual void set_log_level(u32 log_level)   = 0;
    virtual u32  get_log_level()                = 0;

protected:
private:
};

}
}

#endif // ILOG_H
