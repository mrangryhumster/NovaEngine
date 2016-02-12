#include "CNovaEngine.h"

//------------------------------------------------------
#if     defined(NE_WINDOW_WIN32)
#include "CWin32Window.h"
#elif   defined(NE_WINDOW_ANDROID)
#include "CAndroidWindow.h"
#elif   defined(NE_WINDOW_X11)
#include "CX11Window.h"
#endif // NE_WINDOW_WIN32

#if   defined(NE_OPENGL_RENDERER)
#include "COpenGLRenderer.h"
#elif defined(NE_OPENGLES1_RENDERER)
#include "COpenGLES1Renderer.h"
#endif // NE_OPENGL_RENDERER
//------------------------------------------------------

#include "NLogger.h"
#include "CEventManager.h"
#include "CFileSystem.h"
#include "CResourceManager.h"
#include "CGeometryManager.h"
#include "CSceneManager.h"

namespace novaengine
{

CNovaEngine::CNovaEngine(SEngineConf engine_conf):
    EventManager(nullptr),
    Window(nullptr),
    Renderer(nullptr),
    FileSystem(nullptr),
    ResourceManager(nullptr),
    SceneManager(nullptr),
    PerformanceCounter(nullptr),
    noerror(true),
    exit(false)
{

    LOG_INFO("NovaEngine %s \"%s\" %s.%s.%s \n",NOVAENGINE_FULLVERSION_STRING,NOVAENGINE_STATUS,NOVAENGINE_YEAR,NOVAENGINE_MONTH,NOVAENGINE_DATE);

#ifdef NE_DEBUG
    log::CLogger::get()->set_log_level(log::ELL_ENGINE_DEBUG);
#else
    log::CLogger::get()->set_log_level(engine_conf.LogLevel);
#endif // NE_DEBUG
//------------------------------------------
    EventManager       = new CEventManager();
    PerformanceCounter = new CPerformanceCounter();
//------------------------------------------
#if defined(NE_WINDOW_WIN32)
    Window = new window::CWin32Window(engine_conf,EventManager); //Создание окна для Windows
#elif defined(NE_WINDOW_ANDROID)
    Window = nullptr; //!< Android support removed temporary due rewrites of window class(again)
    //Over and over
    //Over and over
    //I rewrite this piece of shiiiit...
#elif defined(NE_WINDOW_X11)
    Window = new window::CX11Window(engine_conf,EventManager);
#else
#error "No window system selected"
#endif
    if(Window == nullptr || !Window->isOk())
    {
        LOG_FATAL_ERROR("Cannot create window class\n");
        noerror = false;
        return;
    }


    switch(engine_conf.Renderer)
    {
    case renderer::ERT_OPENGL:
#if   defined(NE_OPENGL_RENDERER)
        Renderer = new renderer::COpenGLRenderer(PerformanceCounter,Window,engine_conf);
#elif defined(NE_OPENGLES1_RENDERER)
        Renderer = new renderer::COpenGLES1Renderer(Window,engine_conf);
#else
        LOG_FATAL_ERROR("OpenGL renderer not compiled in current engine version\n");
#endif // NE_OPENGL_RENDERER
        break;
    default:
        LOG_FATAL_ERROR("Unknown renderer selected\n");
    }
    if(Renderer == nullptr || !Renderer->isOk())
    {
        LOG_FATAL_ERROR("Cannot create renderer class\n");
        noerror = false;
        return;
    }

    FileSystem      = new io::CFileSystem();
    ResourceManager = new CResourceManager(FileSystem);
    GeometryManager = new CGeometryManager(ResourceManager);
    SceneManager    = new scene::CSceneManager(Renderer,EventManager);
}
//-------------------------------------------------------------------------------------------
CNovaEngine::~CNovaEngine()
{
    LOG_ENGINE_DEBUG("Release SceneManager...\n");
    if(SceneManager)
        SceneManager->release();
    LOG_ENGINE_DEBUG("Release GeometryManager...\n");
    if(GeometryManager)
        GeometryManager->release();
    LOG_ENGINE_DEBUG("Release ResourceManager...\n");
    if(ResourceManager)
        ResourceManager->release();
    LOG_ENGINE_DEBUG("Release FileSystem...\n");
    if(FileSystem)
        FileSystem->release();

    LOG_ENGINE_DEBUG("Release Renderer...\n");
    if(Renderer)
        Renderer->release();
    LOG_ENGINE_DEBUG("Release Window...\n");
    if(Window)
        Window->release();

    LOG_ENGINE_DEBUG("Release EnginePerformanceCounter...\n");
    if(PerformanceCounter)
        PerformanceCounter->release();

    LOG_ENGINE_DEBUG("Release EventManager...\n");
    if(EventManager)
        EventManager->release();


    LOG_INFO("Engine closed\n");
}
//-------------------------------------------------------------------------------------------
IEventManager*    CNovaEngine::getEventManager()
{
    return EventManager;
}
//-------------------------------------------------------------------------------------------
window::IWindow* CNovaEngine::getWindow()
{
    return Window;
}
//-------------------------------------------------------------------------------------------
renderer::IRenderer* CNovaEngine::getRenderer()
{
    return Renderer;
}
//-------------------------------------------------------------------------------------------
io::IFileSystem* CNovaEngine::getFileSystem()
{
    return FileSystem;
}
//-------------------------------------------------------------------------------------------
IResourceManager*CNovaEngine::getResourceManager()
{
    return ResourceManager;
}
//-------------------------------------------------------------------------------------------
IGeometryManager* CNovaEngine::getGeometryManager()
{
    return GeometryManager;
}
//-------------------------------------------------------------------------------------------
scene::ISceneManager*   CNovaEngine::getSceneManager()
{
    return SceneManager;
}
//-------------------------------------------------------------------------------------------
log::ILogger* CNovaEngine::getLogger()
{
    return log::CLogger::get();
}
//-------------------------------------------------------------------------------------------
IPerformanceCounter* CNovaEngine::getPerformanceCounter()
{
    return PerformanceCounter;
}
//-------------------------------------------------------------------------------------------
bool CNovaEngine::isOk()
{
    if(noerror == false)
        return false;

    bool WindowError   = false;
    bool RendererError = false;

    if(Window)
        WindowError   = !Window->isOk();
    if(Renderer)
        RendererError = !Renderer->isOk();

    noerror = !( WindowError || RendererError);

    return (noerror && !exit);
}
//-------------------------------------------------------------------------------------------
bool CNovaEngine::update()
{
    PerformanceCounter->register_frame();
    if(!isOk())
    {
        return false;
    }
    else
    {
        if(!Window->update())
            noerror = false;
        if(!Renderer->update())
            noerror = false;

        return (noerror);
    }
}
//-------------------------------------------------------------------------------------------
}
