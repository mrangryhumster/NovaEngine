#ifndef IGUIMANAGER_H
#define IGUIMANAGER_H

#include "Object.h"
#include "NTypes.h"

#include "IRenderer.h"
#include "IEventManager.h"

namespace novaengine
{
namespace gui
{

class IGUILayout;

class IGUIManager : public Object
{
public:

    virtual void setActiveLayout(IGUILayout* layout = 0) = 0;
    virtual IGUILayout* getActiveLayout() = 0;
    virtual IGUILayout* getRootLayout() = 0;

    virtual void updateActiveLayout()                  = 0;
    virtual void animateActiveLayout(f32 deltatime_ms) = 0;
    virtual void renderActiveLayout()                  = 0;

    virtual renderer::IRenderer* getRenderer() = 0;
    virtual IEventManager*       getEventManager() = 0;

protected:
private:
};

}
}

#endif // IGUIMANAGER_H
