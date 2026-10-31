/*
 ============================================================================
 TEMPOS DE EXECUÇÃO:
 - Sequencial: 13.314349 segundos
 - Paralela Multicore (OpenMP): 7.781263 segundos
 - Paralela GPU (OpenMP): 8.071083 segundos
 - CUDA: 0.137781 segundos

 MÉTRICAS NVPROF (CUDA):
 - warps_launched: 125000
 - warp_execution_efficiency: 100.00%
 ============================================================================
*/

#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <cuda.h>

// Kernel CUDA para Multiplicação de Matrizes
__global__ void mm_kernel(double* a, double* b, double* c, int width) 
{
    // Dica i: Dois índices para lidar com as duas dimensões (linha e coluna)
    int col = blockIdx.x * blockDim.x + threadIdx.x;
    int row = blockIdx.y * blockDim.y + threadIdx.y;

    // Dica iv: Conferir se as threads não estão acessando elementos inexistentes
    if (row < width && col < width) {
        double sum = 0.0;
        
        // Dica ii: Apenas o laço do "k" (mais interno) é executado no kernel
        for (int k = 0; k < width; k++) {
            double x = a[row * width + k];
            double y = b[k * width + col];
            sum += x * y;
        }
        c[row * width + col] = sum;
    }
}

int main()
{
    int width = 2000;
    size_t size = width * width * sizeof(double);

    // 1. Alocação de memória no Host (CPU)
    double *h_a = (double*) malloc (size);
    double *h_b = (double*) malloc (size);
    double *h_c = (double*) malloc (size);

    // Inicialização das matrizes
    for(int i = 0; i < width; i++) {
        for(int j = 0; j < width; j++) {
            h_a[i * width + j] = i;
            h_b[i * width + j] = j;
            h_c[i * width + j] = 0;
        }
    }

    // 2. Alocação de memória no Device (GPU)
    double *d_a, *d_b, *d_c;
    cudaMalloc((void**)&d_a, size);
    cudaMalloc((void**)&d_b, size);
    cudaMalloc((void**)&d_c, size);

    // Início da medição de tempo
    struct timeval start, end;
    gettimeofday(&start, NULL);

    // 3. Cópia dos dados da CPU (Host) para a GPU (Device)
    cudaMemcpy(d_a, h_a, size, cudaMemcpyHostToDevice);
    cudaMemcpy(d_b, h_b, size, cudaMemcpyHostToDevice);

    // Dica iii: dimGrid e dimBlock utilizam a segunda dimensão (y)
    dim3 dimBlock(16, 16);
    dim3 dimGrid((width + dimBlock.x - 1) / dimBlock.x, (width + dimBlock.y - 1) / dimBlock.y);

    // 4. Lançamento do Kernel
    mm_kernel<<<dimGrid, dimBlock>>>(d_a, d_b, d_c, width);
    
    // Sincroniza a GPU para garantir que o kernel terminou antes de prosseguir
    cudaDeviceSynchronize(); 

    // 5. Cópia do resultado da GPU (Device) de volta para a CPU (Host)
    cudaMemcpy(h_c, d_c, size, cudaMemcpyDeviceToHost);

    // Fim da medição de tempo
    gettimeofday(&end, NULL);
  
    double time_taken = (end.tv_sec - start.tv_sec) * 1e6;
    time_taken = (time_taken + (end.tv_usec - start.tv_usec)) * 1e-6;
  
    printf("Tempo de execução (CUDA): %f segundos\n", time_taken);

    // 6. Liberação da memória
    cudaFree(d_a);
    cudaFree(d_b);
    cudaFree(d_c);
    free(h_a);
    free(h_b);
    free(h_c);

    return 0;
}