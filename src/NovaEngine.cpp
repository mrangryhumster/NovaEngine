#include "NovaEngine.h"
#include "CNovaEngine.h"
#include "NLogger.h"

#include "CompileConfig.h"
#include "IWindow.h"
#include "IRenderer.h"

namespace novaengine
{
namespace
{
static INovaEngine* NovaEngine                  = NULL;
static window::IWindow*   NovaEngineWindow      = NULL;
static renderer::IRenderer* NovaEngineRenderer  = NULL;
}

INovaEngine* createEngine()
{
    SEngineConf engine_conf;
    return createEngineEx(engine_conf);
}

INovaEngine* createEngineEx(SEngineConf engine_conf)
{
    if(NovaEngine)
    {
        LOG_ERROR("Another NovaEngine copy already exist\n");
        return NULL;
    }
    NovaEngine = new CNovaEngine(engine_conf);
    if(NovaEngine->isOk())
    {
        NovaEngineWindow   = NovaEngine->getWindow();
        NovaEngineRenderer = NovaEngine->getRenderer();
        return NovaEngine;
    }
    else
    {
        LOG_FATAL_ERROR("Error while loading NovaEngine occurred ...\n");
    }

    delete NovaEngine;
    NovaEngine = NULL;
    return NULL;
}

void closeEngine()
{
    if(not NovaEngine->is_null())
    {
        NovaEngine->release();
        NovaEngine = NULL;
        NovaEngineWindow = NULL;
        NovaEngineRenderer = NULL;
    }
}
INovaEngine* getNovaEngine()
{
    return NovaEngine;
}
namespace window
{
IWindow* getWindow()
{
    return NovaEngineWindow;
}
}

namespace renderer
{
IRenderer* getRenderer()
{
    return NovaEngineRenderer;
}
}

namespace log
{
ILogger* getLogger()
{
    return log::CLogger::get();
}
}

}
