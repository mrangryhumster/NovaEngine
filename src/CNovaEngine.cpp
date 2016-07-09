#include "CNovaEngine.h"
//------------------------------------------------------
#if     defined(NE_WINDOW_WIN32)
#include "CWin32Window.h"
#elif   defined(NE_WINDOW_ANDROID)
#include "CAndroidWindow.h"
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
#include "CGUIManager.h"

namespace novaengine
{

CNovaEngine::CNovaEngine(SEngineConf engine_conf):
    m_EventManager(nullptr),
    m_Window(nullptr),
    m_Renderer(nullptr),
    m_FileSystem(nullptr),
    m_ResourceManager(nullptr),
	m_GeometryManager(nullptr),
    m_SceneManager(nullptr),
    m_GUIManager(nullptr),
    m_PerformanceCounter(nullptr),
    m_noerror(true),
    m_exit(false)
{

    LOG_INFO("NovaEngine %s \"%s\" [Builded : %s - %s] \n",NOVAENGINE_FULLVERSION_STRING,NOVAENGINE_STATUS,__TIME__,__DATE__);

#ifdef NE_DEBUG
    log::CLogger::get()->set_log_level(log::ELL_ENGINE_DEBUG);
#else
    log::CLogger::get()->set_log_level(engine_conf.LogLevel);
#endif // NE_DEBUG
//------------------------------------------
    m_EventManager       = new CEventManager();
    m_PerformanceCounter = new CPerformanceCounter();
//------------------------------------------

#if defined(NE_WINDOW_WIN32)
    m_Window = new window::CWin32Window(engine_conf,m_EventManager); //Создание окна для Windows
#elif defined(NE_WINDOW_ANDROID)
    Window = nullptr; //!< Android support removed temporary due rewrites of window class(again)
    //Over and over
    //Over and over
    //I rewrite this piece of shiiiit...
#elif defined(NE_WINDOW_X11)
#error "Oops missing code"
#else
#error "No window system selected"
#endif
    if(m_Window == nullptr || !m_Window->isOk())
    {
        LOG_FATAL_ERROR("Cannot create window class\n");
        m_noerror = false;
        return;
    }


    switch(engine_conf.Renderer)
    {
    case renderer::ERT_OPENGL:
#if   defined(NE_OPENGL_RENDERER)
		{
			renderer::COpenGLRenderer* OpenGLRenderer = new renderer::COpenGLRenderer(m_PerformanceCounter, m_Window, engine_conf);
			m_EventManager->registerEventListener((IEventListener*)OpenGLRenderer, EET_WINDOW_EVENT, 1);
			m_Renderer = OpenGLRenderer;
		}
#elif defined(NE_OPENGLES1_RENDERER)
        Renderer = new renderer::COpenGLES1Renderer(Window,engine_conf);
#else
        LOG_FATAL_ERROR("OpenGL renderer not compiled in current engine version\n");
#endif // NE_OPENGL_RENDERER
        break;
    default:
        LOG_FATAL_ERROR("Unknown renderer selected\n");
    }
    if(m_Renderer == nullptr || !m_Renderer->isOk())
    {
        LOG_FATAL_ERROR("Cannot create renderer class\n");
        m_noerror = false;
        return;
    }

    m_FileSystem      = new io::CFileSystem();
    m_ResourceManager = new CResourceManager(m_FileSystem);
    m_GeometryManager = new CGeometryManager(m_ResourceManager);
    m_SceneManager    = new scene::CSceneManager(m_Renderer,m_EventManager);
    m_GUIManager      = new gui::CGUIManager(m_Renderer,m_EventManager);
}
//-------------------------------------------------------------------------------------------
CNovaEngine::~CNovaEngine()
{
    LOG_ENGINE_DEBUG("Release GUIManager...\n");
    if(m_GUIManager)
        m_GUIManager->release();
    LOG_ENGINE_DEBUG("Release SceneManager...\n");
    if(m_SceneManager)
        m_SceneManager->release();
    LOG_ENGINE_DEBUG("Release GeometryManager...\n");
    if(m_GeometryManager)
        m_GeometryManager->release();
    LOG_ENGINE_DEBUG("Release ResourceManager...\n");
    if(m_ResourceManager)
        m_ResourceManager->release();
    LOG_ENGINE_DEBUG("Release FileSystem...\n");
    if(m_FileSystem)
        m_FileSystem->release();

    LOG_ENGINE_DEBUG("Release Renderer...\n");
    if(m_Renderer)
        m_Renderer->release();
    LOG_ENGINE_DEBUG("Release Window...\n");
    if(m_Window)
        m_Window->release();

    LOG_ENGINE_DEBUG("Release EnginePerformanceCounter...\n");
    if(m_PerformanceCounter)
        m_PerformanceCounter->release();

    LOG_ENGINE_DEBUG("Release EventManager...\n");
    if(m_EventManager)
        m_EventManager->release();


    LOG_INFO("Engine closed\n");
}
//-------------------------------------------------------------------------------------------
IEventManager*    CNovaEngine::getEventManager()
{
    return m_EventManager;
}
//-------------------------------------------------------------------------------------------
window::IWindow* CNovaEngine::getWindow()
{
    return m_Window;
}
//-------------------------------------------------------------------------------------------
renderer::IRenderer* CNovaEngine::getRenderer()
{
    return m_Renderer;
}
//-------------------------------------------------------------------------------------------
io::IFileSystem* CNovaEngine::getFileSystem()
{
    return m_FileSystem;
}
//-------------------------------------------------------------------------------------------
IResourceManager*CNovaEngine::getResourceManager()
{
    return m_ResourceManager;
}
//-------------------------------------------------------------------------------------------
IGeometryManager* CNovaEngine::getGeometryManager()
{
    return m_GeometryManager;
}
//-------------------------------------------------------------------------------------------
scene::ISceneManager*   CNovaEngine::getSceneManager()
{
    return m_SceneManager;
}
//-------------------------------------------------------------------------------------------
gui::IGUIManager* CNovaEngine::getGUIManager()
{
    return m_GUIManager;
}
//-------------------------------------------------------------------------------------------
log::ILogger* CNovaEngine::getLogger()
{
    return log::CLogger::get();
}
//-------------------------------------------------------------------------------------------
IPerformanceCounter* CNovaEngine::getPerformanceCounter()
{
    return m_PerformanceCounter;
}
//-------------------------------------------------------------------------------------------
bool CNovaEngine::isOk()
{
    if(m_noerror == false)
        return false;

    bool WindowError   = false;
    bool RendererError = false;

    if(m_Window)
        WindowError   = !m_Window->isOk();
    if(m_Renderer)
        RendererError = !m_Renderer->isOk();

    m_noerror = !( WindowError || RendererError);

    return (m_noerror && !m_exit);
}
//-------------------------------------------------------------------------------------------
bool CNovaEngine::update()
{
    m_PerformanceCounter->register_frame();
    if(!isOk())
    {
        return false;
    }
    else
    {
        if(!m_Window->update())
            m_noerror = false;
        if(!m_Renderer->update())
            m_noerror = false;

        return (m_noerror);
    }
}
//-------------------------------------------------------------------------------------------
}
