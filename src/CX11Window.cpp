#include "CX11Window.h"

namespace novaengine
{
namespace window
{

CX11Window::CX11Window(SEngineConf conf,IEventManager* EventManager)
{
    LOG_ENGINE_DEBUG("CX11Window() begin\n");

    int snglBuf[] = {GLX_RGBA, GLX_DEPTH_SIZE, 16, None};
    int dblBuf[]  = {GLX_RGBA, GLX_DEPTH_SIZE, 16, GLX_DOUBLEBUFFER, None};
    GLboolean doubleBuffer = GL_TRUE;
    LOG_WARNING("%s%s\n","Hello world","!");
    dpy = XOpenDisplay(NULL);
        if (dpy == NULL)
        LOG_FATAL_ERROR("could not open display");

       /*** (3) find an appropriate visual ***/

    //vi = DefaultVisual(dpy,0);
    vi = glXChooseVisual(dpy, DefaultScreen(dpy), dblBuf);
      if (vi == NULL)
      {
        vi = glXChooseVisual(dpy, DefaultScreen(dpy), snglBuf);
        if (vi == NULL) LOG_FATAL_ERROR("no RGB visual with depth buffer");
        doubleBuffer = GL_FALSE;
      }

    /*** (4) create an OpenGL rendering context  ***/

      cx = glXCreateContext(dpy, vi, /* no shared dlists */ None,
                        /* direct rendering if possible */ GL_TRUE);
      if (cx == NULL)
        LOG_FATAL_ERROR("could not create rendering context");

        /*** (5) create an X window with the selected visual ***/

  /* create an X colormap since probably not using default visual */
  cmap = XCreateColormap(dpy, RootWindow(dpy, vi->screen), vi->visual, AllocNone);
  XSetWindowAttributes swa;
  swa.colormap = cmap;
  swa.border_pixel = 0;
  swa.event_mask = KeyPressMask    | ExposureMask
                 | ButtonPressMask | StructureNotifyMask;
  win = XCreateWindow(dpy, RootWindow(dpy, vi->screen), 0, 0,
                      300, 300, 0, vi->depth, InputOutput, vi->visual,
                      CWBorderPixel | CWColormap | CWEventMask, &swa);
  XSetStandardProperties(dpy, win, "main", "main", None,
                         NULL, NULL, NULL);

 XMapWindow(dpy,win);

    XEvent xevent;
    while(XPending(dpy))
    {
        XNextEvent(dpy,&xevent);

    }
    LOG_ENGINE_DEBUG("CX11Window() end\n");
}

CX11Window::~CX11Window()
{
    //dtor
}

void CX11Window::setResolution(core::dim2<u32>)
{
}
core::dim2<u32> CX11Window::getResolution()
{
}

void CX11Window::setFullscreenMode(bool fullscreen)
{
}
bool CX11Window::isFullscreenMode()
{
}

void CX11Window::setVisible(bool visible)
{
}
bool CX11Window::isVisible()
{
}

void CX11Window::setTittle(const char*)
{
}
const char* CX11Window::getTittle()
{
}

ICursorControl* CX11Window::getCursorControl()
{
}

bool CX11Window::update()
{
    return true;
}
bool CX11Window::isOk()
{
    return true;
}

void* CX11Window::getWindowInternalVariable(const char* name)
{
}

}

}
