#include <stdbool.h>
#include <stdio.h>

#ifdef __linux__
#include <X11/Xlib.h>

typedef struct {
    Display *display;
    Window rootWin;
    Window win;
} window;

#elif _WINVER
#include <windows.h>
#include <wingdi.h>
#include <winuser.h>
typedef struct {
    WNDCLASSEX wc;
    HWND hwnd;
} window;

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam); 
#endif

typedef enum {
    OK,
    INIT_ERR
} ERRORS;

ERRORS initWindow(const char* windowClassName, void* hInstance); 
ERRORS createWindow(const char* name, long int dwstyle, int x, int y, int width, int height);
int mainLoop();
