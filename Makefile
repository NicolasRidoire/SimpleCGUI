ifeq ($(OS), Windows_NT)
	CCFLAGS = -lgdi32
	EXEC = emu-nes.exe
else
	CCFLAGS = -lX11
	EXEC = emu-nes
endif

commands:
	gcc main.c $(CCFLAGS) -o $(EXEC) -Wall
	./$(EXEC)
