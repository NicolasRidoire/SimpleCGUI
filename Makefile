ifeq ($(OS), Windows_NT)
	CCFLAGS = -lgdi32
else

endif

emu-nes:
	gcc main.c $(CCFLAGS) -o main
	./main
