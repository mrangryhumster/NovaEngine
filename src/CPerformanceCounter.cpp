#include "CPerformanceCounter.h"

//#include "ne_math.h"

namespace novaengine
{

CPerformanceCounter::CPerformanceCounter():
    FramesPerSecond(0),
    MilisecondsPerFrame(0),
    FramesCount(0),
    DeltaUpdate(time::getRealTime()),
    CounterUpdate(250),
    VerticesPerFrame(0),
    DrawCallPerFrame(0),
    VerticesCount(0),
    DrawCallCount(0),
    PrecisionTime(0)
{
    //Start PerfTimer
    PrecisionTime = time::getPerfTime();

}
//-------------------------------------------------------------------------------------------
CPerformanceCounter::~CPerformanceCounter()
{
    //dtor
}
//-------------------------------------------------------------------------------------------
void CPerformanceCounter::register_frame()
{

    FramesCount++;

    const f32 RealTime = time::getPerfTime();
    const f32 Time = RealTime - DeltaUpdate;

    if(Time >= CounterUpdate)
    {
		FramesPerSecond = ((f32)FramesCount/Time) * 1000.0f;

        FramesCount = 0;
        DeltaUpdate = RealTime;
        //---------------------
    }

    MilisecondsPerFrame = time::getPerfTime() - PrecisionTime;
    PrecisionTime = time::getPerfTime();

    //Reset Vertices/DrawCall counters
    VerticesPerFrame   = VerticesCount;
    DrawCallPerFrame   = DrawCallCount;

    VerticesCount   = 0;
    DrawCallCount   = 0;

}
//-------------------------------------------------------------------------------------------
void CPerformanceCounter::register_draw(u32 Vertices)
{
    VerticesCount += Vertices;
    DrawCallCount++;
}
//-------------------------------------------------------------------------------------------
f32 CPerformanceCounter::getFramesPerSecond()
{
    return FramesPerSecond;
}
//-------------------------------------------------------------------------------------------
f32 CPerformanceCounter::getMilisecondsPerFrame()
{
    return MilisecondsPerFrame;
}
//-------------------------------------------------------------------------------------------
u32 CPerformanceCounter::getVerticesPerFrame()
{
    return VerticesPerFrame;
}
//-------------------------------------------------------------------------------------------
u32 CPerformanceCounter::getDrawCallsPerFrame()
{
    return DrawCallPerFrame;
}
//-------------------------------------------------------------------------------------------
}
