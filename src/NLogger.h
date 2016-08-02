#ifndef NLOG_H
#define NLOG_H

#include "ne_types.h"
#include "ILogger.h"

#include <cstdarg>

namespace novaengine
{

namespace log
{

class CLogger : public ILogger
{
public:
    CLogger();

    static CLogger* get()
    {
        static CLogger logger;
        return &logger;
    }

    void log(u32 p_log_level,const char* message,va_list args);

    void log_message(u32 log_level,const char*    message ...);

    void log_engine_debug(const char* message ...);
    void log_debug(const char* message ...);
    void log_info(const char* message ...);
    void log_warning(const char* message ...);
    void log_error(const char* message ...);
    void log_fatal_error(const char* message ...);

    void set_log_level(u32 log_level);
    u32  get_log_level();

protected:
private:

    u32 log_level;
};

#define LOG_ENGINE_DEBUG(...)   novaengine::log::CLogger::get()->log_engine_debug(__VA_ARGS__)
#define LOG_DEBUG(...)          novaengine::log::CLogger::get()->log_debug(__VA_ARGS__)
#define LOG_INFO(...)           novaengine::log::CLogger::get()->log_info(__VA_ARGS__)
#define LOG_WARNING(...)        novaengine::log::CLogger::get()->log_warning(__VA_ARGS__)
#define LOG_ERROR(...)          novaengine::log::CLogger::get()->log_error(__VA_ARGS__)
#define LOG_FATAL_ERROR(...)    novaengine::log::CLogger::get()->log_fatal_error(__VA_ARGS__)

}

}
#endif // NLOG_H
