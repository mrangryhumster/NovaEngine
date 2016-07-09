#include "CGUIManager.h"

namespace novaengine
{
namespace gui
{

CGUIManager::CGUIManager(renderer::IRenderer* p_Renderer,IEventManager* p_EventManager):
    m_Renderer(p_Renderer),
    m_EventManager(p_EventManager)
{
    //ctor
}
//-------------------------------------------------------------------------------------------
CGUIManager::~CGUIManager()
{
    //dtor
}
//-------------------------------------------------------------------------------------------
void CGUIManager::setActiveLayout(IGUILayout* layout)
{

}
//-------------------------------------------------------------------------------------------
IGUILayout* CGUIManager::getActiveLayout()
{
	return nullptr;
}
//-------------------------------------------------------------------------------------------
IGUILayout* CGUIManager::getRootLayout()
{
	return nullptr;
}
//-------------------------------------------------------------------------------------------
void CGUIManager::updateActiveLayout()
{

}
//-------------------------------------------------------------------------------------------
void CGUIManager::animateActiveLayout(f32 deltatime_ms)
{

}
//-------------------------------------------------------------------------------------------
void CGUIManager::renderActiveLayout()
{

}
//-------------------------------------------------------------------------------------------
renderer::IRenderer* CGUIManager::getRenderer()
{
    return m_Renderer;
}
//-------------------------------------------------------------------------------------------
IEventManager* CGUIManager::getEventManager()
{
    return m_EventManager;
}

}
}
