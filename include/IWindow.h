#ifndef IWINDOW_H
#define IWINDOW_H

#include "Object.h"
#include "ne_types.h"


#include "dim2.h"
#include "vector2.h"

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

    virtual void        setTittle(const char*) = 0;
    virtual const char* getTittle() = 0;

    virtual ICursorControl* getCursorControl() = 0;

    virtual bool update() = 0;
    virtual bool isOk()   = 0;

    //! This function the only one way to get platform-specific varibles
    //! HWND - win; XDisplay - X11; android_app - android
    virtual void* getWindowInternalVariable(const char* name) = 0;

protected:
private:
};
}
}
#endif // IWINDOW_H
