#ifdef NE_WINDOW_WIN32

#ifndef CWIN32WINDOW_H
#define CWIN32WINDOW_H

#include "NTypes.h"
#include "IWindow.h"
#include "IEventManager.h"

#include "SEngineConf.h"
#include "SEvent.h"
#include "CKeyMap.h"

#include "EKeycodes.h"

#include <windows.h>

namespace novaengine
{
namespace window
{

class CWin32Window : public IWindow
{
public:
    CWin32Window(SEngineConf conf,IEventListener* event_proc);
    virtual ~CWin32Window();

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
    HWND  hWnd;
    HINSTANCE hInstance;
    wchar_t* title;
    //! Here we store window style if we change mode to full screen
    //! (if we return to windowed mode again we will take our style back)
    DWORD NoFullScreenWindowStyle;
    DWORD NoFullScreenWindowStyleEx;

    bool IsFullscreenWindow;
    bool IsExternalWindow;
    bool IsVisible;
    //------------------
    bool noerror;
    bool exit;
    //------------------
    IEventListener* EventHandler;

    class CCursorControl : public ICursorControl
    {
    public:
        void setCursorPosition(core::vector2s pos)
        {
            POINT rp;
            rp.x = 0;
            rp.y = 0;
            ClientToScreen(hWnd, &rp);

            pos.x += rp.x;
            pos.y += rp.y;

            SetCursorPos(pos.x,pos.y);
        }
        core::vector2s getCursorPosition()
        {
            return LastPosition;
        }
        void setCursorVisible(bool state)
        {
            ShowCursor(state);
            Visible = state;
        }
        bool isCursorVisible()
        {
            return Visible;
        }

        core::vector2s  LastPosition;
        bool            Visible;
        HWND            hWnd;

    } CursorControl;

};
}
}
#endif // CWIN32WINDOW_H
#endif
