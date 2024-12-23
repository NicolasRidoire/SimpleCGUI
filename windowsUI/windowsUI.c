#include "windowsUI.h"

WNDCLASSEX wc;
    
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
        case WM_CLOSE:
            DestroyWindow(hwnd);
            break;
        case WM_DESTROY:
            PostQuitMessage(0);
            break;
        default:
            return DefWindowProc(hwnd, msg, wParam, lParam);
    }
    return 0;
}

ERRORS initWindow(const char* windowClassName, HINSTANCE hInstance) {
    wc = (WNDCLASSEX){
        .cbSize = sizeof(WNDCLASSEX),
        .lpfnWndProc = WndProc,
        .hInstance = hInstance,
        .hIcon = LoadIcon(NULL, IDI_APPLICATION),
        .hCursor = LoadCursor(NULL, IDC_ARROW),
        .hbrBackground = (HBRUSH)(COLOR_BACKGROUND + 1),
        .lpszMenuName = NULL,
        .lpszClassName = windowClassName,
        .hIconSm = LoadIcon(NULL, IDI_APPLICATION)
    };

    if (!RegisterClassEx(&wc))
        return INIT_ERR;
    return OK;
}

HWND createWindow(const char* name, DWORD dwstyle, int x, int y, int nWidth, int nHeight, HINSTANCE hInstance) {
    HWND hwnd = CreateWindow(wc.lpszClassName, name, dwstyle, 
        x, y, nWidth, nHeight, NULL, NULL, hInstance, NULL);
    ShowWindow(hwnd, SW_SHOW);
    UpdateWindow(hwnd);

    return hwnd;
}