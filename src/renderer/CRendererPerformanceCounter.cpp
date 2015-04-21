#include "CRendererPerformanceCounter.h"

namespace novaengine
{
namespace renderer
{

CRendererPerformanceCounter::CRendererPerformanceCounter():
    FramePerSecond(60),
    MilisecondPerFrame(16.6),
    FrameCount(0),
    DeltaUpdate(time::getRealTime()),
    CounterUpdate(1000),
    PrimitivesPerFrame(0),
    VerticesPerFrame(0),
    DrawCallPerFrame(0),
    PrimitivesCount(0),
    VerticesCount(0),
    DrawCallCount(0)
{

}

CRendererPerformanceCounter::~CRendererPerformanceCounter()
{
    //dtor
}
void CRendererPerformanceCounter::register_frame_begin()
{

}
void CRendererPerformanceCounter::register_frame_end()
{

    FrameCount++;

    const u32 RealTime = time::getRealTime();
    const u32 Time = RealTime - DeltaUpdate;

    if(Time >= CounterUpdate)
    {
        FramePerSecond = (FrameCount/(f32)Time)*1000;

        if(FrameCount == 0)
            MilisecondPerFrame = 0;
        else
            MilisecondPerFrame = 1000 / (f32)FramePerSecond;

        FrameCount=0;
        DeltaUpdate = RealTime;
        //---------------------
    }

    //Reset Vertices/Primiteves/DrawCall counters
    PrimitivesPerFrame = PrimitivesCount;
    VerticesPerFrame  = VerticesCount;
    DrawCallPerFrame   = DrawCallCount;

    PrimitivesCount = 0;
    VerticesCount  = 0;
    DrawCallCount   = 0;

}

void CRendererPerformanceCounter::register_draw(u32 Vertices,u32 VertexInPrimitive)
{
    PrimitivesCount += (Vertices / VertexInPrimitive);
    VerticesCount  +=  Vertices;
    DrawCallCount   += 1;
}

f32 CRendererPerformanceCounter::getFramePerSecond()
{
    return FramePerSecond;
}

f32 CRendererPerformanceCounter::getMilisecondPerFrame()
{
    return MilisecondPerFrame;
}

u32 CRendererPerformanceCounter::getPrimitevesPerFrame()
{
    return PrimitivesPerFrame;
}

u32 CRendererPerformanceCounter::getVerticesPerFrame()
{
    return VerticesPerFrame;
}

u32 CRendererPerformanceCounter::getDrawCallPerFrame()
{
    return DrawCallPerFrame;
}

}
}
