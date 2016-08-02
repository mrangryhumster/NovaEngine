#ifndef CPERFORMANCECOUNTER_H
#define CPERFORMANCECOUNTER_H

#include "ne_types.h"
#include "NTime.h"
#include "IPerformanceCounter.h"

namespace novaengine
{

class CPerformanceCounter : public IPerformanceCounter
{
public:
    CPerformanceCounter();
    virtual ~CPerformanceCounter();

    void register_frame();
    void register_draw(u32 Vertices);


    f32 getFramesPerSecond();
    f32 getMilisecondsPerFrame();

    u32 getVerticesPerFrame();
    u32 getDrawCallsPerFrame();

protected:
private:

    //FPS/MPS
    f32 FramesPerSecond;
    f32 MilisecondsPerFrame;

    u32 FramesCount;
    f32 DeltaUpdate;
    u32 CounterUpdate;

    //VPF/DCPF
    u32 VerticesPerFrame;
    u32 DrawCallPerFrame;
    //sup
    u32 VerticesCount;
    u32 DrawCallCount;
    f64 PrecisionTime;
};

}

#endif // CPERFORMANCECOUNTER_H
