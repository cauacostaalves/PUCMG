#!/usr/bin/env python3
"""
Prepara a base de dados real usada no trabalho: Covertype (UCI Machine Learning
Repository) - 581.012 amostras de areas florestais do Colorado (EUA), 54
atributos (elevacao, declive, distancias a agua/estradas, sombreamento, tipo
de solo, area selvagem...).

O script:
  1) baixa covtype.data.gz da UCI (ou usa um arquivo ja baixado, se existir);
  2) descarta a ultima coluna (o rotulo da classe - o k-means e nao supervisionado);
  3) normaliza cada coluna com z-score (media 0, desvio 1), para que atributos
     com escala grande (ex.: elevacao em metros) nao dominem a distancia;
  4) grava covtype.txt no formato lido pelo programa em C:
        primeira linha: N D
        depois: N linhas com D numeros separados por espaco

So usa a biblioteca padrao do Python (nao precisa de numpy).
Uso: python3 prepare_dataset.py [saida=covtype.txt]
"""
import gzip
import math
import os
import sys
import urllib.request

URLS = [
    "https://archive.ics.uci.edu/ml/machine-learning-databases/covtype/covtype.data.gz",
    "https://archive.ics.uci.edu/static/public/31/covertype.zip",
]
RAW = "covtype.data.gz"


def download():
    if os.path.exists(RAW) or os.path.exists("covtype.data"):
        return
    for url in URLS:
        try:
            print(f"Baixando {url} ...")
            data = urllib.request.urlopen(url, timeout=120).read()
            if url.endswith(".zip"):
                import io
                import zipfile
                with zipfile.ZipFile(io.BytesIO(data)) as z:
                    name = [m for m in z.namelist() if m.endswith("covtype.data.gz")][0]
                    data = z.read(name)
            with open(RAW, "wb") as f:
                f.write(data)
            return
        except Exception as e:  # tenta a proxima URL
            print(f"  falhou: {e}")
    sys.exit("Nao consegui baixar. Baixe covtype.data.gz manualmente em "
             "https://archive.ics.uci.edu/dataset/31/covertype e coloque nesta pasta.")


def read_rows():
    opener = gzip.open if os.path.exists(RAW) else open
    path = RAW if os.path.exists(RAW) else "covtype.data"
    with opener(path, "rt") as f:
        for line in f:
            line = line.strip()
            if line:
                yield [float(v) for v in line.split(",")[:-1]]  # sem o rotulo


def main():
    out = sys.argv[1] if len(sys.argv) > 1 else "covtype.txt"
    download()

    # 1a passada: media e desvio padrao de cada coluna (sem guardar tudo na memoria)
    n, s1, s2 = 0, None, None
    for r in read_rows():
        if s1 is None:
            s1, s2 = [0.0] * len(r), [0.0] * len(r)
        for j, v in enumerate(r):
            s1[j] += v
            s2[j] += v * v
        n += 1
    d = len(s1)
    mean = [s1[j] / n for j in range(d)]
    std = [math.sqrt(max(s2[j] / n - mean[j] ** 2, 0.0)) or 1.0 for j in range(d)]
    print(f"Lidas {n} linhas x {d} colunas")

    # 2a passada: grava normalizado
    with open(out, "w") as f:
        f.write(f"{n} {d}\n")
        for r in read_rows():
            f.write(" ".join(f"{(r[j] - mean[j]) / std[j]:.5f}" for j in range(d)))
            f.write("\n")
    print(f"Gravado {out}")


if __name__ == "__main__":
    main()
