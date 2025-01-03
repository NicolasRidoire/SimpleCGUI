#include "SimpleCGUI/SimpleCGUI.c"

#ifdef __linux__
int main() {
    const char windowClassName[] = "NESEmulatorClass";
    ERRORS err = initWindow(windowClassName, NULL);
    if (err != OK) 
        return err;

    err = createWindow("NES Emulator", 0, 0, 0, 800, 600);
    if (err != OK)
        return err;

    Color rectColor = {
        .red = 0,
        .green = 255,
        .blue = 0
    };
    Color rectColor2 = {
        .red = 255,
        .green = 0,
        .blue = 0
    };
    int rectId1 = addRectangle(0, 0, 100, 25, rectColor);
    int rectId2 = addRectangle(200, 200, 100, 25, rectColor);
    editRectangle(rectId2, -1, -1, 200, 200, rectColor2);
    deleteRectangle(rectId1);

    err = mainLoop();

    printf("Finished with error code %d\n", err);
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
