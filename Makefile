ifeq ($(OS), Windows_NT)
	CCFLAGS = -lgdi32
	EXEC = main.exe
else
	CCFLAGS = -lX11
	EXEC = main
endif

emu-nes:
	gcc main.c $(CCFLAGS) -o $(EXEC)
	./$(EXEC)
