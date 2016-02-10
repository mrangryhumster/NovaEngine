#ifndef CWIN32OPENGLRENDERERWRAP_H
#define CWIN32OPENGLRENDERERWRAP_H

#ifdef NE_WINDOW_WIN32

#include <windows.h>
#include "COpenGLRenderer.h"
#include "log\NLog.h"

#include "GL\glew.h"
#include "wglext.h"

namespace novaengine
{
namespace renderer
{

class CWin32OpenGLRendererWrapper : public COpenGLRenderer
{
public:
    CWin32OpenGLRendererWrapper(window::IWindow* wnd,SEngineConf conf);
    virtual ~CWin32OpenGLRendererWrapper();
    //--------------------------------------------------------------------------
    void setVSync(bool enable);
    bool getVSync();
    //--------------------------------------------------------------------------
    void end_frame();

protected:
private:

    HWND  hWnd;
    HDC   hDC;
    HGLRC hRC;

    bool VSync;

};


}
}
#endif // NE_WINDOW_WIN32

#endif // CWIN32OPENGLRENDERERWRAP_H
