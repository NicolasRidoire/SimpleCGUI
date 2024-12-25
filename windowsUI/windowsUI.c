#include "windowsUI.h"

#undef createWindow

window mainWin;

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
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

ERRORS initWindow(const char* windowClassName, HINSTANCE hInstance) {
    mainWin.wc = (WNDCLASSEX){
        .cbSize = sizeof(WNDCLASSEX),
        .lpfnWndProc = WndProc,
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

ERRORS createWindow(const char* name, DWORD dwstyle, int x, int y, int nWidth, int nHeight) {
    HWND hwnd = CreateWindow(mainWin.wc.lpszClassName, name, dwstyle, 
        x, y, nWidth, nHeight, NULL, NULL, mainWin.wc.hInstance, NULL);
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
