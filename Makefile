
FILES = main.c Renderer2D.c Map.c GameOfLife.c
EXEC = test
LIB = -lSDL3

build : $(FILES)
	gcc $(FILES) -o $(EXEC) $(LIB)

run :
	./$(EXEC)

clear :
	rm -r $(EXEC) 