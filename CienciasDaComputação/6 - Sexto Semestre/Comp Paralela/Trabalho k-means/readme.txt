PROJETO 01 - IMPLEMENTAÇÃO PARALELA DO ALGORITMO K-MEANS
=========================================================
Integrantes: ______________________________________________

Versões entregues:
  kmeans_seq.c     -> versão sequencial (referência)
  kmeans_omp.c     -> versão paralela para CPU multicore (OpenMP)
  kmeans_cuda.cu   -> versão paralela para GPU (CUDA)
  kmeans_common.h  -> leitura do arquivo, inicialização e relógio (comum às 3)
  prepare_dataset.py -> baixa e prepara a base de dados real (Covertype)
  run_benchmarks.py -> mede as versões compiladas e compara os resultados
  run_benchmarks.sh -> compila as versões de CPU no Linux e chama o benchmark
  Makefile


1. A APLICAÇÃO
--------------
O k-means agrupa N pontos de D dimensões em K grupos (clusters). Usamos o
algoritmo de Lloyd:

  1. Inicialização: escolhe K pontos como centroides iniciais. Usamos K
     pontos igualmente espaçados no arquivo (posições 0, N/K, 2N/K, ...),
     o que é determinístico. As três versões partem dos mesmos centroides.
  2. Atribuição: cada ponto vai para o cluster do centroide mais próximo
     (distância euclidiana ao quadrado; não precisa tirar a raiz porque ela
     não muda qual distância é a menor).
  3. Atualização: cada centroide passa a ser a média dos pontos do cluster.
     Cluster vazio mantém o centroide anterior.
  4. Repete 2 e 3 até nenhum ponto trocar de cluster (convergiu) ou até
     atingir max_iter.

Custo por iteração: O(N·K·D) no passo 2 e só O(N·D + K·D) no passo 3.
Ou seja, o passo de atribuição é o gargalo e é o que foi paralelizado.

2. BASE DE DADOS
----------------
Covertype (UCI Machine Learning Repository)
https://archive.ics.uci.edu/dataset/31/covertype

Dados reais de 581.012 áreas de 30x30 m em florestas do Colorado (EUA), com
54 atributos cartográficos (elevação, declive, distância até água e
estradas, sombreamento, área selvagem, tipo de solo). Removemos a coluna da
classe (k-means não é supervisionado) e normalizamos cada coluna por z-score,
para que atributos de escala grande (elevação em metros) não dominem a
distância.

Referência da base (CC BY 4.0): Blackard, J. (1998). Covertype [Dataset].
UCI Machine Learning Repository. https://doi.org/10.24432/C50K5N
O benchmark usa todos os pontos, K = 40 e limite de 30 iterações.
Na máquina de teste, essa configuração atende ao mínimo de 10 s na versão
sequencial e reduz a duração da coleta. Em outra máquina, verifique o tempo
sequencial antes de medir as versões paralelas.


3. COMO COMPILAR E EXECUTAR
---------------------------
Requisitos: GCC com suporte a reduções de array do OpenMP 4.5, Python 3 e,
para CUDA, NVIDIA CUDA Toolkit (nvcc), compilador C++ compatível e GPU NVIDIA.
O código usa atomicAdd em double, que exige compute capability >= 6.0. A
arquitetura da GPU também deve ser suportada pela versão do CUDA Toolkit.

Linux (PARCODE ou computador pessoal)

  # 1) preparar a base (gera covtype.txt, aproximadamente 280 MB)
  python3 prepare_dataset.py
  #    Sem internet no servidor? Baixe covtype.data.gz no seu computador,
  #    copie para esta pasta e rode o script: ele usa o arquivo local.

  # 2) compilar
  make                 # gera kmeans_seq e kmeans_omp
  make cuda            # gera kmeans_cuda (ARCH=native detecta a GPU)
  make cuda ARCH=sm_75 # exemplo para uma RTX 2060, se "native" falhar

  #    ou manualmente:
  gcc -O2 -std=c11 -o kmeans_seq kmeans_seq.c -lm
  gcc -O2 -std=c11 -fopenmp -o kmeans_omp kmeans_omp.c -lm
  nvcc -O2 --fmad=false -arch=sm_75 -o kmeans_cuda kmeans_cuda.cu

  # 3) executar:  <programa> <dataset> <K> <max_iter> [arquivo_centroides]
  ./kmeans_seq covtype.txt 40 30
  ./kmeans_omp covtype.txt 40 30
  ./kmeans_cuda covtype.txt 40 30

Windows (GCC do MSYS2 UCRT64 no PATH e terminal x64 do Visual Studio aberto
para o nvcc localizar o MSVC)

  python prepare_dataset.py
  gcc -O2 -std=c11 -Wall -o kmeans_seq.exe kmeans_seq.c -lm
  gcc -O2 -std=c11 -Wall -fopenmp -o kmeans_omp.exe kmeans_omp.c -lm
  nvcc -O2 --fmad=false -arch=sm_75 -o kmeans_cuda.exe kmeans_cuda.cu

Para uma execução individual no PowerShell:

  .\kmeans_seq.exe covtype.txt 40 30
  .\kmeans_omp.exe covtype.txt 40 30
  .\kmeans_cuda.exe covtype.txt 40 30

Nos testes manuais, altere o argumento de omp_set_num_threads no arquivo
kmeans_omp.c e recompile para cada número de threads. omp_set_dynamic(0)
desativa o ajuste automático da equipe, e a saída RESULT omp informa
quantas threads foram usadas.

Para automatizar as medições, retire a chamada omp_set_num_threads e
recompile o OpenMP. O script controlará o tamanho da equipe pelo ambiente.

  Linux    bash run_benchmarks.sh covtype.txt 40 30 --repeticoes 3
  Windows  python run_benchmarks.py covtype.txt 40 30 --repeticoes 3

O tempo de CPU cobre o laço de iterações. O tempo total de CUDA inclui a
transposição, as transferências entre CPU e GPU e o laço. A leitura, a
escolha dos centroides iniciais, as alocações explícitas e a conferência
dos resultados ficam fora da medição, assim como a criação do contexto CUDA.
O tempo por eventos CUDA é um diagnóstico do laço e inclui os resets, os
kernels e a sincronização para copiar o contador de trocas a cada iteração.
O speedup é o tempo sequencial dividido pelo tempo da versão medida. Quando
há várias repetições, use a mediana dos tempos de cada configuração.
O script fixa OMP_DYNAMIC=FALSE e OMP_WAIT_POLICY=PASSIVE nas rodadas OpenMP
e verifica se a equipe criada tem o número solicitado de threads.
O nvcc usa --fmad=false para calcular as distâncias com multiplicação e
adição separadas, como no GCC com os flags acima.


4. O QUE FOI PARALELIZADO
-------------------------
4.1 OpenMP (kmeans_omp.c) - mudanças marcadas no código com [OMP-n]

  - O laço "for i" sobre os N pontos (atribuição + acúmulo das somas) virou
    "#pragma omp for schedule(static)" dentro de uma região paralela. O
    custo de calcular as distâncias é o mesmo por ponto (K·D), o que permite
    distribuir blocos de pontos sem escalonamento dinâmico.
  - labels[i]: cada ponto é tratado por uma única thread -> sem corrida.
  - changes (contador de trocas): reduction(+:changes).
  - sums[K·D] e counts[K]: várias threads podem somar no mesmo cluster.
    Usar atomic ou critical serializaria N·D somas. Usamos redução de
    ARRAY do OpenMP 4.5: reduction(+:sums[:kd], counts[:k]). Cada thread
    acumula em uma cópia privada e o runtime junta as cópias no fim do laço
    (custo extra de P·K·D somas, desprezível).
  - A atualização dos centroides (K·D operações) ficou sequencial: é tão
    pequena que criar/sincronizar threads custaria mais do que ganharia.

4.2 CUDA (kmeans_cuda.cu) - mudanças marcadas no código com [CUDA-n]

  - Os pontos são copiados para a GPU uma única vez, antes do laço. Dentro
    do laço só volta 1 inteiro por iteração (o contador de trocas, usado no
    critério de parada).
  - Os pontos vão TRANSPOSTOS (xT[j·N + i]): threads vizinhas lendo a mesma
    feature acessam endereços consecutivos -> acesso coalescido.
  - Kernel assign_and_accumulate: 1 thread por ponto (blocos de 256).
      * os K·D centroides são carregados na memória compartilhada, pois
        todas as threads do bloco os leem várias vezes;
      * as somas por cluster são feitas em 2 níveis: atomicAdd na memória
        compartilhada do bloco e depois 1 atomicAdd por posição na memória
        global. Cada bloco envia apenas as somas parciais não nulas, com
        limite de K·D atômicos globais de soma por bloco;
      * o contador de trocas é somado no bloco e depois no global.
  - Kernel update_centroids: 1 thread por coordenada (K·D threads) calcula
    a média. Fica na GPU para não precisar trazer sums/counts para a CPU.
  - O benchmark usa blocos fixos de 256 threads.


5. RESULTADOS
-------------

5.1 Ambiente do teste

  Data da medição       24/11/2026
  Máquina e sistema     PC pessoal, Gigabyte B560M AORUS ELITE
                        Windows 11 Pro (build 26300)
  CPU e núcleos         Intel Core i5-11400F a 2,60 GHz (6 físicos / 12 lógicos)
  GPU                   NVIDIA GeForce RTX 2060 (driver 610.88)
  Compiladores          GCC 16.1.0 e nvcc 13.3.33 com Visual Studio 2022
  Base                  Covertype (581.012 pontos, 54 atributos)
  Parâmetros medidos    K = 40, max_iter = 30
  Repetições            1 por configuração

5.2 Checklist de execução no Windows (cmd.exe)

  [x] Sequencial. Compile e rode primeiro para verificar o mínimo de 10 s.

      gcc -O2 -std=c11 -Wall -o kmeans_seq.exe kmeans_seq.c -lm
      kmeans_seq.exe covtype.txt 40 30

      Use o mesmo K, max_iter e arquivo de entrada em todas as
      configurações da tabela.

  [x] OpenMP. Ajuste omp_set_num_threads no fonte, salve e recompile
      a cada configuração. Confira a contagem na linha RESULT omp.

      gcc -O2 -std=c11 -Wall -fopenmp -o kmeans_omp.exe kmeans_omp.c -lm && .\kmeans_omp.exe covtype.txt 40 30

      [x] 1 thread    [x] 2 threads  [x] 4 threads   [x] 8 threads   [x] 16 threads  [x] 32 threads

  [x] CUDA. Compilação para a RTX 2060 confirmada. Se editar o fonte,
      recompile com o comando da seção 3. No Toolkit portátil, informe
      o caminho completo do nvcc.
  [x] CUDA. Execute com os mesmos parâmetros das versões de CPU.

      kmeans_cuda.exe covtype.txt 40 30

5.3 Tabela de medições

  Guarde a linha RESULT de cada execução. Para o OpenMP, confira o número
  de threads informado. Se fizer três repetições, anote a mediana dos
  tempos. Calcule o speedup dividindo o tempo sequencial pelo da versão.
  Para o CUDA, use o tempo total, que inclui transposição e transferências.

  Versão             | Tempo (s) | Speedup | Iterações
  -------------------+-----------+---------+----------
  Sequencial         | 18,113694 |  1,00x  |        30
  OpenMP  1 thread   | 20,148328 |  0,90x  |        30
  OpenMP  2 threads  | 11,845973 |  1,53x  |        30
  OpenMP  4 threads  |  7,770411 |  2,33x  |        30
  OpenMP  8 threads  |  4,779484 |  3,79x  |        30
  OpenMP 16 threads  |  4,407301 |  4,11x  |        30
  OpenMP 32 threads  |  3,984387 |  4,55x  |        30
  CUDA (tempo total) |  0,492608 | 36,77x  |        30

  Tempo do laço CUDA (diagnóstico)  0,433859 s

5.4 Conferência e análise

  [x] Confirmar que o sequencial levou pelo menos 10 s.
  [x] Conferir o número de iterações do OpenMP e do CUDA.
  [x] Copiar os tempos finais para os comentários no início dos fontes.

  Todas as versões atingiram o limite de 30 iterações.

  O OpenMP com 1 thread levou 20,148328 s, frente a 18,113694 s do
  sequencial. Com 2 threads já houve aceleração. O tempo caiu à medida que
  a equipe cresceu, com ganhos menores a partir de 8 threads. O melhor
  resultado observado foi 3,984387 s com 32 threads, um speedup de 4,55x.
  As equipes de 16 e 32 threads compartilham os 12 processadores lógicos,
  e cada configuração foi medida uma vez.

  Na RTX 2060, com blocos de 256 threads, o CUDA registrou 36,77x de speedup
  em relação ao sequencial e 8,09x em relação ao OpenMP com 32 threads.
