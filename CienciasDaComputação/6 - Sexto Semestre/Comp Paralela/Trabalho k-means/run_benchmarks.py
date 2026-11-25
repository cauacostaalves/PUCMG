#!/usr/bin/env python3
"""Executa as versões compiladas e compara os tempos medianos."""

import argparse
import math
import os
from pathlib import Path
from statistics import median
import subprocess
import sys


THREADS = (1, 2, 4, 8, 16, 32)


def positive_int(value):
    number = int(value)
    if number < 1:
        raise argparse.ArgumentTypeError("informe um inteiro positivo")
    return number


def executable(version):
    suffix = ".exe" if os.name == "nt" else ""
    return Path(f"kmeans_{version}{suffix}").resolve()


def run(version, threads, args):
    env = os.environ.copy()
    if version == "omp":
        env.update(OMP_NUM_THREADS=str(threads), OMP_DYNAMIC="FALSE", OMP_WAIT_POLICY="PASSIVE")
    result = subprocess.run(
        [str(executable(version)), args.dataset, str(args.k), str(args.max_iter)],
        env=env, capture_output=True, text=True,
    )
    if result.returncode:
        raise RuntimeError(result.stderr.strip() or f"processo retornou {result.returncode}")
    lines = [line.split() for line in result.stdout.splitlines() if line.startswith("RESULT ")]
    if len(lines) != 1 or len(lines[0]) != (7 if version == "cuda" else 6):
        raise RuntimeError("saída RESULT ausente ou inválida")
    _, implementation, reported_threads, time, iterations, sse, *loop = lines[0]
    if (implementation, int(reported_threads)) != (version, threads):
        raise RuntimeError(f"configuração esperada: {version}/{threads}; saída: {' '.join(lines[0])}")
    elapsed, iterations, sse = float(time), int(iterations), float(sse)
    loop_time = float(loop[0]) if loop else None
    if not math.isfinite(elapsed) or elapsed <= 0 or not 1 <= iterations <= args.max_iter:
        raise RuntimeError("tempo ou número de iterações inválido")
    if not math.isfinite(sse) or sse < 0:
        raise RuntimeError("SSE inválido")
    if version == "cuda" and (not math.isfinite(loop_time) or loop_time <= 0):
        raise RuntimeError("tempo do laço CUDA inválido")
    return elapsed, iterations, sse, loop_time


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("dataset", nargs="?", default="covtype.txt")
    parser.add_argument("k", nargs="?", type=positive_int, default=40)
    parser.add_argument("max_iter", nargs="?", type=positive_int, default=100)
    parser.add_argument("--repeticoes", type=positive_int, default=3)
    args = parser.parse_args()

    if not Path(args.dataset).is_file():
        parser.error(f"base {args.dataset} ausente. Rode prepare_dataset.py")
    for version in ("seq", "omp"):
        if not executable(version).is_file():
            parser.error(f"compile {executable(version).name} antes de rodar os testes")

    configs = [("seq", 1, "Sequencial")]
    configs += [("omp", t, f"OpenMP {t} thread(s)") for t in THREADS]
    if executable("cuda").is_file():
        configs.append(("cuda", 0, "CUDA (total)"))
    else:
        print("CUDA pendente. Compile o executável para incluir a GPU.", file=sys.stderr)

    samples = {label: [] for _, _, label in configs}
    print(f"Base {args.dataset} | K={args.k} | max_iter={args.max_iter} | "
          f"repetições={args.repeticoes} | CPUs lógicas={os.cpu_count()}", flush=True)
    for repetition in range(1, args.repeticoes + 1):
        for version, threads, label in configs:
            try:
                sample = run(version, threads, args)
            except (OSError, RuntimeError, ValueError) as error:
                sys.exit(f"Falha em {label}: {error}")
            samples[label].append(sample)
            print(f"{repetition}/{args.repeticoes} {label:<21} {sample[0]:9.3f} s "
                  f"(iterações={sample[1]}, SSE={sample[2]:.10e})", flush=True)

    baseline = median(row[0] for row in samples["Sequencial"])
    print(f"\n{'Versão':<22} | {'Mediana (s)':>11} | {'Speedup':>7} | {'Faixa (s)':>16}")
    print("-----------------------+-------------+---------+-----------------")
    for _, _, label in configs:
        rows = samples[label]
        times = [row[0] for row in rows]
        midpoint = median(times)
        print(f"{label:<22} | {midpoint:11.3f} | {baseline / midpoint:6.2f}x | "
              f"{min(times):7.3f} a {max(times):7.3f}")
    if "CUDA (total)" in samples:
        print(f"Mediana do laço CUDA (diagnóstico): "
              f"{median(row[3] for row in samples['CUDA (total)']):.3f} s")

    if baseline < 10:
        print("A mediana sequencial ficou abaixo dos 10 s do enunciado.")
    _, ref_iter, ref_sse, _ = samples["Sequencial"][0]
    for label, rows in samples.items():
        for repetition, (_, iterations, sse, _) in enumerate(rows, 1):
            if iterations != ref_iter or not math.isclose(sse, ref_sse, rel_tol=1e-5, abs_tol=1e-9):
                sys.exit(f"Divergência em {label}, repetição {repetition}: "
                         f"iterações {iterations}/{ref_iter}, SSE {sse:.10e}/{ref_sse:.10e}")
    print(f"Validação: {ref_iter} iterações e SSE consistente com {ref_sse:.10e}.")


if __name__ == "__main__":
    main()
