#ifndef IEVENTLISTENER_H
#define IEVENTLISTENER_H

#include "Object.h"
#include "SEvent.h"

namespace novaengine
{
class IEventListener
{
    public:
        virtual void OnEvent(SEvent& incoming_event) = 0;
    protected:
    private:
};
}
#endif // IEVENTLISTENER_H
