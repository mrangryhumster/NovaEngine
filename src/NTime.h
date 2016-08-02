#ifndef NTIME_H
#define NTIME_H

#include <time.h>

#include "ne_types.h"
#include "NLogger.h"

#include "CompileConfig.h"
#ifdef _NE_WIN32_PLATFORM
#include <windows.h>

class CWin32Timer
{
public:

    CWin32Timer()
    {
        LOG_ENGINE_DEBUG("PerfTimer started...\n");
        LARGE_INTEGER Frequency;
        if(QueryPerformanceFrequency(&Frequency))
        {
            PerformanceTimer = true;
            PerformanceTimerResolution = f64(Frequency.QuadPart) / 1000.0;

            LARGE_INTEGER PerformanceCount;
            QueryPerformanceCounter(&PerformanceCount);
            PerformanceTimerStart = PerformanceCount.QuadPart;
        }
        else
        {
            PerformanceTimer = false;
            ClockTimerStart = clock();
        }
    }

    f64 getTime()
    {
      if(PerformanceTimer)
      {
          LARGE_INTEGER PerformanceCount;
          QueryPerformanceCounter(&PerformanceCount);
          return f64(PerformanceCount.QuadPart- PerformanceTimerStart)/PerformanceTimerResolution;
      }
      else
      {
          return (clock() - ClockTimerStart)/1000.0;
      }
    }

private:

    //Performance counter
    bool    PerformanceTimer;
    f64     PerformanceTimerResolution;
    __int64 PerformanceTimerStart;
    //clock()
    f64     ClockTimerStart;

};
#endif // _NE_WIN32_PLATFORM

namespace novaengine
{
namespace time
{

static inline f64 getPerfTime()
{
#ifdef _NE_WIN32_PLATFORM
    static CWin32Timer Timer;
    return Timer.getTime();
#endif // _NE_WIN32_PLATFORM
}
static inline u64 getRealTime()
{
    return clock();
}

}
}


#endif // NTIME_H
