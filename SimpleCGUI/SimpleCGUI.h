#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int x;
    int y;
    int width;
    int height;
    unsigned long int color;
} DrawParams;

typedef struct DrawnObject {
    void (*draw)(DrawParams);
    DrawParams params;
    struct DrawnObject* previous;
    struct DrawnObject* next;
} DrawnObject;

//////////////////////////////////////////////////
#ifdef __linux__
#include <X11/X.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>

typedef struct {
    Display *display;
    Window rootWin;
    Window win;
    GC gc;
    DrawnObject* firstDrawnObject;
} window;

void eventParsing(XEvent generalEvent, bool* isWindowOpen); 
//////////////////////////////////////////////////
#elif _WIN32
#include <windows.h>
#include <wingdi.h>
#include <winuser.h>
#include <windef.h>

typedef struct {
    WNDCLASSEX wc;
    HWND hwnd;
    PAINTSTRUCT ps;
    DrawnObject* firstDrawnObject;
} window;

LRESULT CALLBACK eventParsing(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam); 
#endif
//////////////////////////////////////////////////
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
DrawnObject* getLastDrawnObject();
DrawnObject* registerDrawCallback(void (*callback)(DrawParams), DrawParams params);
void unregisterDrawCallback(DrawnObject* object);
void drawRectangle(DrawParams params);
void deleteDraw(DrawnObject* object);
DrawnObject* addRectangle(int x, int y, int width, int height, Color rgb);
DrawnObject* addCircle(int x, int y, int width, int height, Color rgb);
void deleteRectangle(DrawnObject* rect);
