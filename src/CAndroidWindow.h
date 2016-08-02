#ifndef CANDROIDWINDOW_H
#define CANDROIDWINDOW_H

#ifdef NE_WINDOW_ANDROID

#include "ne_types.h"
#include "IWindow.h"
#include "IEventManager.h"
#include "IEventListener.h"
#include "SEvent.h"

#include "SEngineConf.h"
#include "CKeyMap.h"

#include "EKeycodes.h"

#include "NLogger.h"

#include <jni.h>
#include <errno.h>
#include <android/sensor.h>
#include <android_native_app_glue.h>




namespace novaengine
{
namespace window
{

class CAndroidWindow : public IWindow
{
public:
    CAndroidWindow(SEngineConf conf,IEventListener* event_proc);
    virtual ~CAndroidWindow();

    void             setResolution(core::dim2<u32>) ;
    core::dim2<u32>  getResolution()                ;

    void setFullscreenMode(bool fullscreen) ;
    bool isFullscreenMode()                 ;

    void setVisible(bool visible)   ;
    bool isVisible()                ;

    void            setCaption(const wchar_t* caption) ;
    const wchar_t*  getCaption()                       ;

    s32  getWindowType()    ;
    int  getWindowID()      ;

    bool update();
    bool isOk();

    void pullEvents();
    void pushEvent(SEvent event);
    //----------------
    void loadKeyMap();
    //----------------
    void* getWindowInternalVariable(const char* name);
    //----------------
    ICursorControl* getCursorControl()
    {
        return &CursorControl;
    }

protected:
private:

    //------------------
    struct android_app* AndroidAPP;
    //------------------
    bool ready;
    bool noerror;
    bool exit;
    //------------------
    IEventListener* EventHandler;

    class CCursorControl : public ICursorControl
    {
    public:
        void setCursorPosition(core::vector2s pos)
        {
        }
        core::vector2s getCursorPosition()
        {
            return LastPosition;
        }
        void setCursorVisible(bool state)
        {
        }
        bool isCursorVisible()
        {
        }

        core::vector2s  LastPosition;

    } CursorControl;

};
}
}
#endif // NE_WINDOW_ANDROID
#endif // CANDROIDWINDOW_H
