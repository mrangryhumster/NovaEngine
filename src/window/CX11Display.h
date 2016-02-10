#ifndef CX11DISPLAY_H
#define CX11DISPLAY_H

#include "NTypes.h"
#include "IWindow.h"
#include "IEventManager.h"

#include "SEngineConf.h"
#include "SEvent.h"
#include "CKeyMap.h"

#include "EKeycodes.h"


namespace novaengine
{
namespace window
{
class CX11Display : public IWindow
{
public:
    CX11Display();
    virtual ~CX11Display();

    void             setResolution(core::dim2<u32>) ;
    core::dim2<u32>  getResolution()                ;

    void setFullscreenMode(bool fullscreen) ;
    bool isFullscreenMode()                 ;

    void setVisible(bool visible)   ;
    bool isVisible()                ;

    void            setCaption(const wchar_t* caption) ;
    const wchar_t*  getCaption()                       ;

    ICursorControl* getCursorControl() ;

    s32  getWindowType()    ;
    int  getWindowID()      ;

    bool update() ;
    bool isOk()   ;

    //! This function the only one way to get platform-specific varibles
    //! HWND - win; XDisplay - X11; android_app - android
    void* getWindowInternalVariable(const char* name) ;

protected:
private:
};
}
}
#endif // CX11DISPLAY_H
