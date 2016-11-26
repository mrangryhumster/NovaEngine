#include "CX11Window.h"

namespace novaengine
{
namespace window
{

CX11Window::CX11Window(SEngineConf conf,IEventManager* p_EventManager):
    EventManager(p_EventManager)
{
    LOG_ENGINE_DEBUG("CX11Window() begin\n");

    int snglBuf[] = {GLX_RGBA, GLX_DEPTH_SIZE, 16, None};//Also can use conf.ZBits instead 16
    int dblBuf[]  = {GLX_RGBA, GLX_DEPTH_SIZE, 16, GLX_DOUBLEBUFFER, None};
    GLboolean doubleBuffer = GL_TRUE;// IDK
    LOG_WARNING("Hello world!\n");
    dpy = XOpenDisplay(NULL);//Opening display by XServer... I guess... https://tronche.com/gui/x/xlib/
    if (dpy == NULL)
        LOG_FATAL_ERROR("Could not open display");

    ///Find an appropriate visual

    //vi = DefaultVisual(dpy,0);
    vi = glXChooseVisual(dpy, DefaultScreen(dpy), dblBuf);
    if (vi == NULL)
    {
        vi = glXChooseVisual(dpy, DefaultScreen(dpy), snglBuf);
        if (vi == NULL) LOG_FATAL_ERROR("no RGB visual with depth buffer");
        doubleBuffer = GL_FALSE;
    }

    ///Create an OpenGL rendering context

    cx = glXCreateContext(dpy, vi, /* no shared dlists */ None,
                          /* direct rendering if possible */ GL_TRUE);
    if (cx == NULL)
        LOG_FATAL_ERROR("could not create rendering context");

    ///Create an X window with the selected visual

//Create an X colormap since probably not using default visual
    cmap = XCreateColormap(dpy, RootWindow(dpy, vi->screen), vi->visual, AllocNone);
    XSetWindowAttributes swa;
    swa.colormap = cmap;
    swa.border_pixel = 0;
    swa.background_pixel = XWhitePixel(dpy, 0);
    swa.event_mask = KeyPressMask | KeyReleaseMask | ExposureMask | ButtonPressMask | ButtonReleaseMask | StructureNotifyMask | PointerMotionMask;

    win = XCreateWindow(dpy, RootWindow(dpy, vi->screen), 0, 0,           //XCreateWindow(display, parent, x, y,
                        conf.WindowSize.width, conf.WindowSize.height, 0, vi->depth, InputOutput, vi->visual,  //width, height, border_width, depth, class, visual,
                        CWBorderPixel | CWColormap | CWEventMask, &swa);  //valuemask, attributes)

    XSetStandardProperties(dpy, win, "Widnow name is HERE", "Icon name is HERE", None,
                           NULL, NULL, NULL);


    ///Context

    glXMakeCurrent(dpy,win,cx);
//    glXSwapIntervalEXT(false);

    //XFlush(dpy);
    //sleep(1);
    if (conf.FullScreen)
        setFullscreenMode(true);
    else XMapWindow(dpy,win);
    //while(true)
    //{
    //    XFlush(dpy);
    //XPending(dpy);
    //XNextEvent(dpy,&xevent);
    //}
    loadKeymap();
    LOG_ENGINE_DEBUG("CX11Window() end\n");
}

CX11Window::~CX11Window()//Destructor
{
    //dtor
}

void CX11Window::setResolution(core::dim2<u32>)
{

}

core::dim2<u32> CX11Window::getResolution()
{
    return core::dim2<u32>(1920/2,1080/2);
}

void CX11Window::setFullscreenMode(bool fullscreen_d)
{
    ///Fullscreen realization

    LOG_WARNING("Starting fullscreen!\n");
    Atom wm_state = XInternAtom(dpy, "_NET_WM_STATE", False);
    Atom fullscreen = XInternAtom(dpy, "_NET_WM_STATE_FULLSCREEN", False);
    XEvent xevent;
    memset(&xevent, 0, sizeof(xevent));
    xevent.type = ClientMessage;
    xevent.xclient.window = win;
    xevent.xclient.message_type = wm_state;
    xevent.xclient.format = 32;
    xevent.xclient.data.l[0] = 1;
    xevent.xclient.data.l[1] = fullscreen;
    xevent.xclient.data.l[2] = 0;
    xevent.xclient.data.l[2] = 0;
    XSendEvent (dpy, DefaultRootWindow(dpy), False,
                SubstructureRedirectMask | SubstructureNotifyMask, &xevent);
    XMapWindow(dpy,win);

//XFlush(dpy);
//XPending(dpy);
//XNextEvent(dpy,&xevent);

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
    poolEvents();
    return true;
}
bool CX11Window::isOk()
{
    return true;
}
bool CX11Window::poolEvents()
{
    SEvent EngineEvent;
    XEvent event;//Структура описывающая события
    while(XPending(dpy))//Вощвращает число событий в очереди у Х определенного сервера
    {
        XNextEvent(dpy,&event);//Выгружает событие из очереди в структуру
        switch(event.type)
        {

        case KeyPress:
        case KeyRelease:
        {
            LOG_ENGINE_DEBUG("%d\n",event.xkey.keycode);

            EngineEvent.event_type = EET_KEYBOARD_EVENT;
            EngineEvent.keyboard.keycode   = KeyMap[event.xkey.keycode];
            EngineEvent.keyboard.key_state = (event.type == KeyPress) ? EKS_DOWN : EKS_UP;
            EventManager->pushEvent(EngineEvent);
            continue;
        }
        break;
        case ButtonPress:
        case ButtonRelease:
        {
            EngineEvent.event_type = EET_POINTER_EVENT;
            EngineEvent.pointer.event_type = EPET_BUTTON;
            EngineEvent.pointer.x = event.xbutton.x;
            EngineEvent.pointer.y = event.xbutton.y;
            EngineEvent.pointer.key_state = (event.type == ButtonPress) ? EKS_DOWN : EKS_UP;
            if(event.xbutton.button == 1)
                EngineEvent.pointer.keycode = KEY_MOUSE_LEFT;
            else if(event.xbutton.button == 2)
                EngineEvent.pointer.keycode = KEY_MOUSE_MIDDLE;
            else if(event.xbutton.button == 3)
                EngineEvent.pointer.keycode = KEY_MOUSE_RIGHT;
            else if(event.xbutton.button == 4 || event.xbutton.button == 5)
            {
                EngineEvent.pointer.event_type = EPET_WHELL;
                EngineEvent.pointer.keycode = KEY_MOUSE_WHELL;
                EngineEvent.pointer.key_state = (event.xbutton.button == 4) ? EKS_UP : EKS_DOWN;
            }

            EventManager->pushEvent(EngineEvent);
        }
        break;
        case MotionNotify:
        {
            EngineEvent.event_type = EET_POINTER_EVENT;
            EngineEvent.pointer.event_type = EPET_MOVE;
            EngineEvent.pointer.x = event.xmotion.x;
            EngineEvent.pointer.y = event.xmotion.y;
            EventManager->pushEvent(EngineEvent);
            continue;
        }
        break;

        default:
            break;
        }
    }
    return true;
}
void CX11Window::loadKeymap()
{
    KeyMap.insert(std::pair<int,E_KEY_CODE>(0,novaengine::KEY_UNKNOW));
    KeyMap.insert(std::pair<int,E_KEY_CODE>(9,novaengine::KEY_ESCAPE));
    KeyMap.insert(std::pair<int,E_KEY_CODE>(50,novaengine::KEY_SHIFT));
    KeyMap.insert(std::pair<int,E_KEY_CODE>(67,novaengine::KEY_F1));
    KeyMap.insert(std::pair<int,E_KEY_CODE>(68,novaengine::KEY_F2));
    KeyMap.insert(std::pair<int,E_KEY_CODE>(69,novaengine::KEY_F3));
    KeyMap.insert(std::pair<int,E_KEY_CODE>(70,novaengine::KEY_F4));
    KeyMap.insert(std::pair<int,E_KEY_CODE>(25,novaengine::KEY_KEY_W));
    KeyMap.insert(std::pair<int,E_KEY_CODE>(38,novaengine::KEY_KEY_A));
    KeyMap.insert(std::pair<int,E_KEY_CODE>(39,novaengine::KEY_KEY_S));
    KeyMap.insert(std::pair<int,E_KEY_CODE>(40,novaengine::KEY_KEY_D));
}
void* CX11Window::getWindowInternalVariable(const char* name)
{
    if(strcmp("xWindow",name) == 0)
        return &win;
    else if(strcmp("xDisplay",name) == 0)
        return (void*)dpy;
    else if(strcmp("swap",name) == 0)
        glXSwapBuffers(dpy,win);
    else
        return nullptr;

}

}

}
