#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_STRING_LENGTH 4096

typedef struct {
    int x;
    int y;
} Point;

typedef struct {
    Point pos;
    int width;
    int height;
    unsigned long int color;
} DrawParams;

typedef struct {
    Point pos;
    unsigned long int color;
    const char* text;
} TextParams;

typedef struct DrawnObject {
    void (*draw)(void*);
    void* params;
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
DrawnObject* registerObject(void callback(void*), void* params);
DrawnObject* registerDraw(void callback(void*), DrawParams params);
DrawnObject* registerText(void callback(void*), TextParams params);
void unregisterObject(DrawnObject* object);
void drawRectangle(void* params);
void drawCircle(void* params);
void drawText(void* params);
void deleteDraw(DrawnObject* object);
DrawnObject* addRectangle(int x, int y, int width, int height, Color rgb);
DrawnObject* addCircle(int x, int y, int width, int height, Color rgb);
DrawnObject* addText(int x, int y, const char* text, Color rgb);
DrawnObject* addDefaultText(int x, int y, const char* text);
