/*
 * kmeans_seq.c - K-means (algoritmo de Lloyd) - VERSAO SEQUENCIAL
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
 * Algoritmo, a cada iteracao:
 *   1) ATRIBUICAO: cada ponto vai para o cluster do centroide mais proximo
 *      (distancia euclidiana ao quadrado - a raiz nao muda qual e o menor).
 *   2) ATUALIZACAO: cada centroide vira a media dos pontos do seu cluster.
 *   Para quando nenhum ponto troca de cluster ou quando chega em max_iter.
 *
 * Custo por iteracao: O(N * K * D). E o passo 1 (+ o acumulo das somas)
 * que domina o tempo, e e ele que as versoes paralelas atacam.
 *
 * Compilar: gcc -O2 -std=c11 -o kmeans_seq kmeans_seq.c -lm
 * Executar: ./kmeans_seq covtype.txt 40 30
 */
#define _POSIX_C_SOURCE 199309L
#include "kmeans_common.h"

/* Retorna o indice do centroide mais proximo do ponto pt. */
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
    int    *counts = (int    *)xmalloc((size_t)k * sizeof(int));        /* pontos por cluster */
    int    *labels = (int    *)xmalloc((size_t)n * sizeof(int));        /* cluster de cada ponto */
    for (int i = 0; i < n; i++) labels[i] = -1;
    init_centroids(&ds, k, cent);

    double t0 = wall_time();   /* ---- inicio da medicao (so o algoritmo) ---- */

    int iter = 0, changes = 0;
    while (iter < max_iter) {
        memset(sums, 0, (size_t)kd * sizeof(double));
        memset(counts, 0, (size_t)k * sizeof(int));
        changes = 0;

        /* Passo 1 (atribuicao) + acumulo das somas para o passo 2.
         * Este laco e o "hotspot": N * K * D operacoes. */
        for (int i = 0; i < n; i++) {
            const float *pt = &x[(size_t)i * d];
            int best = nearest_centroid(pt, cent, k, d);
            if (labels[i] != best) { labels[i] = best; changes++; }
            counts[best]++;
            double *s = &sums[(size_t)best * d];
            for (int j = 0; j < d; j++) s[j] += pt[j];
        }

        /* Passo 2 (atualizacao): centroide = media. Custo so O(K * D).
         * Cluster vazio mantem o centroide anterior. */
        for (int c = 0; c < k; c++) {
            if (counts[c] == 0) continue;
            for (int j = 0; j < d; j++)
                cent[(size_t)c * d + j] = (float)(sums[(size_t)c * d + j] / counts[c]);
        }

        iter++;
        if (changes == 0) break;   /* convergiu */
    }

    double elapsed = wall_time() - t0;   /* ---- fim da medicao ---- */

    double sse = compute_sse(&ds, cent, labels);
    printf("Versao: sequencial\n");
    printf("N = %d, D = %d, K = %d\n", n, d, k);
    printf("Iteracoes: %d (%s)\n", iter, changes == 0 ? "convergiu" : "atingiu max_iter");
    printf("SSE final: %.6e\n", sse);
    printf("Tempo do k-means: %.4f s\n", elapsed);
    printf("RESULT seq 1 %.6f %d %.10e\n", elapsed, iter, sse);   /* linha p/ scripts */

    if (argc >= 5) save_centroids(argv[4], cent, k, d);

    free(ds.x); free(cent); free(sums); free(counts); free(labels);
    return 0;
}
