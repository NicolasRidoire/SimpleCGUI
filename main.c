#include "SimpleCGUI/SimpleCGUI.c"

int demo();
#ifdef _WIN32
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    const char windowClassName[] = "SimpleCGUIdemo";
    ERRORS err = initWindow(windowClassName, hInstance);
    if (err != OK) 
        return err;
    err = createWindow("SimpleCGUIdemo",
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
    err = createWindow("SimpleCGUIdemo", 0, 0, 0, 800, 600);
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
    DrawnObject* rect = addRectangle(0, 0, 100, 25, rectColor);
    DrawnObject* circle = addCircle(200, 200, 100, 100, rectColor2);
    char test[] = "test";
    DrawnObject* text = addDefaultText(100, 100, test);
    ((TextParams*)(text->params))->text = "test2";

    ERRORS err = mainLoop();

    printf("Finished with error code %d\n", err);
    return err;
}

