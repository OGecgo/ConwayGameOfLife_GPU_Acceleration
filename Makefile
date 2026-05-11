
GPU_GLOBAL = GoF_Calculation/GoF_Global.cu
GPU_SHARED= GoF_Calculation/GoF_Shared.cu
CPU_FILES = GoF_Calculation/GoF.c

MAIN_FILES = main.c utils/Bitmap.c

EXEC = test
LOGS = logs.txt
FOLDER_EXEC = build
CUDA_FLAGS = -Xptxas -v

# if sdl on. will be created window with output
# if sdl of. only terminal output
USE_SDL ?= 1
ifeq ($(USE_SDL), 1)
    LIB = -lSDL3
	WINDOW_FILES =  GoF_Window/Renderer2D.c GoF_Window/Window.c
else
    LIB = -D NO_SDL
	WINDOW_FILES = 
endif


build_gpu_global: $(MAIN_FILES) $(GPU_GLOBAL)
	mkdir -p $(FOLDER_EXEC)
	nvcc $(CUDA_FLAGS) $(MAIN_FILES) $(WINDOW_FILES) $(GPU_GLOBAL) -o $(FOLDER_EXEC)/$(EXEC)_gpu_global $(LIB)

build_gpu_shared: $(MAIN_FILES) $(GPU_SHARED)
	mkdir -p $(FOLDER_EXEC)
	nvcc $(CUDA_FLAGS) $(MAIN_FILES) $(WINDOW_FILES) $(GPU_SHARED) -o $(FOLDER_EXEC)/$(EXEC)_gpu_shared $(LIB)


build_cpu : $(MAIN_FILES) $(CPU_FILES)
	mkdir -p $(FOLDER_EXEC)
	gcc $(MAIN_FILES) $(CPU_FILES) $(WINDOW_FILES) -o $(FOLDER_EXEC)/$(EXEC)_cpu $(LIB)







run_gpu_global:
	CUDA_LOG_FILE=$(FOLDER_EXEC)/cuda_$(LOGS) ./$(FOLDER_EXEC)/$(EXEC)_gpu_global

run_gpu_shared:
	CUDA_LOG_FILE=$(FOLDER_EXEC)/cuda_$(LOGS) ./$(FOLDER_EXEC)/$(EXEC)_gpu_shared

run_cpu :
	./$(FOLDER_EXEC)/$(EXEC)_cpu




clear :
	rm $(FOLDER_EXEC)/$(EXEC)*
	rm $(FOLDER_EXEC)/*$(LOGS)
