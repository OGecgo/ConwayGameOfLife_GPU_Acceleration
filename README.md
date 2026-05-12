# Conway's Game of Life - GPU Acceleration

A C and CUDA implementation of Conway's Game of Life, designed to demonstrate the performance benefits of GPU parallelization

## Custom Game Rules
This implementation uses an extended neighborhood radius of 5 cells (an 11x11 square around each cell, totaling 120 neighbors):
- **Survival:** A live cell with 34 to 58 live neighbors survives.
- **Death:** A live cell with less than 34 (underpopulation) or more than 58 (overpopulation) live neighbors dies
- **Birth:** A dead cell with exactly 34 to 45 live neighbors becomes a live cell

## Implementations
The project includes three different versions to compare performance:
1. **CPU (Sequential):** Baseline implementation written in standard C
2. **GPU Global Memory (CUDA):** Parallel implementation where each thread calculates the state for a single cell. Uses coalesced global memory reads, but still suffers from high global memory access cos
3. **GPU Shared Memory (CUDA):** Optimized CUDA implementation that loads neighborhood row data into Shared Memory to significantly reduce global memory access and improve bandwidth efficiency

## Compilation and Execution
The project uses a `Makefile` for easy preparation. You can build it with or without the **SDL3** library (which provides real-time window visualization)

### Build Commands
- CPU Version: `make build_cpu`
- GPU Global Version: `make build_gpu_global`
- GPU Shared Version: `make build_gpu_shared`

*Note: To build without GUI visualization (terminal only mode), append `USE_SDL=0` to the build command (e.g., `make build_gpu_shared USE_SDL=0`).*

### Run Commands
- CPU Version: `make run_cpu`
- GPU Global Version: `make run_gpu_global`
- GPU Shared Version: `make run_gpu_shared`

## Performance Highlights
Using GPU parallelization provides massive speedups compared to the sequential CPU approach, as thousands of cells are calculated simultaneously

Measurements for a **1024x1024 grid** (computing 500 generations without SDL visualization):
- **CPU:** ~378 ms per generation
- **GPU Global Memory:** ~1.04 ms per generation (~361x speedup)
- **GPU Shared Memory:** ~0.90 ms per generation (**~419x speedup**)
