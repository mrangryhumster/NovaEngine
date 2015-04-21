#ifndef IRENDERERPERFORMANCECOUNTER_H
#define IRENDERERPERFORMANCECOUNTER_H

#include "NTypes.h"

namespace novaengine
{
namespace renderer
{

class IRendererPerformanceCounter
{
public:
        virtual f32 getFramePerSecond()            = 0;
        virtual f32 getMilisecondPerFrame()        = 0;

        virtual u32 getPrimitevesPerFrame()        = 0;
        virtual u32 getVerticesPerFrame()          = 0;
        virtual u32 getDrawCallPerFrame()          = 0;
protected:
private:
};

}
}

#endif // IRENDERERPERFORMANCECOUNTER_H
