#ifndef NTIME_H
#define NTIME_H

#include <chrono>
#include <time.h>
#include <sys/time.h>

#include "NTypes.h"

#include "ITimer.h"
#include "NLogger.h"
namespace novaengine
{
namespace time
{

static inline u64 getRealTime()
{
    timeval tv;
    gettimeofday(&tv,0);
    return (u64) (tv.tv_sec * 1000) + (tv.tv_usec / 1000);

}

static inline u64 Clock_Seconds()
{
    return std::chrono::time_point_cast<std::chrono::seconds>(std::chrono::steady_clock::now()).time_since_epoch().count();
}
static inline u64 Clock_Milliseconds()
{
    return std::chrono::time_point_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now()).time_since_epoch().count();
}
static inline u64 Clock_Microseconds()
{
    return std::chrono::time_point_cast<std::chrono::microseconds>(std::chrono::steady_clock::now()).time_since_epoch().count();
}

}
}


#endif // NTIME_H
