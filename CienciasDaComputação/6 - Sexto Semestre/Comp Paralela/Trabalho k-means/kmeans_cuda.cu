/*
 * kmeans_cuda.cu - K-means (algoritmo de Lloyd) - VERSAO PARALELA CUDA (GPU)
 * ============================================================================
 * TEMPOS MEDIDOS EM 24/11/2026 (1 execucao por configuracao)
 * CPU: Intel Core i5-11400F (6 nucleos fisicos / 12 logicos), Windows 11 Pro
 * GPU: NVIDIA GeForce RTX 2060 (blocos CUDA de 256 threads)
 * Dataset: Covertype (UCI), N = 581012, D = 54, K = 40, max_iter = 30
 * Todas as versoes executaram 30 iteracoes e tiveram SSE 1.3526299496e+07.
 *
 *   Versao             | Tempo (s) | Speedup
 *   -------------------+-----------+--------
 *   Sequencial         | 18.113694 |  1.00x
 *   OpenMP  1 thread   | 20.148328 |  0.90x
 *   OpenMP  2 threads  | 11.845973 |  1.53x
 *   OpenMP  4 threads  |  7.770411 |  2.33x
 *   OpenMP  8 threads  |  4.779484 |  3.79x
 *   OpenMP 16 threads  |  4.407301 |  4.11x
 *   OpenMP 32 threads  |  3.984387 |  4.55x
 *   CUDA (tempo total) |  0.492608 | 36.77x
 *   CUDA (laco)        |  0.433859 |      -
 * ============================================================================
 *
 * Mesmo algoritmo do kmeans_seq.c. Mudancas para a GPU (marcadas [CUDA]):
 *
 *  [CUDA-1] Os dados sao copiados UMA vez para a GPU, antes do laco. Dentro
 *           do laco so trafega 1 inteiro por iteracao (o contador de trocas,
 *           usado para o criterio de parada).
 *  [CUDA-2] Os pontos vao para a GPU TRANSPOSTOS (layout coluna:
 *           xT[j*N + i]). Assim, quando as threads vizinhas i, i+1, ... leem
 *           a feature j, acessam enderecos consecutivos -> acesso
 *           "coalescido" a memoria global (menos transacoes por warp).
 *  [CUDA-3] Kernel assign_and_accumulate: 1 thread por ponto. Substitui o
 *           laco "for i" do sequencial. Cada thread acha o centroide mais
 *           proximo e grava labels[i] (sem corrida: so ela escreve ali).
 *  [CUDA-4] Os centroides (K*D floats) sao lidos por todas as threads do
 *           bloco, entao sao carregados uma vez na memoria compartilhada.
 *  [CUDA-5] As somas por cluster tem corrida (muitas threads, mesmo
 *           cluster). Solucao em 2 niveis: cada bloco acumula com atomicAdd
 *           na memoria COMPARTILHADA (rapida) e, no fim, o bloco soma o seu
 *           parcial na memoria global com ate K*D atomicos de soma por bloco.
 *           (Mesma ideia da reducao de array do OpenMP.)
 *  [CUDA-6] O contador de trocas tambem e reduzido no bloco e depois
 *           somado no global com 1 atomicAdd por bloco.
 *  [CUDA-7] Kernel update_centroids: 1 thread por coordenada (K*D threads)
 *           calcula a media. Fica na GPU para nao copiar sums/counts de volta.
 *
 * Requer GPU com compute capability >= 6.0 (atomicAdd em double).
 * Compilar: nvcc -O2 --fmad=false -arch=sm_75 -o kmeans_cuda kmeans_cuda.cu
 *           (troque sm_75 pela arquitetura da sua GPU)
 * Executar: ./kmeans_cuda covtype.txt 40 30
 */
#include <cuda_runtime.h>
#include "kmeans_common.h"

#define BLOCK_SIZE 256

#define CUDA_CHECK(call)                                                      \
    do {                                                                      \
        cudaError_t err__ = (call);                                           \
        if (err__ != cudaSuccess) {                                           \
            fprintf(stderr, "Erro CUDA %s:%d: %s\n", __FILE__, __LINE__,      \
                    cudaGetErrorString(err__));                               \
            exit(1);                                                          \
        }                                                                     \
    } while (0)

/* [CUDA-3..6] Passo 1 (atribuicao) + acumulo das somas. 1 thread = 1 ponto. */
__global__ void assign_and_accumulate(const float *__restrict__ xT, int n, int d, int k,
                                      const float *__restrict__ cent,
                                      int *labels, double *sums, int *counts, int *changes)
{
    /* memoria compartilhada dinamica: [ s_sums (K*D doubles) | s_cent (K*D floats)
     *                                   | s_counts (K ints) | s_changes (1 int) ]
     * doubles primeiro para manter o alinhamento de 8 bytes */
    extern __shared__ double smem[];
    const int kd = k * d;
    double *s_sums    = smem;
    float  *s_cent    = (float *)(s_sums + kd);
    int    *s_counts  = (int *)(s_cent + kd);
    int    *s_changes = s_counts + k;

    /* [CUDA-4] carrega centroides e zera os acumuladores do bloco */
    for (int t = threadIdx.x; t < kd; t += blockDim.x) {
        s_cent[t] = cent[t];
        s_sums[t] = 0.0;
    }
    for (int t = threadIdx.x; t < k; t += blockDim.x) s_counts[t] = 0;
    if (threadIdx.x == 0) *s_changes = 0;
    __syncthreads();

    int i = blockIdx.x * blockDim.x + threadIdx.x;
    if (i < n) {
        /* centroide mais proximo (mesma conta do sequencial) */
        int best = 0;
        float best_dist = FLT_MAX;
        for (int c = 0; c < k; c++) {
            float dist = 0.0f;
            for (int j = 0; j < d; j++) {
                float diff = xT[(size_t)j * n + i] - s_cent[c * d + j];  /* [CUDA-2] coalescido */
                dist += diff * diff;
            }
            if (dist < best_dist) { best_dist = dist; best = c; }
        }
        if (labels[i] != best) {
            labels[i] = best;
            atomicAdd(s_changes, 1);                    /* [CUDA-6] */
        }
        /* [CUDA-5] nivel 1: acumulo no bloco (memoria compartilhada) */
        atomicAdd(&s_counts[best], 1);
        for (int j = 0; j < d; j++)
            atomicAdd(&s_sums[best * d + j], (double)xT[(size_t)j * n + i]);
    }
    __syncthreads();

    /* [CUDA-5] nivel 2: parcial do bloco -> memoria global */
    for (int t = threadIdx.x; t < kd; t += blockDim.x)
        if (s_sums[t] != 0.0) atomicAdd(&sums[t], s_sums[t]);
    for (int t = threadIdx.x; t < k; t += blockDim.x)
        if (s_counts[t] != 0) atomicAdd(&counts[t], s_counts[t]);
    if (threadIdx.x == 0 && *s_changes != 0) atomicAdd(changes, *s_changes);
}

/* [CUDA-7] Passo 2 (atualizacao): 1 thread por coordenada de centroide. */
__global__ void update_centroids(float *cent, const double *sums, const int *counts, int k, int d)
{
    int t = blockIdx.x * blockDim.x + threadIdx.x;
    if (t < k * d) {
        int c = t / d;
        if (counts[c] > 0) cent[t] = (float)(sums[t] / counts[c]);   /* vazio: mantem */
    }
}

int main(int argc, char **argv) {
    if (argc < 4) usage(argv[0]);
    const char *path = argv[1];
    int k = positive_arg(argv[2], "K");
    int max_iter = positive_arg(argv[3], "max_iter");

    Dataset ds = load_dataset(path);
    int n = ds.n, d = ds.d;
    const int kd = centroid_size(&ds, k);

    float *cent   = (float *)xmalloc((size_t)kd * sizeof(float));
    int   *labels = (int *)xmalloc((size_t)n * sizeof(int));
    init_centroids(&ds, k, cent);

    float *xT = (float *)xmalloc((size_t)n * d * sizeof(float));

    size_t shmem = (size_t)kd * (sizeof(double) + sizeof(float)) + ((size_t)k + 1) * sizeof(int);
    cudaDeviceProp prop;
    CUDA_CHECK(cudaGetDeviceProperties(&prop, 0));
    if (shmem > prop.sharedMemPerBlock) {
        fprintf(stderr, "K*D grande demais para a memoria compartilhada (%zu > %zu bytes)\n",
                shmem, (size_t)prop.sharedMemPerBlock);
        return 1;
    }
    CUDA_CHECK(cudaFree(0));   /* forca a criacao do contexto antes de medir */

    float *d_x, *d_cent; double *d_sums; int *d_labels, *d_counts, *d_changes;
    CUDA_CHECK(cudaMalloc(&d_x,       (size_t)n * d * sizeof(float)));
    CUDA_CHECK(cudaMalloc(&d_cent,    (size_t)kd * sizeof(float)));
    CUDA_CHECK(cudaMalloc(&d_sums,    (size_t)kd * sizeof(double)));
    CUDA_CHECK(cudaMalloc(&d_labels,  (size_t)n * sizeof(int)));
    CUDA_CHECK(cudaMalloc(&d_counts,  (size_t)k * sizeof(int)));
    CUDA_CHECK(cudaMalloc(&d_changes, sizeof(int)));

    cudaEvent_t ev_start, ev_stop;
    CUDA_CHECK(cudaEventCreate(&ev_start));
    CUDA_CHECK(cudaEventCreate(&ev_stop));

    double t0 = wall_time();   /* ---- inclui transposicao e copias ---- */

    /* [CUDA-2] transposicao no host; e parte do custo da versao GPU. */
    for (int i = 0; i < n; i++)
        for (int j = 0; j < d; j++)
            xT[(size_t)j * n + i] = ds.x[(size_t)i * d + j];

    /* [CUDA-1] copia unica dos dados para a GPU */
    CUDA_CHECK(cudaMemcpy(d_x, xT, (size_t)n * d * sizeof(float), cudaMemcpyHostToDevice));
    CUDA_CHECK(cudaMemcpy(d_cent, cent, (size_t)kd * sizeof(float), cudaMemcpyHostToDevice));
    CUDA_CHECK(cudaMemset(d_labels, 0xFF, (size_t)n * sizeof(int)));   /* labels = -1 */

    int grid_pts = 1 + (n - 1) / BLOCK_SIZE;
    int grid_kd  = 1 + (kd - 1) / BLOCK_SIZE;

    CUDA_CHECK(cudaEventRecord(ev_start));
    int iter = 0, changes = 0;
    while (iter < max_iter) {
        CUDA_CHECK(cudaMemset(d_sums, 0, (size_t)kd * sizeof(double)));
        CUDA_CHECK(cudaMemset(d_counts, 0, (size_t)k * sizeof(int)));
        CUDA_CHECK(cudaMemset(d_changes, 0, sizeof(int)));

        assign_and_accumulate<<<grid_pts, BLOCK_SIZE, shmem>>>(d_x, n, d, k, d_cent,
                                                               d_labels, d_sums, d_counts, d_changes);
        CUDA_CHECK(cudaGetLastError());
        update_centroids<<<grid_kd, BLOCK_SIZE>>>(d_cent, d_sums, d_counts, k, d);
        CUDA_CHECK(cudaGetLastError());

        /* [CUDA-1] unico dado copiado por iteracao (e funciona como sincronizacao) */
        CUDA_CHECK(cudaMemcpy(&changes, d_changes, sizeof(int), cudaMemcpyDeviceToHost));
        iter++;
        if (changes == 0) break;
    }
    CUDA_CHECK(cudaEventRecord(ev_stop));
    CUDA_CHECK(cudaEventSynchronize(ev_stop));

    CUDA_CHECK(cudaMemcpy(cent, d_cent, (size_t)kd * sizeof(float), cudaMemcpyDeviceToHost));
    CUDA_CHECK(cudaMemcpy(labels, d_labels, (size_t)n * sizeof(int), cudaMemcpyDeviceToHost));

    double elapsed = wall_time() - t0;   /* ---- fim da medicao ---- */
    float loop_ms = 0.0f;
    CUDA_CHECK(cudaEventElapsedTime(&loop_ms, ev_start, ev_stop));

    double sse = compute_sse(&ds, cent, labels);
    printf("Versao: CUDA (GPU: %s, blocos de %d threads)\n", prop.name, BLOCK_SIZE);
    printf("N = %d, D = %d, K = %d\n", n, d, k);
    printf("Iteracoes: %d (%s)\n", iter, changes == 0 ? "convergiu" : "atingiu max_iter");
    printf("SSE final: %.6e\n", sse);
    printf("Tempo do k-means (transposicao e copias incluidas): %.4f s\n", elapsed);
    printf("Tempo do laco CUDA (cudaEvent): %.4f s\n", loop_ms / 1000.0);
    printf("RESULT cuda 0 %.6f %d %.10e %.6f\n", elapsed, iter, sse, loop_ms / 1000.0);

    if (argc >= 5) save_centroids(argv[4], cent, k, d);

    cudaFree(d_x); cudaFree(d_cent); cudaFree(d_sums);
    cudaFree(d_labels); cudaFree(d_counts); cudaFree(d_changes);
    cudaEventDestroy(ev_start); cudaEventDestroy(ev_stop);
    free(ds.x); free(xT); free(cent); free(labels);
    return 0;
}
