#include "SimpleCGUI.h"

#define DEFAULT_BK_COLOR 0x1e1e2e

static window mainWin;

//////////////////////////////////////////////////
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
void setDefaultFont() {
    XFontStruct* font = XLoadQueryFont(mainWin.display, "-misc-aurebeshinv-medium-r-normal--0-0-0-0-p-0-iso10646-1");
    printf("%d\n", font->fid);
    XSetFont(mainWin.display, mainWin.gc, font->fid);
}

void drawRectangle(void* params) {
    DrawParams* p = (DrawParams*)params;
    XSetForeground(mainWin.display, mainWin.gc, p->color);
    XFillRectangle(mainWin.display, mainWin.win, mainWin.gc, p->pos.x, 
            p->pos.y, p->width, p->height);
}

void drawCircle(void* params) {
    DrawParams* p = (DrawParams*)params;
    XSetForeground(mainWin.display, mainWin.gc, p->color);
    XFillArc(mainWin.display, mainWin.win, mainWin.gc, p->pos.x, 
            p->pos.y, p->width, p->height, 0, 360*64);
}

void drawText(void* params) {
    TextParams* p = (TextParams*)params;
    setDefaultFont();
    XSetForeground(mainWin.display, mainWin.gc, p->color);
    XDrawString(mainWin.display, mainWin.win, mainWin.gc, p->pos.x, p->pos.y, p->text, strlen(p->text));
}

//////////////////////////////////////////////////
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
        .hbrBackground = CreateSolidBrush(DEFAULT_BK_COLOR),
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
void setBrushColor(int color) {
    Color rgb = fromHexToColor(color);
    SelectObject(mainWin.ps.hdc, CreateSolidBrush(RGB(rgb.red, rgb.green, rgb.blue)));
}

void setPenColor(int color) {
    Color rgb = fromHexToColor(color);
    SelectObject(mainWin.ps.hdc, CreatePen(PS_SOLID, 0, RGB(rgb.red, rgb.green, rgb.blue)));
}

void setTextColor(int color) {
    Color rgb = fromHexToColor(color);
    SetTextColor(mainWin.ps.hdc, RGB(rgb.red, rgb.green, rgb.blue));
}

void setBkColor(int color) {
    Color rgb = fromHexToColor(color);
    SetBkColor(mainWin.ps.hdc, RGB(rgb.red, rgb.green, rgb.blue));
}

void drawRectangle(void* params) {
    DrawParams* p = (DrawParams*)params;
    setBrushColor(p->color);
    if (Rectangle(mainWin.ps.hdc, p->pos.x, p->pos.y, p->pos.x + p->width, p->pos.y + p->height) == 0)
        printf("Failed to draw the rectangle\n");
}

void drawCircle(void* params) {
    DrawParams* p = (DrawParams*)params;
    setBrushColor(p->color);
    if (Ellipse(mainWin.ps.hdc, p->pos.x, p->pos.y, p->pos.x + p->width, p->pos.y + p->height) == 0)
        printf("Failed to draw the ellipse\n");
}

void setDefaultFont() {
    HFONT font = CreateFont(16, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, DEFAULT_CHARSET, 
            OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_QUALITY, 
            "Arial");
    SelectObject(mainWin.ps.hdc, font);
}   

void drawText(void* params) {
    TextParams* pt = (TextParams*)params;
    Point p = pt->pos;
    setDefaultFont();
    setTextColor(pt->color);
    setBkColor(DEFAULT_BK_COLOR);
    if (TextOutA(mainWin.ps.hdc, p.x, p.y, pt->text, strlen(pt->text)) == 0)
        printf("Failed to draw the text %s\n", pt->text);
}

#endif
//////////////////////////////////////////////////

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

DrawnObject* registerObject(void callback(void*), void* params) {
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

DrawnObject* registerDraw(void callback(void*), DrawParams params) {
    DrawParams* pParams = malloc(sizeof(DrawParams));
    *pParams = params;
    return registerObject(callback, pParams);
}

DrawnObject* registerText(void callback(void*), TextParams params) {
    TextParams* pParams = malloc(sizeof(TextParams));
    *pParams = params;
    return registerObject(callback, pParams);
}

void unregisterObject(DrawnObject* object) {
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
    free(object->params);
    free(object);
}

// Drawing API
void deleteDraw(DrawnObject* object) {
    unregisterObject(object);  
}

DrawnObject* addRectangle(int x, int y, int width, int height, Color rgb) {
    DrawParams params = {
        .color = fromColorToHex(rgb),
        .pos = {
            .x = x,
            .y = y,
        },
        .width = width,
        .height = height
    };
    return registerDraw(&drawRectangle, params);
}

DrawnObject* addCircle(int x, int y, int width, int height, Color rgb) {
    DrawParams params = {
        .color = fromColorToHex(rgb),
        .pos = {
            .x = x,
            .y = y,
        },
        .width = width,
        .height = height
    };
    return registerDraw(&drawCircle, params);
}

DrawnObject* addText(int x, int y, const char* text, Color rgb) {
    TextParams params = {
        .text = text,
        .color = fromColorToHex(rgb),
        .pos = {
            .x = x,
            .y = y,
        }
    };
    return registerText(&drawText, params); 
}

DrawnObject* addDefaultText(int x, int y, const char* text) {
    if (strlen(text) > MAX_STRING_LENGTH) {
        printf("string is too long");
        return NULL;
    }
    TextParams params = {
        .text = text,
        .color = 0xffffff,
        .pos = {
            .x = x,
            .y = y,
        }
    };
    return registerText(&drawText, params); 
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
