#ifndef INOVAENGINE_H
#define INOVAENGINE_H

#include "Object.h"
#include "IEventManager.h"
#include "IWindow.h"
#include "IRenderer.h"
#include "IFileSystem.h"
#include "IResourceManager.h"
#include "IGeometryManager.h"
#include "ISceneManager.h"
#include "IGUIManager.h"
#include "ILogger.h"
#include "IPerformanceCounter.h"

namespace novaengine
{

class INovaEngine : public Object
{
public:
    virtual IEventManager*         getEventManager()       = 0;
    virtual window::IWindow*       getWindow()             = 0;
    virtual renderer::IRenderer*   getRenderer()           = 0;
    virtual io::IFileSystem*       getFileSystem()         = 0;
    virtual IResourceManager*      getResourceManager()    = 0;
    virtual IGeometryManager*      getGeometryManager()    = 0;
    virtual scene::ISceneManager*  getSceneManager()       = 0;
    virtual gui::IGUIManager*      getGUIManager()         = 0;
    virtual log::ILogger*          getLogger()             = 0;

    virtual IPerformanceCounter*   getPerformanceCounter() = 0;

    virtual bool isOk()   = 0;
    virtual bool update() = 0;
};

}
#endif // INOVAENGINE_H
