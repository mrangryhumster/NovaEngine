#ifndef CGUIMANAGER_H
#define CGUIMANAGER_H

#include "IGUIManager.h"
#include "IRenderer.h"

namespace novaengine
{
namespace gui
{

class CGUIManager : public IGUIManager
{
public:
    CGUIManager(renderer::IRenderer* Renderer,IEventManager* EventManager);
    virtual ~CGUIManager();

    void setActiveLayout(IGUILayout* layout = 0);
    IGUILayout* getActiveLayout();
    IGUILayout* getRootLayout();

    void updateActiveLayout();
    void animateActiveLayout(f32 deltatime_ms);
    void renderActiveLayout();

    renderer::IRenderer* getRenderer();
    IEventManager*       getEventManager();

protected:

private:
    renderer::IRenderer* m_Renderer;
    IEventManager*       m_EventManager;
};

}
}
#endif // CGUIMANAGER_H
