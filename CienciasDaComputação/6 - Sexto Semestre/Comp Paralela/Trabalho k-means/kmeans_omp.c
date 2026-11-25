/*
 * kmeans_omp.c - K-means (algoritmo de Lloyd) - VERSAO PARALELA OpenMP (CPU)
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
 * Os trechos marcados com [OMP] implementam as mudancas para a paralelizacao:
 *
 *  [OMP-1] O laco de atribuicao (N * K * D, o hotspot) virou um
 *          "#pragma omp for" em uma equipe paralela: cada thread processa um bloco
 *          contiguo de pontos (schedule static, ja que todo ponto custa o
 *          mesmo: K * D operacoes).
 *  [OMP-2] labels[i] nao tem condicao de corrida: o ponto i so e tocado
 *          pela thread dona dele.
 *  [OMP-3] "changes" (quantos pontos trocaram de cluster) e compartilhado
 *          por todas as threads -> reduction(+:changes).
 *  [OMP-4] sums[] e counts[] tambem sao escritos por todas as threads
 *          (dois pontos de threads diferentes podem cair no mesmo cluster).
 *          Em vez de atomic/critical (que serializariam N*D somas), usamos
 *          reducao de ARRAY do OpenMP 4.5: cada thread acumula numa copia
 *          privada de tamanho K*D e, no fim do laco, o runtime soma as copias.
 *          Custo extra: P * K * D somas, desprezivel perto de N * K * D.
 *  [OMP-5] O passo de atualizacao (K * D operacoes) ficou sequencial: e tao
 *          pequeno que o custo de criar/sincronizar threads seria maior que
 *          o ganho.
 *
 * Compilar: gcc -O2 -std=c11 -fopenmp -o kmeans_omp kmeans_omp.c -lm
 * Ajuste omp_set_num_threads no fonte e recompile para cada equipe.
 * Executar: ./kmeans_omp covtype.txt 40 30
 */
#define _POSIX_C_SOURCE 199309L
#include <omp.h>
#include "kmeans_common.h"

static int nearest_centroid(const float *pt, const float *cent, int k, int d) {
    int best = 0;
    float best_dist = FLT_MAX;
    for (int c = 0; c < k; c++) {
        const float *cc = &cent[(size_t)c * d];
        float dist = 0.0f;
        for (int j = 0; j < d; j++) {
            float diff = pt[j] - cc[j];
            dist += diff * diff;
        }
        if (dist < best_dist) { best_dist = dist; best = c; }
    }
    return best;
}

int main(int argc, char **argv) {
    if (argc < 4) usage(argv[0]);
    const char *path = argv[1];
    int k = positive_arg(argv[2], "K");
    int max_iter = positive_arg(argv[3], "max_iter");

    Dataset ds = load_dataset(path);
    int n = ds.n, d = ds.d;
    /* ponteiro local com restrict: diz ao compilador que os pontos nao se
     * sobrepoem a sums/labels, permitindo manter valores em registrador */
    const float *restrict x = ds.x;
    const int kd = centroid_size(&ds, k);

    float  *cent   = (float  *)xmalloc((size_t)kd * sizeof(float));
    double *sums   = (double *)xmalloc((size_t)kd * sizeof(double)); /* soma das coords por cluster */
    int    *counts = (int    *)xmalloc((size_t)k * sizeof(int));
    int    *labels = (int    *)xmalloc((size_t)n * sizeof(int));
    for (int i = 0; i < n; i++) labels[i] = -1;
    init_centroids(&ds, k, cent);

    int nthreads = 0;

    omp_set_dynamic(0);
    omp_set_num_threads(32);
    double t0 = wall_time();   /* ---- inicio da medicao (so o algoritmo) ---- */

    int iter = 0, changes = 0;
    while (iter < max_iter) {
        memset(sums, 0, (size_t)kd * sizeof(double));
        memset(counts, 0, (size_t)k * sizeof(int));
        changes = 0;

        /* [OMP-1] laco principal dividido entre as threads
         * [OMP-3] reducao do contador de trocas
         * [OMP-4] reducao de array: copia privada de sums/counts por thread */
        #pragma omp parallel
        {
            #pragma omp single nowait
            nthreads = omp_get_num_threads();

            #pragma omp for schedule(static) nowait \
                    reduction(+:changes) reduction(+:sums[:kd], counts[:k])
            for (int i = 0; i < n; i++) {
                const float *pt = &x[(size_t)i * d];
                int best = nearest_centroid(pt, cent, k, d);
                if (labels[i] != best) { labels[i] = best; changes++; }   /* [OMP-2] */
                counts[best]++;
                double *s = &sums[(size_t)best * d];
                for (int j = 0; j < d; j++) s[j] += pt[j];
            }
        }
        /* barreira no fim da equipe: todas as threads terminaram e as copias
         * privadas ja foram somadas em sums/counts/changes */

        /* [OMP-5] atualizacao sequencial (so K * D operacoes) */
        for (int c = 0; c < k; c++) {
            if (counts[c] == 0) continue;
            for (int j = 0; j < d; j++)
                cent[(size_t)c * d + j] = (float)(sums[(size_t)c * d + j] / counts[c]);
        }

        iter++;
        if (changes == 0) break;
    }

    double elapsed = wall_time() - t0;   /* ---- fim da medicao ---- */

    double sse = compute_sse(&ds, cent, labels);
    printf("Versao: OpenMP (%d threads)\n", nthreads);
    printf("N = %d, D = %d, K = %d\n", n, d, k);
    printf("Iteracoes: %d (%s)\n", iter, changes == 0 ? "convergiu" : "atingiu max_iter");
    printf("SSE final: %.6e\n", sse);
    printf("Tempo do k-means: %.4f s\n", elapsed);
    printf("RESULT omp %d %.6f %d %.10e\n", nthreads, elapsed, iter, sse);

    if (argc >= 5) save_centroids(argv[4], cent, k, d);

    free(ds.x); free(cent); free(sums); free(counts); free(labels);
    return 0;
}
