#include "SimpleCGUI/SimpleCGUI.c"

int demo();
#ifdef _WIN32
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
    return demo();
}
#elif __linux__
int main() {
    ERRORS err = initWindow(NULL, NULL);
    if (err != OK) 
        return err;
    err = createWindow("NES Emulator", 0, 0, 0, 800, 600);
    if (err != OK)
        return err;

    return demo();
}
#endif
int demo() {
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
    DrawnObject* rectId1 = addRectangle(0, 0, 100, 25, rectColor);
    DrawnObject* rectId2 = addRectangle(200, 200, 100, 25, rectColor);
    rectId2->params.rectParams.color = fromColorToHex(rectColor2);
    deleteRectangle(rectId1);

    ERRORS err = mainLoop();

    printf("Finished with error code %d\n", err);
    return err;
}

