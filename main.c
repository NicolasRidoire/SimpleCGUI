#include "windowsUI/windowsUI.c"

#ifdef __linux__
int main() {
    const char windowClassName[] = "NESEmulatorClass";
    ERRORS err = initWindow(windowClassName, NULL);
    if (err != OK) 
        return err;

    err = createWindow("NES Emulator",
            0, 0, 0, 800, 600);
    if (err != OK)
        return err;


    err = mainLoop();

    printf("Finished : %d", err);
    return err;
}

#else

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

#endif
