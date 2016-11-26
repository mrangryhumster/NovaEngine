#ifndef CX11WINDOW_H
#define CX11WINDOW_H


#include "ne_types.h"
#include "IWindow.h"
#include "IEventManager.h"
#include "IEventListener.h"
#include "SEvent.h"

#include "SEngineConf.h"
#include "CKeyMap.h"

#include "EKeycodes.h"

#include "NLogger.h"

#include <GL/glew.h>
#include <GL/glx.h>
#include <GL/gl.h>


#include <X11/X.h>
#include <X11/Xlib.h>
#include <X11/keysym.h>

#include <stdio.h>
#include <stdlib.h>

namespace novaengine
{
namespace window
{

class CX11Window : public IWindow
{
public:
    CX11Window(SEngineConf conf,IEventManager* EventManager);
    ~CX11Window();

    void             setResolution(core::dim2<u32>) ;
    core::dim2<u32>  getResolution()                ;

    void setFullscreenMode(bool fullscreen) ;
    bool isFullscreenMode()                 ;

    void setVisible(bool visible)   ;
    bool isVisible()                ;

    void        setTittle(const char*) ;
    const char* getTittle() ;

    ICursorControl* getCursorControl() ;

    bool update() ;
    bool isOk()   ;

    //! This function the only one way to get platform-specific varibles
    //! HWND - win; XDisplay - X11; android_app - android
    void* getWindowInternalVariable(const char* name) ;


protected:
private:
    bool poolEvents();
    void loadKeymap();

    Display    *dpy;
    Window      win;
    XVisualInfo *vi;
    GLXContext   cx;
    Colormap    cmap;
    IEventManager* EventManager;

    std::map<int,E_KEY_CODE> KeyMap;
};

}

}
#endif // CX11WINDOW_H
