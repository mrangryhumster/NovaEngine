#include "CPerformanceCounter.h"

namespace novaengine
{

CPerformanceCounter::CPerformanceCounter():
    FramesPerSecond(60),
    MilisecondsPerFrame(16.6),
    FramesCount(0),
    DeltaUpdate(time::getRealTime()),
    CounterUpdate(1000),
    VerticesPerFrame(0),
    DrawCallPerFrame(0),
    VerticesCount(0),
    DrawCallCount(0)
{
    //ctor
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

    const u32 RealTime = time::getRealTime();
    const u32 Time = RealTime - DeltaUpdate;

    if(Time >= CounterUpdate)
    {
        FramesPerSecond = (FramesCount/(f32)Time)*1000;

        if(FramesCount == 0)
            MilisecondsPerFrame = 0;
        else
            MilisecondsPerFrame = 1000 / (f32)FramesPerSecond;

        FramesCount=0;
        DeltaUpdate = RealTime;
        //---------------------
    }

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
