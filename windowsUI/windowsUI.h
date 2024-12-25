#include <windows.h>
#include <stdbool.h>
#include <stdio.h>
#include <wingdi.h>
#include <winuser.h>

typedef enum {
    OK,
    INIT_ERR
} ERRORS;

typedef struct {
    WNDCLASSEX wc;
    HWND hwnd;
} window;

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam); 
ERRORS initWindow(const char* windowClassName, HINSTANCE hInstance); 
ERRORS createWindow(const char* name, DWORD dwstyle, int x, int y, int nWidth, int nHeight);
int mainLoop();
