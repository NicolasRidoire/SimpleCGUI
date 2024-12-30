#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#ifdef __linux__
#include <X11/X.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>

typedef struct {
    int x;
    int y;
    int width;
    int height;
    unsigned long int color;
} RectParams;

typedef union {
    RectParams rectParams;
} DrawParams;

typedef struct DrawCallback {
    void (*draw)(DrawParams);
    struct DrawCallback* next;
    DrawParams params;
} DrawCallback;

typedef struct {
    Display *display;
    Window rootWin;
    Window win;
    GC gc;
    DrawCallback* firstDraw;
} window;

#elif _WIN32
#include <windows.h>
#include <wingdi.h>
#include <winuser.h>
typedef struct {
    WNDCLASSEX wc;
    HWND hwnd;
} window;

LRESULT CALLBACK eventParsing(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam); 
#endif

typedef struct {
    unsigned char red;
    unsigned char green;
    unsigned char blue;
} Color;

typedef enum {
    OK,
    INIT_ERR
} ERRORS;

// Window related
ERRORS initWindow(const char* windowClassName, void* hInstance); 
ERRORS createWindow(const char* name, long int dwstyle, int x, int y, int width, int height);
int mainLoop();

// Utilies
Color fromHexToColor(unsigned long int rgb);
unsigned long int fromColorToHex(Color rgb);

// Drawing related
void addRectangle(int x, int y, int width, int height, Color rgb);
