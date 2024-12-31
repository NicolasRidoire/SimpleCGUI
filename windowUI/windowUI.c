#include "windowUI.h"

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
                DrawCallback* callback = mainWin.firstDraw;
                while (callback != NULL) {
                    callback->draw(callback->params);
                    callback = callback->next;
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

// Callbacks
DrawCallback* getLastDrawCallback() {
    DrawCallback* lastCallback = mainWin.firstDraw;
    if (lastCallback == NULL) 
        return NULL;
    while (lastCallback->next != NULL) {
        lastCallback = lastCallback->next;
    }
    return lastCallback;
}

DrawCallback* getDrawCallbackById(int id) {
    DrawCallback* callback = mainWin.firstDraw;
    if (callback == NULL) {
        printf("No drawCallback initialized in the window structure");
        return NULL;
    }
    while (callback->id != id && callback->next != NULL) {
        callback = callback->next;
    }
    if (callback == NULL)
        printf("No drawCallback with this specific id");
    return callback;
}

int registerDrawCallback(void (*callback)(DrawParams), DrawParams params) {
    DrawCallback* drawCallback = (DrawCallback*)malloc(sizeof(DrawCallback));
    drawCallback->draw = callback;
    drawCallback->params = params;
    drawCallback->previous = NULL;
    drawCallback->next = NULL;
    drawCallback->id = 0;
    DrawCallback* lastCallback = getLastDrawCallback();
    if (lastCallback == NULL)
        mainWin.firstDraw = drawCallback;
    else {
        drawCallback->id = lastCallback->id+1;
        drawCallback->previous = lastCallback;
        lastCallback->next = drawCallback;
    }
    return drawCallback->id;
}

void unregisterDrawCallback(int id) {
    DrawCallback* callback = getDrawCallbackById(id);
    if (callback == NULL)
        return;
    DrawCallback* previous = callback->previous;
    DrawCallback* next = callback->next;
    if (callback == mainWin.firstDraw) {
        mainWin.firstDraw = next;
        return;
    }
    if (next == NULL)
        previous->next = NULL;
    next->previous = previous;
    free(callback);
}

// Drawing functions
void drawRectangle(DrawParams params) {
    RectParams rectParams = params.rectParams;
    XSetForeground(mainWin.display, mainWin.gc, rectParams.color);
    XFillRectangle(mainWin.display, mainWin.win, mainWin.gc, rectParams.x, 
            rectParams.y, rectParams.width, rectParams.height);
}

// Drawing API
int addRectangle(int x, int y, int width, int height, Color rgb) {
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

void editRectangle(int id, int x, int y, int width, int height, Color rgb) {
    DrawCallback* callback = getDrawCallbackById(id);
    if (x >= 0)
        callback->params.rectParams.x = x;
    if (y >= 0)
        callback->params.rectParams.y = y;
    if (width >= 0)
        callback->params.rectParams.width = width;
    if (height >= 0)
        callback->params.rectParams.height = height;
    unsigned long int color = fromColorToHex(rgb);
    if (color >= 0) {
        callback->params.rectParams.color = color;
    }
}

void deleteDraw(int id) {
    unregisterDrawCallback(id);  
}

void deleteRectangle(int id) {
    deleteDraw(id);
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

// Drawing related
void addRectangle(int x, int y, int width, int height) {
    printf("Not implemented");
}
#endif

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
