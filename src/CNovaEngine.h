#ifndef CNOVAENGINE_H
#define CNOVAENGINE_H

#include "version.h"
#include "INovaEngine.h"
#include "SEngineConf.h"
#include "IWindow.h"
#include "IRenderer.h"
#include "IEventManager.h"
#include "IFileSystem.h"
#include "IResourceManager.h"

#include "CPerformanceCounter.h"

namespace novaengine
{

class CNovaEngine : public INovaEngine
{
public:
    CNovaEngine(SEngineConf engine_conf);
    virtual ~CNovaEngine();

    IEventManager*          getEventManager();
    window::IWindow*        getWindow();
    renderer::IRenderer*    getRenderer();
    io::IFileSystem*        getFileSystem();
    IResourceManager*       getResourceManager();
    IGeometryManager*       getGeometryManager();
    scene::ISceneManager*   getSceneManager();
    gui::IGUIManager*       getGUIManager();
    log::ILogger*           getLogger();

    IPerformanceCounter*    getPerformanceCounter();

    bool isOk();
    bool update();

protected:
private:

    IEventManager*          m_EventManager;
    window::IWindow*        m_Window;
    renderer::IRenderer*    m_Renderer;
    io::IFileSystem*        m_FileSystem;
    IResourceManager*       m_ResourceManager;
    IGeometryManager*       m_GeometryManager;
    scene::ISceneManager*   m_SceneManager;
    gui::IGUIManager*       m_GUIManager;
    //--------------------------
    CPerformanceCounter*    m_PerformanceCounter;
    //--------------------------
    bool m_noerror;
    bool m_exit;
    //--------------------------
};

}
#endif // CNOVAENGINE_H
