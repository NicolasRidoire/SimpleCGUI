#include "SimpleCGUI.h"

static window mainWin;

#ifdef __linux__

// Window related
static Atom EventDeleteWindow;

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
        case Expose: 
            {
                DrawnObject* object = mainWin.firstDrawnObject;
                while (object != NULL) {
                    object->draw(object->params);
                    object = object->next;
                }
            } break;
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
        XNextEvent(mainWin.display, &event);
        eventParsing(event, &isWindowOpen);       
    }
    return 0;
}

// Drawing functions
void drawRectangle(DrawParams params) {
    RectParams rectParams = params.rectParams;
    XSetForeground(mainWin.display, mainWin.gc, rectParams.color);
    XFillRectangle(mainWin.display, mainWin.win, mainWin.gc, rectParams.x, 
            rectParams.y, rectParams.width, rectParams.height);
}

#else

// Window related
#undef createWindow

LRESULT CALLBACK eventParsing(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
        case WM_CLOSE:
            DestroyWindow(hwnd);
            break;

        case WM_PAINT:
            BeginPaint(hwnd, &mainWin.ps);
            DrawnObject* object = mainWin.firstDrawnObject;
            while (object != NULL) {
                object->draw(object->params);
                object = object->next;
            }
            EndPaint(hwnd, &mainWin.ps);
            UpdateWindow(mainWin.hwnd);
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

// Drawing functions
void drawRectangle(DrawParams params) {
    RectParams rectParams = params.rectParams;
    RECT rect = {
        .top = rectParams.y,
        .left = rectParams.x,
        .bottom = rectParams.y + rectParams.height,
        .right = rectParams.x + rectParams.width
    };
    Color rgb = fromHexToColor(rectParams.color);
    if (FillRect(mainWin.ps.hdc, &rect, CreateSolidBrush(RGB(rgb.red, rgb.green, rgb.blue))) == 0)
        printf("Failed to draw the rectangle");
}

#endif

// Callbacks
DrawnObject* getLastDrawnObject() {
    DrawnObject* lastObject = mainWin.firstDrawnObject;
    if (lastObject == NULL) 
        return NULL;
    while (lastObject->next != NULL) {
        lastObject = lastObject->next;
    }
    return lastObject;
}

DrawnObject* registerDrawCallback(void (*callback)(DrawParams), DrawParams params) {
    DrawnObject* object = (DrawnObject*)malloc(sizeof(DrawnObject));
    object->draw = callback;
    object->params = params;
    object->previous = NULL;
    object->next = NULL;
    DrawnObject* lastObject = getLastDrawnObject();
    if (lastObject == NULL)
        mainWin.firstDrawnObject = object;
    else {
        object->previous = lastObject;
        lastObject->next = object;
    }
    return object;
}

void unregisterDrawCallback(DrawnObject* object) {
    if (object == NULL)
        return;
    DrawnObject* previous = object->previous;
    DrawnObject* next = object->next;
    if (object == mainWin.firstDrawnObject) {
        mainWin.firstDrawnObject = next;
        return;
    }
    if (next == NULL)
        previous->next = NULL;
    next->previous = previous;
    free(object);
}

// Drawing API
DrawnObject* addRectangle(int x, int y, int width, int height, Color rgb) {
    DrawParams params = {
        .rectParams = {
            .color = fromColorToHex(rgb),
            .x = x,
            .y = y,
            .width = width,
            .height = height
        }
    };
    return registerDrawCallback(&drawRectangle, params);
}

void deleteDraw(DrawnObject* object) {
    unregisterDrawCallback(object);  
}

void deleteRectangle(DrawnObject* rect) {
    deleteDraw(rect);
}

// Utilities
Color fromHexToColor(unsigned long int rgb) {
    Color converted = {
        .red = (rgb >> 16) & 0xff,
        .green = (rgb >> 8) & 0xff,
        .blue = rgb & 0xff
    };
    return converted;
}

unsigned long int fromColorToHex(Color rgb) {
    return (rgb.red << 16) + (rgb.green << 8) + rgb.blue;
}
