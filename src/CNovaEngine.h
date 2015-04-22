#ifndef CNOVAENGINE_H
#define CNOVAENGINE_H

#include "../version.h"
#include "INovaEngine.h"
#include "SEngineConf.h"
#include "IWindow.h"
#include "IRenderer.h"
#include "IEventManager.h"
#include "IFileSystem.h"
#include "IResourceManager.h"

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
    log::ILogger*           getLogger();

    bool isOk();
    bool update();

protected:
private:

    IEventManager*          EventManager;
    window::IWindow*        Window;
    renderer::IRenderer*    Renderer;
    io::IFileSystem*        FileSystem;
    IResourceManager*       ResourceManager;
    IGeometryManager*       GeometryManager;
    scene::ISceneManager*   SceneManager;
    //--------------------------
    bool noerror;
    bool exit;
    //--------------------------
};

}
#endif // CNOVAENGINE_H
