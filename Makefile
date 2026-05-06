
GPU_NO_SHARED_MEMORY_FILES = GoF_Canculations/GoF_No_shared_Memory.cu
# GPU_SHARED_MEMORY_FILES= GoF_Canculations/GoF_shared_Memory.cu
CPU_FILES = GoF_Canculations/GoF.c

MAIN_FILES = main.c Gof_Window/Renderer2D.c Gof_Window/Window.c utils/Bitmap.c

EXEC = test
LOGS = logs.txt
FOLDER_EXEC = build
LIB = -lSDL3




# errors only for gpu using cuda api
build_gpu_no_shared_memory: $(MAIN_FILES) $(GPU_NO_SHARED_MEMORY_FILES)
	mkdir -p $(FOLDER_EXEC)
	nvcc $(MAIN_FILES) $(GPU_NO_SHARED_MEMORY_FILES) -o $(FOLDER_EXEC)/$(EXEC)_gpu_no_shared_memory $(LIB)

# build_gpu_shared_memory: $(MAIN_FILES) $(GPU_SHARED_MEMORY_FILES)
# 	mkdir -p $(FOLDER_EXEC)
# 	nvcc $(MAIN_FILES) $(GPU_SHARED_MEMORY_FILES) -o $(FOLDER_EXEC)/$(EXEC)_gpu_shared_memory $(LIB)


build_cpu : $(MAIN_FILES) $(CPU_FILES)
	mkdir -p $(FOLDER_EXEC)
	gcc $(MAIN_FILES) $(CPU_FILES) -o $(FOLDER_EXEC)/$(EXEC)_cpu $(LIB)




run_gpu_no_shared_memory:
	CUDA_LOG_FILE=$(FOLDER_EXEC)/cuda_$(LOGS) ./$(FOLDER_EXEC)/$(EXEC)_gpu_no_shared_memory

# run_gpu_shared_memory:
# 	CUDA_LOG_FILE=$(FOLDER_EXEC)/cuda_$(LOGS) ./$(FOLDER_EXEC)/$(EXEC)_gpu_shared_memory

run_cpu :
	./$(FOLDER_EXEC)/$(EXEC)_cpu




clear :
	rm $(FOLDER_EXEC)/$(EXEC)*
	rm $(FOLDER_EXEC)/*$(LOGS)
