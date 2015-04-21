#ifndef ITIMER_H
#define ITIMER_H

#include "Object.h"

namespace novaengine
{
namespace time
{

class ITimer : public Object
{
public:

    virtual void Start() = 0;
    virtual void Stop()  = 0;

    virtual void Pause() = 0;
    virtual bool isPaused() = 0;

    virtual f64  getTime() = 0;

protected:
private:
};

class ITimerDelta : public ITimer
{
    public:
        virtual void getDeltaTime() = 0;
};

}
}


#endif // ITIMER_H
