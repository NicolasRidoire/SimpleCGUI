ifeq ($(OS), Windows_NT)
	CCFLAGS = -lgdi32
	EXEC = GUIdemo.exe
else
	CCFLAGS = -lX11
	EXEC = GUIdemo
endif

commands:
	gcc main.c $(CCFLAGS) -o $(EXEC) -Wall -ggdb 
	./$(EXEC)
