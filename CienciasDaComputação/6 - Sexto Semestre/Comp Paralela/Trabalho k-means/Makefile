# Compila as 3 versoes. A versao CUDA so e compilada com "make cuda"
# (precisa do nvcc). ARCH=native detecta a GPU da maquina; se der erro,
# informe a arquitetura: make cuda ARCH=sm_75 (sm_60, sm_70, sm_86, sm_89...)
CC     = gcc
CFLAGS = -O2 -std=c11 -Wall
NVCC   = nvcc
ARCH   = native

all: kmeans_seq kmeans_omp

kmeans_seq: kmeans_seq.c kmeans_common.h
	$(CC) $(CFLAGS) -o $@ kmeans_seq.c -lm

kmeans_omp: kmeans_omp.c kmeans_common.h
	$(CC) $(CFLAGS) -fopenmp -o $@ kmeans_omp.c -lm

cuda: kmeans_cuda

kmeans_cuda: kmeans_cuda.cu kmeans_common.h
	$(NVCC) -O2 --fmad=false -arch=$(ARCH) -o $@ kmeans_cuda.cu

clean:
	rm -f kmeans_seq kmeans_omp kmeans_cuda

.PHONY: all cuda clean
