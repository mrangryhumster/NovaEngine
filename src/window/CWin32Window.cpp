#ifdef NE_WINDOW_WIN32
#include "CWin32Window.h"
#include "log\NLogger.h"
#include <map>

namespace novaengine
{
namespace window
{

//! Special functions and classes for win32window
namespace win32
{
//! Helpfull class to map HWND -> CWin32Window* (designed for multiwindow)
//! I think using only std::map, instead class with private std::map, better idea.
class Win32WindowMap
{
public:
    Win32WindowMap()
    {
        WindowMap.clear();
    }
    CWin32Window* getWindow(HWND hwnd)
    {
        return (WindowMap.find(hwnd)==WindowMap.end())?NULL:WindowMap.find(hwnd)->second;
    }
    void addWindow(HWND hwnd,CWin32Window* cwin32window)
    {
        WindowMap[hwnd] = cwin32window;
    }
    void removeWindow(HWND hwnd)
    {
        WindowMap.erase(hwnd);
    }
    u32 getMappedWindowCount()
    {
        return WindowMap.size();
    }

private:
    std::map<HWND,CWin32Window*> WindowMap;
};
//! Because we get all events from all windows attached to one WndClass
//! and we can have more than one active window(in future)
//! in Win32_WndProc and have only HWND of event sender.
//! we must redirect event from A Window to class that control A Window.
//! we used Win32WindowMap for mapping HWND -> CWin32Window*
static Win32WindowMap WindowMap;
//!This array need to avoid to auto clicks from keyboard
//! example: if you hold key "a" on keyboard wndproc will receive more than one click
//! this will be repeating while you hold "a"
static bool KeyPressed[256];
//!win32::KeyMap store map from Win32 keycodes to NovaEngine keycodes
static CKeyMap<int,E_KEY_CODE> KeyMap;
//!Standart Win32 event processing...
LRESULT CALLBACK Win32_WndProc(HWND hWnd, UINT Msg , WPARAM wParam, LPARAM lParam)
{
    //------------------------------------------------------
    CWin32Window* window = win32::WindowMap.getWindow(hWnd);
    if(window == NULL)
        return DefWindowProc(hWnd, Msg, wParam, lParam);
    //------------------------------------------------------
    SEvent event;

    switch (Msg)
    {
    case WM_CLOSE:
        PostQuitMessage(0);
        break;

    case WM_DESTROY:
        return 0;

    case WM_LBUTTONDOWN:
    case WM_LBUTTONUP:
    case WM_RBUTTONDOWN:
    case WM_RBUTTONUP:
    case WM_MBUTTONDOWN:
    case WM_MBUTTONUP:
    case WM_MOUSEMOVE:
    case WM_MOUSEWHEEL:
    {
        event.event_type = EET_POINTER_EVENT;
        event.pointer.event_type = EPET_MOVE;

        event.pointer.x = (short)LOWORD(lParam);
        event.pointer.y = (short)HIWORD(lParam);

        switch(Msg)
        {
        case WM_LBUTTONDOWN:
        case WM_RBUTTONDOWN:
        case WM_MBUTTONDOWN:
            event.pointer.event_type = EPET_BUTTON;
            event.pointer.key_state  = EKS_DOWN;
            break;

        case WM_LBUTTONUP:
        case WM_RBUTTONUP:
        case WM_MBUTTONUP:
            event.pointer.event_type = EPET_BUTTON;
            event.pointer.key_state  = EKS_UP;
            break;

        case WM_MOUSEWHEEL:
            event.pointer.event_type = EPET_WHELL;
            event.pointer.keycode = KEY_MOUSE_WHELL;
            //! get whell spin direction
            short whell_dir = (short)HIWORD(wParam);
            event.pointer.key_state = (whell_dir > 0) ? EKS_DOWN : EKS_UP;

            //! fix pointer coords from relative_screen to relative_window
            POINT rp;
            rp.x = 0;
            rp.y = 0;
            ClientToScreen(hWnd, &rp);

            event.pointer.x -= rp.x;
            event.pointer.y -= rp.y;
            break;

        }

        if(Msg == WM_LBUTTONDOWN || Msg == WM_LBUTTONUP)
            event.pointer.keycode = KEY_MOUSE_LEFT;
        if(Msg == WM_RBUTTONDOWN || Msg == WM_RBUTTONUP)
            event.pointer.keycode = KEY_MOUSE_RIGHT;
        if(Msg == WM_MBUTTONDOWN || Msg == WM_MBUTTONUP)
            event.pointer.keycode = KEY_MOUSE_MIDDLE;


        window->pushEvent(event);
    }
    break;

    case WM_SYSKEYDOWN:
    case WM_SYSKEYUP:
    case WM_KEYUP:
    case WM_KEYDOWN:
    {
        if(KeyPressed[wParam] != (Msg == WM_KEYDOWN || Msg == WM_SYSKEYDOWN))
        {
            KeyPressed[wParam] = (Msg == WM_KEYDOWN || Msg == WM_SYSKEYDOWN);

            event.event_type = EET_KEYBOARD_EVENT;
            event.keyboard.keycode          = KeyMap.get(wParam);
            event.keyboard.native_keycode   = wParam;
            event.keyboard.key_state        = (Msg == WM_KEYDOWN || Msg == WM_SYSKEYDOWN)?EKS_DOWN:EKS_UP;

            event.keyboard.shift = ((KeyPressed[VK_SHIFT]  )!=0);
            event.keyboard.ctrl  = ((KeyPressed[VK_CONTROL])!=0);
            event.keyboard.alt   = ((KeyPressed[VK_MENU]   )!=0);

            //! i fix it in future... now i dont know good way to handle unicode sym from keydown
            // TODO (Gosha#1#): Add unicode from keydown conversion...
            event.keyboard.unicode = L'?';

            window->pushEvent(event);
        }
    }
    break;

    default:
        return DefWindowProc(hWnd, Msg, wParam, lParam);
    }

    return 0;
}

}
//------------------------------------------------

CWin32Window::CWin32Window(SEngineConf conf,IEventListener* event_proc):
    hWnd(nullptr),
    hInstance(nullptr),
    title(nullptr),
    noerror(true),
    exit(false),
    EventHandler(event_proc)
{
    setObjectName("CWin32Window");
    LOG_ENGINE_DEBUG("CWin32Window() begin\n");
    hInstance = GetModuleHandle(0);

    //!If ExternalWindowPointer == nullptr than we create own windowclass and window
    //!else connecting to another window
    if(conf.ExternalWindowPointer == nullptr)
    {
        if(win32::WindowMap.getMappedWindowCount() == 0)
        {
            IsExternalWindow = false;

            //! Window class
            WNDCLASSEX wcex;

            wcex.cbSize         = sizeof(WNDCLASSEX);
            wcex.style          = CS_OWNDC;
            wcex.lpfnWndProc    = win32::Win32_WndProc;
            wcex.cbClsExtra     = 0;
            wcex.cbWndExtra     = 0;
            wcex.hInstance      = hInstance;
            wcex.hIcon          = LoadIcon(nullptr, IDI_APPLICATION);
            wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
            wcex.hbrBackground  = (HBRUSH)GetStockObject(BLACK_BRUSH);
            wcex.lpszMenuName   = nullptr;
            wcex.lpszClassName  = "novaengine";
            wcex.hIconSm        = LoadIcon(nullptr, IDI_APPLICATION);

            //!Try Register wndclass
            if(!RegisterClassEx(&wcex))
            {
                u32 i = GetLastError();
                LOG_FATAL_ERROR("Cant register window class [err:%d]\n",i);
                noerror = false;
                return;
            }
        }
        //! If no error we continue init and create window

        DWORD style = WS_POPUP;

        //!default window style(no fullscreen)
        if(!conf.FullScreen)
            style = WS_SYSMENU      |
                    WS_BORDER       |
                    WS_CAPTION      |
                    WS_CLIPCHILDREN |
                    WS_CLIPSIBLINGS ;

        //!Create window
        hWnd = CreateWindowEx(0,
                              "novaengine",
                              "window",
                              style,
                              CW_USEDEFAULT,
                              CW_USEDEFAULT,
                              conf.WindowSize.width,
                              conf.WindowSize.height,
                              NULL,
                              NULL,
                              hInstance,
                              NULL);

        //!show error if fail
        if(!hWnd)
        {
            u32 i = GetLastError();
            LOG_FATAL_ERROR("Cant create window [err:%d]\n",i);
            noerror = false;
            return;
        }

        //!register window in wndmap...
        win32::WindowMap.addWindow(hWnd,this);

        setVisible(!conf.StartWindowHidden);
        /*
                    if(IsFullscreenWindow)
                        setFullscreenMode(true);
        */
        UpdateWindow(hWnd);

    }


    IsFullscreenWindow = conf.FullScreen;
    loadKeyMap();
    CursorControl.hWnd = hWnd;
    //-----------------------------------
    LOG_ENGINE_DEBUG("CWin32Window() end\n");
}
//-------------------------------------------------------------------------------------------
CWin32Window::~CWin32Window()
{
    if(title)
        delete[] title;

    if(not IsExternalWindow)
    {
        DestroyWindow(hWnd);
        win32::WindowMap.removeWindow(hWnd);

        if(win32::WindowMap.getMappedWindowCount() > 0)
        {
            LOG_FATAL_ERROR("Cant release window class...(%d atached window still alive)\n",win32::WindowMap.getMappedWindowCount());
        }
        else
        {
            if(!UnregisterClass("novaengine",hInstance))
            {
                LOG_FATAL_ERROR("Cannot release window class\n");
            }
            else
            {
                win32::KeyMap.clear();
                LOG_ENGINE_DEBUG("Window class goodbye 0/~\n");
            }
        }
    }
}
//-------------------------------------------------------------------------------------------
void CWin32Window::setResolution(core::dim2<u32> WinSize)
{
    SetWindowPos(hWnd,HWND_TOP,0,0,WinSize.width,WinSize.height,0);
}
//-------------------------------------------------------------------------------------------
core::dim2<u32>  CWin32Window::getResolution()
{
    RECT WindowRect;
    GetClientRect(hWnd,&WindowRect);
    return core::dim2<u32>(WindowRect.right - WindowRect.left,WindowRect.bottom - WindowRect.top);
}
//-------------------------------------------------------------------------------------------
void CWin32Window::setFullscreenMode(bool setfullscreen)
{
    if(!IsFullscreenWindow && setfullscreen)
    {
        NoFullScreenWindowStyle   = GetWindowLong(hWnd,GWL_STYLE);
        NoFullScreenWindowStyleEx = GetWindowLong(hWnd,GWL_EXSTYLE);
    }

    if(setfullscreen)
    {
        if(!IsFullscreenWindow)
        {
            setResolution(core::dim2<u32>(GetSystemMetrics(SM_CXSCREEN),GetSystemMetrics(SM_CYSCREEN)));

            SetWindowLong(
                hWnd,
                GWL_STYLE,
                NoFullScreenWindowStyle & ~(WS_CAPTION | WS_THICKFRAME)
            );
            SetWindowLong(
                hWnd,
                GWL_EXSTYLE,
                NoFullScreenWindowStyleEx & ~(
                    WS_EX_DLGMODALFRAME |
                    WS_EX_WINDOWEDGE    |
                    WS_EX_CLIENTEDGE    |
                    WS_EX_STATICEDGE)
            );
            IsFullscreenWindow = true;
            LOG_INFO("Fullscreen enabled.\n");
        }
    }
    else
    {
        if(IsFullscreenWindow)
        {
            SetWindowLong(hWnd,GWL_STYLE,NoFullScreenWindowStyle);
            SetWindowLong(hWnd,GWL_EXSTYLE,NoFullScreenWindowStyleEx);
            IsFullscreenWindow = false;
        }
    }
}
//-------------------------------------------------------------------------------------------
bool CWin32Window::isFullscreenMode()
{
    return IsFullscreenWindow;
}
//-------------------------------------------------------------------------------------------
void CWin32Window::setVisible(bool visible)
{
    if(visible)
        ShowWindow(hWnd,SW_SHOW);
    else
        ShowWindow(hWnd,SW_HIDE);
    IsVisible = visible;
}
//-------------------------------------------------------------------------------------------
bool CWin32Window::isVisible()
{
    return IsVisible;
}
//-------------------------------------------------------------------------------------------
void CWin32Window::setCaption(const wchar_t* caption)
{
    SetWindowTextW(hWnd,caption);
}
//-------------------------------------------------------------------------------------------
const wchar_t*  CWin32Window::getCaption()
{
//! Hate windows... >.<
//! I mean you must delete returned pointer if you done work with it...
    // TODO (Gosha#1#): Fix this trash
    if(title)
        delete[] title;

    title = new wchar_t[GetWindowTextLengthW(hWnd)];
    GetWindowTextW(hWnd,title,GetWindowTextLengthW(hWnd));
    return title;
}
//-------------------------------------------------------------------------------------------
s32  CWin32Window::getWindowType()
{
    return 0;
}
//-------------------------------------------------------------------------------------------
int  CWin32Window::getWindowID()
{
    return (int)hWnd;
}
//-------------------------------------------------------------------------------------------
bool CWin32Window::update()
{
    pullEvents();
    return isOk();
}
//-------------------------------------------------------------------------------------------
bool CWin32Window::isOk()
{
    return (noerror && !exit);
}
//-------------------------------------------------------------------------------------------
void CWin32Window::pullEvents()
{
    MSG msg;
    while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
    {
        if (msg.message == WM_QUIT)
        {
            exit = true;
        }
        else
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }

    }
}
//-------------------------------------------------------------------------------------------
void CWin32Window::pushEvent(SEvent event)
{
    if(EventHandler)
        EventHandler->OnEvent(event);

    if(event.event_type == EET_POINTER_EVENT && event.pointer.event_type == EPET_MOVE)
        CursorControl.LastPosition.set(event.pointer.x,event.pointer.y);
}
//-------------------------------------------------------------------------------------------
void CWin32Window::loadKeyMap()
{
    win32::KeyMap.add(0,novaengine::KEY_UNKNOW);
    //---------------------------------------
    win32::KeyMap.add(VK_UP,novaengine::KEY_UP);
    win32::KeyMap.add(VK_DOWN,novaengine::KEY_DOWN);
    win32::KeyMap.add(VK_LEFT,novaengine::KEY_LEFT);
    win32::KeyMap.add(VK_RIGHT,novaengine::KEY_RIGHT);
    //---------------------------------------
    win32::KeyMap.add(VK_SHIFT,novaengine::KEY_SHIFT);
    win32::KeyMap.add(VK_CONTROL,novaengine::KEY_CTRL);
    win32::KeyMap.add(VK_MENU,novaengine::KEY_ALT);

    win32::KeyMap.add(0x30,novaengine::KEY_KEY_0);
    win32::KeyMap.add(0x31,novaengine::KEY_KEY_1);
    win32::KeyMap.add(0x32,novaengine::KEY_KEY_2);
    win32::KeyMap.add(0x33,novaengine::KEY_KEY_3);
    win32::KeyMap.add(0x34,novaengine::KEY_KEY_4);
    win32::KeyMap.add(0x35,novaengine::KEY_KEY_5);
    win32::KeyMap.add(0x36,novaengine::KEY_KEY_6);
    win32::KeyMap.add(0x37,novaengine::KEY_KEY_7);
    win32::KeyMap.add(0x38,novaengine::KEY_KEY_8);
    win32::KeyMap.add(0x39,novaengine::KEY_KEY_9);
    //--------------------------------------
    win32::KeyMap.add(0x41,novaengine::KEY_KEY_A);
    win32::KeyMap.add(0x42,novaengine::KEY_KEY_B);
    win32::KeyMap.add(0x43,novaengine::KEY_KEY_C);
    win32::KeyMap.add(0x44,novaengine::KEY_KEY_D);
    win32::KeyMap.add(0x45,novaengine::KEY_KEY_E);
    win32::KeyMap.add(0x46,novaengine::KEY_KEY_F);
    win32::KeyMap.add(0x47,novaengine::KEY_KEY_G);
    win32::KeyMap.add(0x48,novaengine::KEY_KEY_H);
    win32::KeyMap.add(0x49,novaengine::KEY_KEY_I);
    win32::KeyMap.add(0x4a,novaengine::KEY_KEY_J);
    win32::KeyMap.add(0x4b,novaengine::KEY_KEY_K);
    win32::KeyMap.add(0x4c,novaengine::KEY_KEY_L);
    win32::KeyMap.add(0x4d,novaengine::KEY_KEY_M);
    win32::KeyMap.add(0x4e,novaengine::KEY_KEY_N);
    win32::KeyMap.add(0x4f,novaengine::KEY_KEY_O);
    win32::KeyMap.add(0x50,novaengine::KEY_KEY_P);
    win32::KeyMap.add(0x51,novaengine::KEY_KEY_Q);
    win32::KeyMap.add(0x52,novaengine::KEY_KEY_R);
    win32::KeyMap.add(0x53,novaengine::KEY_KEY_S);
    win32::KeyMap.add(0x54,novaengine::KEY_KEY_T);
    win32::KeyMap.add(0x55,novaengine::KEY_KEY_U);
    win32::KeyMap.add(0x56,novaengine::KEY_KEY_V);
    win32::KeyMap.add(0x57,novaengine::KEY_KEY_W);
    win32::KeyMap.add(0x58,novaengine::KEY_KEY_X);
    win32::KeyMap.add(0x59,novaengine::KEY_KEY_Y);
    win32::KeyMap.add(0x5a,novaengine::KEY_KEY_Z);
    //--------------------------------------
    win32::KeyMap.add(VK_BACK      ,novaengine::KEY_BACKSPACE  );
    win32::KeyMap.add(VK_TAB       ,novaengine::KEY_TAB        );
    win32::KeyMap.add(VK_RETURN    ,novaengine::KEY_RETURN     );
    win32::KeyMap.add(VK_CAPITAL   ,novaengine::KEY_CAPSLOCK   );
    win32::KeyMap.add(VK_ESCAPE    ,novaengine::KEY_ESCAPE     );
    win32::KeyMap.add(VK_SPACE     ,novaengine::KEY_SPACE      );
    win32::KeyMap.add(VK_PRIOR     ,novaengine::KEY_PAGEUP     );
    win32::KeyMap.add(VK_NEXT      ,novaengine::KEY_PAGEDOWN   );
    win32::KeyMap.add(VK_END       ,novaengine::KEY_END        );
    win32::KeyMap.add(VK_HOME      ,novaengine::KEY_HOME       );
    win32::KeyMap.add(VK_PRINT     ,novaengine::KEY_PRINTSCREEN);
    win32::KeyMap.add(VK_INSERT    ,novaengine::KEY_INSERT     );
    win32::KeyMap.add(VK_DELETE    ,novaengine::KEY_DELETE     );
    win32::KeyMap.add(VK_LSHIFT    ,novaengine::KEY_LSHIFT     );
    win32::KeyMap.add(VK_RSHIFT    ,novaengine::KEY_RSHIFT     );
    win32::KeyMap.add(VK_LCONTROL  ,novaengine::KEY_LCTRL      );
    win32::KeyMap.add(VK_RCONTROL  ,novaengine::KEY_RCTRL      );
    win32::KeyMap.add(VK_SNAPSHOT  ,novaengine::KEY_PRINTSCREEN);
    win32::KeyMap.add(VK_PAUSE     ,novaengine::KEY_PAUSE      );

    win32::KeyMap.add(VK_F1,novaengine::KEY_F1);
    win32::KeyMap.add(VK_F2,novaengine::KEY_F2);
    win32::KeyMap.add(VK_F3,novaengine::KEY_F3);
    win32::KeyMap.add(VK_F4,novaengine::KEY_F4);
    win32::KeyMap.add(VK_F5,novaengine::KEY_F5);
    win32::KeyMap.add(VK_F6,novaengine::KEY_F6);
    win32::KeyMap.add(VK_F7,novaengine::KEY_F7);
    win32::KeyMap.add(VK_F8,novaengine::KEY_F8);
    win32::KeyMap.add(VK_F9,novaengine::KEY_F9);
    win32::KeyMap.add(VK_F10,novaengine::KEY_F10);
    win32::KeyMap.add(VK_F11,novaengine::KEY_F11);
    win32::KeyMap.add(VK_F12,novaengine::KEY_F12);
    win32::KeyMap.add(VK_F13,novaengine::KEY_F13);
    win32::KeyMap.add(VK_F14,novaengine::KEY_F14);
    win32::KeyMap.add(VK_F15,novaengine::KEY_F15);
    win32::KeyMap.add(VK_NUMLOCK,novaengine::KEY_NUMLOCK);
    win32::KeyMap.add(VK_NUMPAD0,novaengine::KEY_NUM_0);
    win32::KeyMap.add(VK_NUMPAD1,novaengine::KEY_NUM_1);
    win32::KeyMap.add(VK_NUMPAD2,novaengine::KEY_NUM_2);
    win32::KeyMap.add(VK_NUMPAD3,novaengine::KEY_NUM_3);
    win32::KeyMap.add(VK_NUMPAD4,novaengine::KEY_NUM_4);
    win32::KeyMap.add(VK_NUMPAD5,novaengine::KEY_NUM_5);
    win32::KeyMap.add(VK_NUMPAD6,novaengine::KEY_NUM_6);
    win32::KeyMap.add(VK_NUMPAD7,novaengine::KEY_NUM_7);
    win32::KeyMap.add(VK_NUMPAD8,novaengine::KEY_NUM_8);
    win32::KeyMap.add(VK_NUMPAD9,novaengine::KEY_NUM_9);
    win32::KeyMap.add(VK_MULTIPLY,novaengine::KEY_MULTIPLY);
    win32::KeyMap.add(VK_ADD,novaengine::KEY_PLUS);
    win32::KeyMap.add(VK_SUBTRACT,novaengine::KEY_MINUS);
    win32::KeyMap.add(VK_DECIMAL,novaengine::KEY_DECIMAL);
    win32::KeyMap.add(VK_DIVIDE,novaengine::KEY_DIVIDE);
}
//-------------------------------------------------------------------------------------------
void* CWin32Window::getWindowInternalVariable(const char* name)
{
    if(strcmp(name,"HWND") == 0)
        return &hWnd;
    if(strcmp(name,"HINSTANCE") == 0)
        return &hInstance;

    LOG_ERROR("Request for unknow varible...[%s]\n",name);
    return NULL;
}
//-------------------------------------------------------------------------------------------
}
}
#endif
