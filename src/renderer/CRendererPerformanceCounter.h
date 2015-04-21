#ifndef CFPSCOUNTER_H
#define CFPSCOUNTER_H

#include "NTypes.h"
#include "NTime.h"
#include "IRendererPerformanceCounter.h"

namespace novaengine
{
namespace renderer
{

class CRendererPerformanceCounter : public IRendererPerformanceCounter
{
    public:
        CRendererPerformanceCounter();
        virtual ~CRendererPerformanceCounter();

        void register_frame_begin();
        void register_frame_end();
        void register_draw(u32 Vertices,u32 VertexInPrimitive);

        f32 getFramePerSecond();
        f32 getMilisecondPerFrame();


        u32 getPrimitevesPerFrame();
        u32 getVerticesPerFrame();
        u32 getDrawCallPerFrame();

    protected:
    private:

    //FPS/MPS
    f32 FramePerSecond;
    f32 MilisecondPerFrame;

    u32 FrameCount;
    u32 DeltaUpdate;
    u32 CounterUpdate;

    //PPF/VPF/DCPF
    u32 PrimitivesPerFrame;
    u32 VerticesPerFrame;
    u32 DrawCallPerFrame;
    //sup
    u32 PrimitivesCount;
    u32 VerticesCount;
    u32 DrawCallCount;


};

}
}
#endif // CFPSCOUNTER_H
