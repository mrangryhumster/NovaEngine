#ifndef IWINDOW_H
#define IWINDOW_H

#include "Object.h"
#include "NTypes.h"
#include "NExTypes.h"


#include "types/dim2.h"
#include "types/vector2.h"

#include "IEventListener.h"

namespace novaengine
{
namespace window
{

class ICursorControl
{
public:

    virtual void setCursorPosition(core::vector2s pos)  = 0;
    virtual core::vector2s getCursorPosition()          = 0;

    virtual void setCursorVisible(bool state)           = 0;
    virtual bool isCursorVisible()                      = 0;

};

class IWindow : public Object
{
public:

    virtual void             setResolution(core::dim2<u32>) = 0;
    virtual core::dim2<u32>  getResolution()                = 0;

    virtual void setFullscreenMode(bool fullscreen) = 0;
    virtual bool isFullscreenMode()                 = 0;

    virtual void setVisible(bool visible)   = 0;
    virtual bool isVisible()                = 0;

    virtual void            setCaption(const wchar_t* caption) = 0;
    virtual const wchar_t*  getCaption()                       = 0;

    virtual ICursorControl* getCursorControl() = 0;

    virtual s32  getWindowType()    = 0;
    virtual int  getWindowID()      = 0;

    virtual bool update() = 0;
    virtual bool isOk()   = 0;


    //! This function the one way to get platform-specific varibles
    //! HWND - win; XDisplay - X11; android_app - android
    virtual void* getWindowInternalVariable(const char* name) = 0;

protected:
private:
};
}
}
#endif // IWINDOW_H
