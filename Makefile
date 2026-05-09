
GPU_GLOBAL = GoF_Canculation/GoF_Global.cu
GPU_SHARED= GoF_Canculation/GoF_Shared.cu
CPU_FILES = GoF_Canculation/GoF.c

MAIN_FILES = main.c GoF_Window/Renderer2D.c GoF_Window/Window.c utils/Bitmap.c

EXEC = test
LOGS = logs.txt
FOLDER_EXEC = build
LIB = -lSDL3
CUDA_FLAGS = -Xptxas -v



# errors only for gpu using cuda api
build_gpu_global: $(MAIN_FILES) $(GPU_GLOBAL)
	mkdir -p $(FOLDER_EXEC)
	nvcc $(CUDA_FLAGS) $(MAIN_FILES) $(GPU_GLOBAL) -o $(FOLDER_EXEC)/$(EXEC)_gpu_no_shared_memory $(LIB)

build_gpu_shared: $(MAIN_FILES) $(GPU_SHARED)
	mkdir -p $(FOLDER_EXEC)
	nvcc $(CUDA_FLAGS) $(MAIN_FILES) $(GPU_SHARED) -o $(FOLDER_EXEC)/$(EXEC)_gpu_shared_memory $(LIB)


build_cpu : $(MAIN_FILES) $(CPU_FILES)
	mkdir -p $(FOLDER_EXEC)
	gcc $(MAIN_FILES) $(CPU_FILES) -o $(FOLDER_EXEC)/$(EXEC)_cpu $(LIB)




run_gpu_global:
	CUDA_LOG_FILE=$(FOLDER_EXEC)/cuda_$(LOGS) ./$(FOLDER_EXEC)/$(EXEC)_gpu_no_shared_memory

run_gpu_shared:
	CUDA_LOG_FILE=$(FOLDER_EXEC)/cuda_$(LOGS) ./$(FOLDER_EXEC)/$(EXEC)_gpu_shared_memory

run_cpu :
	./$(FOLDER_EXEC)/$(EXEC)_cpu




clear :
	rm $(FOLDER_EXEC)/$(EXEC)*
	rm $(FOLDER_EXEC)/*$(LOGS)
