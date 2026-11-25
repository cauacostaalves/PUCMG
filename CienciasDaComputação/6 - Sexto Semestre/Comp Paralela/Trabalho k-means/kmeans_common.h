/*
 * kmeans_common.h
 * ----------------------------------------------------------------------------
 * Funcoes compartilhadas pelas 3 versoes (sequencial, OpenMP e CUDA):
 *   - leitura do dataset (texto: 1a linha "N D", depois N linhas com D valores)
 *   - inicializacao deterministica dos centroides
 *   - calculo do SSE (soma dos quadrados das distancias), usado para conferir
 *     se as versoes paralelas chegam ao mesmo resultado da sequencial
 *   - relogio de parede e escrita dos centroides
 *
 * Nada aqui e paralelizado: e so infraestrutura. Fica fora da medicao de tempo.
 * Escrito em C compativel com C++ (casts explicitos) para poder ser incluido
 * tambem pelo arquivo .cu (nvcc compila como C++).
 * ----------------------------------------------------------------------------
 */
#ifndef KMEANS_COMMON_H
#define KMEANS_COMMON_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <float.h>
#include <limits.h>
#include <errno.h>
#include <time.h>
#ifdef _WIN32
#include <windows.h>
#endif

typedef struct {
    int n;      /* numero de pontos               */
    int d;      /* numero de dimensoes (features) */
    float *x;   /* pontos em ordem "row-major": x[i*d + j] = feature j do ponto i */
} Dataset;

static void *xmalloc(size_t bytes) {
    void *p = malloc(bytes);
    if (!p) { fprintf(stderr, "Erro: sem memoria (%zu bytes)\n", bytes); exit(1); }
    return p;
}

/* Le o arquivo inteiro para a memoria e converte com strtof (bem mais rapido
 * que fscanf para dezenas de milhoes de numeros). */
static Dataset load_dataset(const char *path) {
    FILE *f = fopen(path, "rb");
    if (!f) { perror(path); exit(1); }
    fseek(f, 0, SEEK_END);
    long size = ftell(f);
    fseek(f, 0, SEEK_SET);
    char *buf = (char *)xmalloc((size_t)size + 1);
    if (fread(buf, 1, (size_t)size, f) != (size_t)size) {
        fprintf(stderr, "Erro lendo %s\n", path); exit(1);
    }
    buf[size] = '\0';
    fclose(f);

    Dataset ds;
    char *p = buf, *end;
    ds.n = (int)strtol(p, &end, 10); p = end;
    ds.d = (int)strtol(p, &end, 10); p = end;
    if (ds.n <= 0 || ds.d <= 0) {
        fprintf(stderr, "Cabecalho invalido em %s (esperado: N D)\n", path); exit(1);
    }
    size_t total = (size_t)ds.n * ds.d;
    ds.x = (float *)xmalloc(total * sizeof(float));
    for (size_t i = 0; i < total; i++) {
        ds.x[i] = strtof(p, &end);
        if (end == p) {
            fprintf(stderr, "Arquivo terminou antes do esperado (valor %zu de %zu)\n", i, total);
            exit(1);
        }
        p = end;
    }
    free(buf);
    return ds;
}

/* Centroides iniciais: K pontos igualmente espacados no arquivo.
 * E deterministico, entao as 3 versoes partem exatamente do mesmo estado. */
static inline void init_centroids(const Dataset *ds, int k, float *cent) {
    for (int c = 0; c < k; c++) {
        size_t idx = (size_t)c * ds->n / k;
        memcpy(&cent[(size_t)c * ds->d], &ds->x[idx * ds->d], ds->d * sizeof(float));
    }
}

/* SSE = soma, para todo ponto, da distancia ao quadrado ate seu centroide. */
static double compute_sse(const Dataset *ds, const float *cent, const int *labels) {
    double sse = 0.0;
    for (int i = 0; i < ds->n; i++) {
        const float *pt = &ds->x[(size_t)i * ds->d];
        const float *c  = &cent[(size_t)labels[i] * ds->d];
        for (int j = 0; j < ds->d; j++) {
            double diff = (double)pt[j] - c[j];
            sse += diff * diff;
        }
    }
    return sse;
}

static inline double wall_time(void) {
#ifdef _WIN32
    LARGE_INTEGER counter, frequency;
    QueryPerformanceCounter(&counter);
    QueryPerformanceFrequency(&frequency);
    return (double)counter.QuadPart / (double)frequency.QuadPart;
#else
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return ts.tv_sec + ts.tv_nsec * 1e-9;
#endif
}

static inline void save_centroids(const char *path, const float *cent, int k, int d) {
    FILE *f = fopen(path, "w");
    if (!f) { perror(path); return; }
    for (int c = 0; c < k; c++) {
        for (int j = 0; j < d; j++)
            fprintf(f, "%.6f%c", cent[(size_t)c * d + j], j == d - 1 ? '\n' : ' ');
    }
    fclose(f);
}

static void usage(const char *prog) {
    fprintf(stderr, "Uso: %s <dataset.txt> <K> <max_iter> [centroides_saida.txt]\n", prog);
    exit(1);
}

static int positive_arg(const char *value, const char *name) {
    char *end;
    errno = 0;
    long parsed = strtol(value, &end, 10);
    if (end == value || *end != '\0' || errno == ERANGE || parsed < 1 || parsed > INT_MAX) {
        fprintf(stderr, "%s deve ser um inteiro positivo (recebido: %s)\n", name, value);
        exit(1);
    }
    return (int)parsed;
}

static int centroid_size(const Dataset *ds, int k) {
    if (k > ds->n || k > INT_MAX / ds->d) {
        fprintf(stderr, "K invalido para o numero de pontos ou dimensoes\n");
        exit(1);
    }
    return k * ds->d;
}

#endif
