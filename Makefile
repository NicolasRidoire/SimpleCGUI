ifeq ($(OS), Windows_NT)
	CCFLAGS = -lgdi32
else
	CCFLAGS = -lX11
endif

emu-nes:
	gcc main.c $(CCFLAGS) -o main
	./main
