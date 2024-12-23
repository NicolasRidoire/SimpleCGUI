#include <windows.h>

typedef enum {
    OK,
    INIT_ERR
} ERRORS;

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam); 
ERRORS initWindow(const char* windowClassName, HINSTANCE hInstance); 
HWND createWindow(const char* name, DWORD dwstyle, int x, int y, int nWidth, int nHeight, HINSTANCE hInstance);