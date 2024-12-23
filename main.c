#include <stdio.h>

#include "windowsUI/windowsUI.c"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    const char windowClassName[] = "NESEmulatorClass";
    ERRORS err = initWindow(windowClassName, hInstance);
    if (err != OK) 
        return err;

    HWND hwnd = createWindow("NES Emulator",
            WS_OVERLAPPEDWINDOW,
            CW_USEDEFAULT, CW_USEDEFAULT, 800, 600,
            hInstance);
    if (hwnd == NULL)
        return err;

    MSG msg;
    while (GetMessage(&msg, hwnd, 0, 0) > 0) {
        DispatchMessage(&msg);
    }

    return msg.wParam;
}