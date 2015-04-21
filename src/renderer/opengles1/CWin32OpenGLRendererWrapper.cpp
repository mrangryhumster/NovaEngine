#include "CWin32OpenGLRendererWrapper.h"
#ifdef NE_WINDOW_WIN32


namespace novaengine
{
namespace renderer
{
CWin32OpenGLRendererWrapper::CWin32OpenGLRendererWrapper(window::IWindow* wnd,SEngineConf conf):
    COpenGLRenderer(wnd,conf),
    hWnd(NULL),
    hDC(NULL),
    hRC(NULL),
    VSync(false)
{
    io::log::writeln(0,"Using opengl win32 wrapper.");
    if(conf.ExternalWindowID == 0)
    {
        PFNWGLCHOOSEPIXELFORMATARBPROC wglChoosePixelFormatARB = NULL;
        PIXELFORMATDESCRIPTOR pfd;
        int pFormat;
        DWORD dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL;

        hWnd = *(HWND*)wnd->getWindowInternalVarible("HWND");
        hDC  = GetDC(hWnd);
        if(!hDC)
        {
            io::log::writeln(0,"Unknown error =(");
        }

        if(conf.DoubleBuf)
            dwFlags |= PFD_DOUBLEBUFFER; //!< DoubleBuf

        ZeroMemory(&pfd,sizeof(PIXELFORMATDESCRIPTOR));

        pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
        pfd.nVersion = 1;
        pfd.dwFlags = dwFlags;
        pfd.iPixelType = PFD_TYPE_RGBA;
        pfd.cColorBits = conf.Bits;
        pfd.cDepthBits = conf.ZBits;
        pfd.iLayerType = PFD_MAIN_PLANE;

        //-------------------------------------------------------------------------------------------
        HWND temp_wnd = NULL;
        HDC temp_dc   = NULL;
        HGLRC temp_rc = NULL;
        //! Create a temp window with gl context(temp)
        temp_wnd = CreateWindowEx(0,"novaengine","window",0,0,0,0,0,0,0,0,0);
        if(!temp_wnd)
        {
            io::log::writeln(0,"Cant create temp window.");
            pFormat = ChoosePixelFormat(hDC,&pfd);
        }
        else
        {
        temp_dc = GetDC(temp_wnd);
        pFormat = ChoosePixelFormat(temp_dc,&pfd);
        if(!SetPixelFormat(temp_dc,pFormat,&pfd))
            io::log::writeln(0,"Unable to set PixelFormat.");
        //! create and enable opengl context
        temp_rc = wglCreateContext(temp_dc);
        }

        if(!temp_rc)
        {
            io::log::writeln(0,"Cannot create temp render context.");
            pFormat = ChoosePixelFormat(hDC,&pfd);
        }
        else
        {

            wglMakeCurrent(temp_dc,temp_rc);
            wglChoosePixelFormatARB = (PFNWGLCHOOSEPIXELFORMATARBPROC)wglGetProcAddress("wglChoosePixelFormatARB");
        }

        if (!wglChoosePixelFormatARB)
        {
            io::log::writeln(io::ELL_ERROR,"AntiAliasing not avalible...");
            pFormat = ChoosePixelFormat(hDC,&pfd);
        }
        else
        {
            bool valid;
            UINT numFormats;
            float fAttributes[] = {0,0};

            int iAttributes[] =
            {
                WGL_DRAW_TO_WINDOW_ARB,GL_TRUE,
                WGL_SUPPORT_OPENGL_ARB,GL_TRUE,
                WGL_ACCELERATION_ARB,WGL_FULL_ACCELERATION_ARB,
                WGL_COLOR_BITS_ARB,(conf.Bits==32) ? 24 : 16,
                WGL_ALPHA_BITS_ARB,(conf.Bits==32) ? 8  : 0,
                WGL_DEPTH_BITS_ARB,(int)conf.ZBits,
                WGL_STENCIL_BITS_ARB,0,
                WGL_DOUBLE_BUFFER_ARB, (conf.DoubleBuf) ? GL_TRUE : GL_FALSE,
                WGL_SAMPLE_BUFFERS_ARB,(conf.AntiAlias) ? GL_TRUE : GL_FALSE,
                WGL_SAMPLES_ARB,(int)conf.AntiAlias,
                0,0
            };
            valid = wglChoosePixelFormatARB(temp_dc,iAttributes,fAttributes,1,&pFormat,&numFormats);

            if ( !(valid && numFormats >= 1) )
            {
                pFormat = ChoosePixelFormat(hDC,&pfd);
            }
        }
        wglMakeCurrent(NULL, NULL);
        if(temp_rc)
            wglDeleteContext(temp_rc);
        if(temp_dc)
            ReleaseDC(temp_wnd, temp_dc);
        if(temp_wnd)
            DestroyWindow(temp_wnd);
        //-------------------------------------------------------------------------------------------

        if(!SetPixelFormat(hDC,pFormat,&pfd))
        {
            io::log::writeln(0,"Unable to set PixelFormat.");
            noerror = false;
        }
        //! create and enable opengl context
        hRC = wglCreateContext(hDC);
        if(!hRC)
        {
            io::log::writeln(0,"Cannot create render context.");
            noerror = false;
        }
        wglMakeCurrent(hDC,hRC);
        //VSync
        setVSync(conf.VSync);
    }
//    wglSwap
    initGL();
}
//--------------------------------------------------------------------------
CWin32OpenGLRendererWrapper::~CWin32OpenGLRendererWrapper()
{
    wglMakeCurrent(NULL, NULL);
    wglDeleteContext(hRC);
    ReleaseDC(hWnd, hDC);
}
//--------------------------------------------------------------------------
void CWin32OpenGLRendererWrapper::setVSync(bool enable)
{
    typedef BOOL (APIENTRY * WGLSWAPINTERVALEXTFUNC)(int);
    WGLSWAPINTERVALEXTFUNC wglSwapIntervalEXT = WGLSWAPINTERVALEXTFUNC(wglGetProcAddress("wglSwapIntervalEXT"));

    if(wglSwapIntervalEXT)
    {
        if(enable)
            wglSwapIntervalEXT(1);
        else
            wglSwapIntervalEXT(0);

        VSync = enable;
    }
    else
    {
        io::log::writeln(0,"VSync not supported. oO?.\n");
        io::log::writeln(0,"Get the nickel bro, and go buy a real computer..\n");
    }
}
//--------------------------------------------------------------------------
bool CWin32OpenGLRendererWrapper::getVSync()
{
    return VSync;
}
//--------------------------------------------------------------------------
void CWin32OpenGLRendererWrapper::end_frame()
{
    COpenGLRenderer::end_frame();
    SwapBuffers(hDC);
}

}
}
#endif // NE_WINDOW_WIN32
