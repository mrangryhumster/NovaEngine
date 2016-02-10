#ifndef IEVENTHANDLER_H
#define IEVENTHANDLER_H

#include "Object.h"
#include "SEvent.h"

namespace novaengine
{
class IEventListener : public Object
{
    public:
        virtual bool OnEvent(SEvent incoming_event) = 0;
    protected:
    private:
};
}
#endif // IEVENTHANDLER_H
