
FILES = Renderer2D.c Window.c Bitmap.c
FILES_CPU = GoF.c main.c
FILES_GPU = GoF.cu main.cu

EXEC = test
LOGS = logs.txt
FOLDER_EXEC = build
LIB = -lSDL3




# errors only for gpu using cuda api
build_gpu: $(FILES) $(FILES_GPU)
	mkdir -p $(FOLDER_EXEC)
	nvcc $(FILES) $(FILES_GPU) -o $(FOLDER_EXEC)/$(EXEC)_gpu $(LIB)

build_cpu : $(FILES) $(FILES_CPU)
	mkdir -p $(FOLDER_EXEC)
	gcc $(FILES) $(FILES_CPU) -o $(FOLDER_EXEC)/$(EXEC)_cpu $(LIB)

run_gpu:
	CUDA_LOG_FILE=$(FOLDER_EXEC)/cuda_$(LOGS) ./$(FOLDER_EXEC)/$(EXEC)_gpu

run_cpu :
	./$(FOLDER_EXEC)/$(EXEC)_cpu


clear :
	rm $(FOLDER_EXEC)/$(EXEC)*
	rm $(FOLDER_EXEC)/*$(LOGS)
