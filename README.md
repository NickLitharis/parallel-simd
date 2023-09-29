# Parallelization and SIMD Optimization

This project explores parallelization and Single Instruction, Multiple Data (SIMD) optimization techniques to enhance the performance of algorithms for finding the minimum and maximum elements in a one-dimensional array. The code is divided into two major sections, each focusing on different optimization strategies.

## Parallelization


### Compilation

To compile each of the parallelization implementations, you can use the provided compilation commands:

- For Pthreads:
  ```bash
  gcc -o minmax-pthreads -Wall -O2 minmax-pthreads.c -lpthread -DN=10000 -DR=10000
  ```

- For OpenMP:
  ```bash
  gcc -o minmax-openmp -Wall -O2 -fopenmp minmax-openmp.c -DN=10000 -DR=10000
  ```

- For Intel TBB:
  ```bash
  g++ -o minmax-tbb -Wall -O2 -std=c++11 minmax-tbb.cpp -ltbb -DN=10000 -DR=10000
  ```

### Implementation Details

#### Pthreads

The Pthreads implementation divides the array into multiple segments and assigns each segment to a separate thread. Each thread independently searches for the maximum element within its assigned segment. Afterward, a mutex is used to update the global maximum. Finally, the maximum values found by each thread are compared, and the overall maximum is determined.

#### OpenMP

The OpenMP implementation parallelizes the computation using OpenMP directives. It initializes an array with random numbers and assigns both the minimum (`checkmin`) and maximum (`checkmax`) values to the initial element. The loop that fills the array is parallelized using `#pragma omp parallel for`. The `reduction` clauses with `min` and `max` operations are used in the next loop iteration to perform parallel reductions of the final minimum and maximum values. The calculated minimum and maximum values are compared with the expected values (`checkmin` and `checkmax`) to verify correctness.

#### Intel TBB

The Intel TBB implementation is a parallel implementation for finding the minimum and maximum elements in a one-dimensional array `v` using the Intel Threading Building Blocks (TBB) library. It uses the `parallel_for` loop to divide the array into blocks and assign each block to a separate thread. Each thread maintains a local minimum and maximum value using `tbb::combinable` and `minmax_combinable`. A lambda function initializes the local minimum and maximum values with the first element of the array. As each thread processes its assigned block, it updates its local minimum and maximum values. Once the loop is complete, the local minimums and maximums are combined using `minmax_combinable` to perform the reduction operation. Finally, the results of the parallel computation are compared with the serial computation to ensure correctness.


## SIMD Optimization


### Compilation

To compile the SIMD optimization code, you can use the provided compilation command:

```bash
gcc -msse2 -Wall -O2 threshold-count-float-sse.c -o threshold-count-float-sse -DN=10000 -DR=10000 -DTHRESHOLD=50.0
```

This command compiles the program with optimizations enabled (`-O2`), defines the array size (`-DN`), the number of repetitions (`-DR`), and the threshold value (`-DTHRESHOLD`). You can adjust these parameters as needed.

### Implementation Details

The SIMD optimization code efficiently counts the number of values greater than a specified threshold in a float array. It utilizes Single Instruction, Multiple Data (SIMD) instructions, specifically SSE2 (Streaming SIMD Extensions 2), for enhanced performance.

The program initializes a float array with random values and counts the values greater than the specified threshold using SIMD instructions. It leverages SSE2 instructions to perform parallel processing, making it suitable for applications requiring high-performance threshold counting in float arrays.
