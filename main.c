#include "windowsUI/windowsUI.c"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    const char windowClassName[] = "NESEmulatorClass";
    ERRORS err = initWindow(windowClassName, hInstance);
    if (err != OK) 
        return err;

    err = createWindow("NES Emulator",
            WS_OVERLAPPEDWINDOW,
            CW_USEDEFAULT, CW_USEDEFAULT, 800, 600);
    if (err != OK)
        return err;


    err = mainLoop();

    printf("Finished : %d", err);
    return err;
}
