#ifndef IPERFORMANCECOUNTER_H
#define IPERFORMANCECOUNTER_H

#include "NTypes.h"
#include "Object.h"
namespace novaengine
{

class IPerformanceCounter : public Object
{
    public:

        virtual f32 getFramesPerSecond()     = 0;
        virtual f32 getMilisecondsPerFrame() = 0;

        virtual u32 getVerticesPerFrame()    = 0;
        virtual u32 getDrawCallsPerFrame()   = 0;

    protected:
    private:
};

}
#endif // IPERFORMANCECOUNTER_H
