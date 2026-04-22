
FILES = Renderer2D.c Window.c Bitmap.c
FILES_CPU = GoF.c main.c
FILES_GPU = GoF.cu main.cpp


EXEC = test
FOLDER_EXEC = build
LIB = -lSDL3

build_cpu : $(FILES) $(FILES_CPU)
	mkdir -p $(FOLDER_EXEC)
	gcc $(FILES) $(FILES_CPU) -o $(FOLDER_EXEC)/$(EXEC) $(LIB)

build_gpu: $(FILES) $(FILES_GPU)
	mkdir -p $(FOLDER_EXEC)
	nvcc $(FILES) $(FILES_GPU) -o $(FOLDER_EXEC)/$(EXEC) $(LIB)

run :
	./$(FOLDER_EXEC)/$(EXEC)


clear :
	rm -r $(EXEC) 