#include "CNovaEngine.h"
//------------------------------------------------------
#if     defined(NE_WINDOW_WIN32)
#include "window\CWin32Window.h"
#elif   defined(NE_WINDOW_ANDROID)
#include "window\CAndroidWindow.h"
#endif // NE_WINDOW_WIN32

#if   defined(NE_OPENGL_RENDERER)
#include "opengl\COpenGLRenderer.h"
#elif defined(NE_OPENGLES1_RENDERER)
#include "opengles1/COpenGLES1Renderer.h"
#endif // NE_OPENGL_RENDERER
//------------------------------------------------------

#include "NLog.h"
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
    noerror(true),
    exit(false)
{

    LOG_INFO("NovaEngine %s \"%s\" %s.%s.%s \n",NOVAENGINE_FULLVERSION_STRING,NOVAENGINE_STATUS,NOVAENGINE_YEAR,NOVAENGINE_MONTH,NOVAENGINE_DATE);
#define NE_DEBUG
#ifdef NE_DEBUG
    log::CLog::get()->set_log_level(log::ELL_ENGINE_DEBUG);
#else
    log::CLog::get()->set_log_level(engine_conf.LogLevel);
#endif // NE_DEBUG
    EventManager = new CEventManager();

#if defined(NE_WINDOW_WIN32)
    Window = new window::CWin32Window(engine_conf,EventManager);
#elif defined(NE_WINDOW_ANDROID)
    Window = new window::CAndroidWindow(engine_conf,EventManager);

    //WARNING: SOME SHIT
    //because we cant create gl context before get APP_CMD_INIT_WINDOW
    //we must wait to it(and wait 3s)
    //--------------------------------------------------------------------------------
    LOG_ENGINE_DEBUG("Waiting APP_CMD_INIT_WINDOW event...");
    bool* WindowReady = (bool*)Window->getWindowInternalVariable("ready");
    u32  ReadyCheckStart = time::getRealTime();
    while(!(*WindowReady))
    {
        WindowReady = (bool*)Window->getWindowInternalVariable("ready");
        if((time::getRealTime() - ReadyCheckStart) < 3000)
            Window->update();
        else
            break;
    }
    LOG_ENGINE_DEBUG("Waiting end...");
    if((*WindowReady))
    {
        LOG_INFO("Window init. done in %d ms.\n",(time::getRealTime() - ReadyCheckStart));
    }
    else
    {
        LOG_FATAL_ERROR("Window init. timout expires...");
        noerror = false;
    }
    //--------------------------------------------------------------------------------
#else
    LOG_FATAL_ERROR("Ops.. No window system selected.\n");
#endif
    if(!noerror and (not Window or not Window->isOk()))
    {
        LOG_FATAL_ERROR("Cannot create window class\n");
        noerror = false;
    }

    switch(engine_conf.Renderer)
    {

    case renderer::ERT_OPENGL:
#if   defined(NE_OPENGL_RENDERER)
        Renderer = new renderer::COpenGLRenderer(Window,engine_conf);
#elif defined(NE_OPENGLES1_RENDERER)
        Renderer = new renderer::COpenGLES1Renderer(Window,engine_conf);
#else
        LOG_FATAL_ERROR("OpenGL renderer not compiled in current engine version\n");
#endif // NE_OPENGL_RENDERER
        break;
    default:
        LOG_FATAL_ERROR("Unknown renderer selected\n");
    }
    if(!noerror and (not Renderer or not Renderer->isOk()))
    {
        LOG_FATAL_ERROR("Cannot create renderer class\n");
        noerror = false;
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
log::ILog* CNovaEngine::getLog()
{
    return log::CLog::get();
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
    if(!Window->update())
        noerror = false;
    if(!Renderer->update())
        noerror = false;

    return isOk();
}
//-------------------------------------------------------------------------------------------
}
