#include "windowUI.h"
#include <X11/X.h>

window mainWin;

#ifdef __linux__

Atom EventDeleteWindow;

ERRORS initWindow(const char* windowClassName, void *hInstance) {
    mainWin.display = XOpenDisplay(NULL);
    mainWin.rootWin = XDefaultRootWindow(mainWin.display);
    int defaultScreen = XDefaultScreen(mainWin.display);
    mainWin.gc = XDefaultGC(mainWin.display, defaultScreen);
    return OK;
}

ERRORS createWindow(const char* name, long int dwstyle, int x, int y, int width, int height) {
    XSetWindowAttributes windowAttributes = {};
    windowAttributes.background_pixel = 0x1e1e2e;
    windowAttributes.event_mask = KeyPressMask | KeyReleaseMask | ExposureMask;
    int attributeValueMask = CWBackPixel | CWEventMask;
    mainWin.win = XCreateWindow(mainWin.display, mainWin.rootWin, x, y, width, height, 1, 
            CopyFromParent, CopyFromParent, CopyFromParent, attributeValueMask, &windowAttributes);
    XMapWindow(mainWin.display, mainWin.win);
    XStoreName(mainWin.display, mainWin.win, name);
    EventDeleteWindow = XInternAtom(mainWin.display, "WM_DELETE_WINDOW", 0);
    XSetWMProtocols(mainWin.display, mainWin.win, &EventDeleteWindow, 1);
    return OK;
}

void eventParsing(XEvent generalEvent, bool* isWindowOpen) {
    switch (generalEvent.type) {
        case ClientMessage: 
            {
                XClientMessageEvent* event = (XClientMessageEvent*) &generalEvent;
                if (event->data.l[0] == EventDeleteWindow) {
                    XDestroyWindow(mainWin.display, mainWin.win);
                    *isWindowOpen = false;
                }
            } break;
    }
}

int mainLoop() {
    bool isWindowOpen = true;
    XEvent event = {};
    while (isWindowOpen) {
        XSetForeground(mainWin.display, mainWin.gc, 0x00ff00);
        XFillRectangle(mainWin.display, mainWin.win, mainWin.gc, 100, 100, 100, 25);
        XNextEvent(mainWin.display, &event);
        eventParsing(event, &isWindowOpen);       
        XClearWindow(mainWin.display, mainWin.win);
    }
    return 0;
}

void addRectangle(int x, int y, int width, int height) {
    printf("Not implemented");
}

#else

#undef createWindow

LRESULT CALLBACK eventParsing(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
        case WM_CLOSE:
            DestroyWindow(hwnd);
            break;

        case WM_PAINT:
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);

            RECT testRect = {
                .top = 0,
                .left = 0,
                .bottom = 25,
                .right = 100
            };
            FillRect(hdc, &testRect, CreateSolidBrush(RGB(255, 0, 0)));

            EndPaint(hwnd, &ps);
            break;

        case WM_DESTROY:
            PostQuitMessage(0);
            break;

        default:
            return DefWindowProc(hwnd, msg, wParam, lParam);
    }
    return 0;
}

ERRORS initWindow(const char* windowClassName, void *hInstance) {
    mainWin.wc = (WNDCLASSEX){
        .cbSize = sizeof(WNDCLASSEX),
        .lpfnWndProc = eventParsing,
        .hInstance = hInstance,
        .hIcon = LoadIcon(NULL, IDI_APPLICATION),
        .hCursor = LoadCursor(NULL, IDC_ARROW),
        .hbrBackground = CreateSolidBrush(RGB(30, 30, 46)),
        .lpszMenuName = NULL,
        .lpszClassName = windowClassName,
        .hIconSm = LoadIcon(NULL, IDI_APPLICATION)
    };

    if (!RegisterClassEx(&mainWin.wc))
        return INIT_ERR;
    return OK;
}

ERRORS createWindow(const char* name, long int dwstyle, int x, int y, int width, int height) {
    HWND hwnd = CreateWindow(mainWin.wc.lpszClassName, name, dwstyle, 
        x, y, width, height, NULL, NULL, mainWin.wc.hInstance, NULL);
    if (hwnd == NULL)
        return INIT_ERR;
    mainWin.hwnd = hwnd;

    ShowWindow(hwnd, SW_SHOW);
    UpdateWindow(hwnd);

    return OK;
}

int mainLoop() {
    MSG msg;
    while (GetMessage(&msg, mainWin.hwnd, 0, 0) > 0) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return msg.wParam;
}

void addRectangle(int x, int y, int width, int height) {
    printf("Not implemented");
}
#endif

