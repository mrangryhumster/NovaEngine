#ifdef NE_WINDOW_ANDROID

#include "CAndroidWindow.h"
#include <map>

namespace novaengine
{
namespace window
{
CAndroidWindow* AndroidWindow = NULL;
//EventProc for android
static void engine_handle_cmd(struct android_app* app, int32_t cmd)
{
    SEvent event;
    switch (cmd)
    {
        case APP_CMD_START:
        LOG_ENGINE_DEBUG("APP_CMD_START");
            break;
        case APP_CMD_STOP:
        LOG_ENGINE_DEBUG("APP_CMD_STOP");
            break;
        case APP_CMD_PAUSE:
        LOG_ENGINE_DEBUG("APP_CMD_PAUSE");
            break;
        case APP_CMD_RESUME:
        LOG_ENGINE_DEBUG("APP_CMD_RESUME");
            break;
        case APP_CMD_SAVE_STATE:
        LOG_ENGINE_DEBUG("APP_CMD_SAVE_STATE");
            break;
        case APP_CMD_INIT_WINDOW:
        LOG_ENGINE_DEBUG("APP_CMD_INIT_WINDOW");
        event.event_type = EET_WINDOW_EVENT;
        event.window.event_type = EWET_INIT_DONE;
        AndroidWindow->pushEvent(event);
            break;
        case APP_CMD_TERM_WINDOW:
        LOG_ENGINE_DEBUG("APP_CMD_TERM_WINDOW");
            break;
        case APP_CMD_GAINED_FOCUS:
        LOG_ENGINE_DEBUG("APP_CMD_GAINED_FOCUS");
            break;
        case APP_CMD_LOST_FOCUS:
        LOG_ENGINE_DEBUG("APP_CMD_LOST_FOCUS");
            break;
    }
}

CAndroidWindow::CAndroidWindow(SEngineConf conf,IEventListener* event_proc):
    ready(false),
    noerror(true),
    exit(false),
    EventHandler(event_proc)
{
    setObjectName("CAndroidWindow");
    LOG_ENGINE_DEBUG("CAndroidWindow() begin\n");

    if(conf.ExternalWindowID)
    {
        AndroidWindow = this;

        AndroidAPP = conf.ExternalWindowID;

        AndroidAPP->onAppCmd = engine_handle_cmd;
    }
    else
    {
        noerror = false;
        LOG_FATAL_ERROR("ExternalWindowID is NULL\n");
    }
    LOG_ENGINE_DEBUG("CAndroidWindow() end\n");
}
//-------------------------------------------------------------------------------------------
CAndroidWindow::~CAndroidWindow()
{

}
//-------------------------------------------------------------------------------------------
void CAndroidWindow::setResolution(core::dim2<u32> WinSize)
{

}
//-------------------------------------------------------------------------------------------
core::dim2<u32>  CAndroidWindow::getResolution()
{

}
//-------------------------------------------------------------------------------------------
void CAndroidWindow::setFullscreenMode(bool setfullscreen)
{

}
//-------------------------------------------------------------------------------------------
bool CAndroidWindow::isFullscreenMode()
{
}
//-------------------------------------------------------------------------------------------
void CAndroidWindow::setVisible(bool visible)
{

}
//-------------------------------------------------------------------------------------------
bool CAndroidWindow::isVisible()
{

}
//-------------------------------------------------------------------------------------------
void CAndroidWindow::setCaption(const wchar_t* caption)
{

}
//-------------------------------------------------------------------------------------------
const wchar_t*  CAndroidWindow::getCaption()
{

}
//-------------------------------------------------------------------------------------------
s32  CAndroidWindow::getWindowType()
{
    return 0;
}
//-------------------------------------------------------------------------------------------
int  CAndroidWindow::getWindowID()
{

}
//-------------------------------------------------------------------------------------------
bool CAndroidWindow::update()
{
    pullEvents();
    return isOk();
}
//-------------------------------------------------------------------------------------------
bool CAndroidWindow::isOk()
{
    return (noerror && !exit);
}
//-------------------------------------------------------------------------------------------
void CAndroidWindow::pullEvents()
{

        int ident;
        int events;
        struct android_poll_source* source;

        while ((ident=ALooper_pollAll(0, NULL, &events, (void**)&source)) >= 0)
        {

            if (source != NULL)
            {
                source->process(AndroidAPP, source);
            }

        }

}
//-------------------------------------------------------------------------------------------
void CAndroidWindow::pushEvent(SEvent event)
{
    if(event.event_type == EET_WINDOW_EVENT && event.window.event_type == EWET_INIT_DONE)
        ready = true;

    EventHandler->OnEvent(event);
}
//-------------------------------------------------------------------------------------------
void CAndroidWindow::loadKeyMap()
{

}
//-------------------------------------------------------------------------------------------
void* CAndroidWindow::getWindowInternalVariable(const char* name)
{
    if(strcmp(name,"android_app") == 0)
        return AndroidAPP;
    if(strcmp(name,"ready") == 0)
        return &ready;
    LOG_ERROR("Request for unknow varible...[%s]\n",name);
    return NULL;
}
//-------------------------------------------------------------------------------------------
}
}
#endif // NE_WINDOW_ANDROID
