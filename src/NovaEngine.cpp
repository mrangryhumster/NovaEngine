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

_NOVAENGINE_API INovaEngine* createEngine(renderer::E_RENDERER_TYPE renderer_type,u32 window_width,u32 window_height)
{
    SEngineConf engine_conf;
    engine_conf.Renderer = renderer_type;
    engine_conf.WindowSize.width  = window_width;
    engine_conf.WindowSize.height = window_height;
    return createEngineEx(engine_conf);
}

_NOVAENGINE_API INovaEngine* createEngineEx(SEngineConf engine_conf)
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


    LOG_FATAL_ERROR("Error while loading NovaEngine occurred ...\n");
    delete NovaEngine;
    NovaEngine = NULL;
    return NULL;
}

_NOVAENGINE_API void closeEngine()
{
    if(NovaEngine)
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
