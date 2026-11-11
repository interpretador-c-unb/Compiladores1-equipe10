#!/usr/bin/env python3
"""Mede a redução de nós da AST causada pelo constant folding.

Uso, a partir da raiz do repositório:
    python3 documentos/scripts/medir_reducao_ast.py

Os resultados são gravados em documentos/gqm/sprint10/.
"""

from __future__ import annotations

import csv
import re
import subprocess
from pathlib import Path

import matplotlib.pyplot as plt

ROOT = Path(__file__).resolve().parents[2]
INTERPRETER = ROOT / "interpreter"
OUTPUT = ROOT / "documentos" / "gqm" / "sprint10"

CASES = [
    ("expressoes_constantes", ROOT / "testes" / "validos" / "expressoes_constantes.c"),
    ("declaracoes_e_expressoes", ROOT / "testes" / "validos" / "02_declaracoes_e_expressoes.c"),
    ("precedencia_com_variaveis", ROOT / "testes" / "validos" / "07_precedencia.c"),
]

NODE_LINE = re.compile(
    r"^\s*(?:Declaration|Assignment|BinaryExpr|Var|Num|String|If|While|Block|Program)"
)


def build() -> None:
    result = subprocess.run(
        ["make", "-C", "src", "rebuild"],
        cwd=ROOT,
        text=True,
        capture_output=True,
    )
    if result.returncode != 0:
        raise SystemExit(result.stderr or result.stdout)


def run_ast(source: Path, optimized: bool) -> list[str]:
    command = [str(INTERPRETER)]
    if optimized:
        command.append("--optimize")
    command.extend(["--print-ast", str(source)])
    result = subprocess.run(command, cwd=ROOT, text=True, capture_output=True)
    if result.returncode != 0:
        raise RuntimeError(f"Falha ao executar {source}: {result.stderr}")

    start_marker = "--- Árvore Sintática Abstrata (AST) ---"
    end_marker = "--- Iniciando Análise Semântica ---"
    if start_marker not in result.stdout or end_marker not in result.stdout:
        raise RuntimeError(f"Saída de AST não encontrada para {source}")

    section = result.stdout.split(start_marker, 1)[1].split(end_marker, 1)[0]
    return [line for line in section.splitlines() if NODE_LINE.match(line)]


def main() -> None:
    build()
    OUTPUT.mkdir(parents=True, exist_ok=True)
    rows = []

    for name, source in CASES:
        before = run_ast(source, optimized=False)
        after = run_ast(source, optimized=True)
        before_count = len(before)
        after_count = len(after)
        removed = before_count - after_count
        reduction = (removed / before_count * 100) if before_count else 0.0
        rows.append(
            {
                "caso": name,
                "arquivo": str(source.relative_to(ROOT)),
                "nos_antes": before_count,
                "nos_depois": after_count,
                "nos_removidos": removed,
                "reducao_percentual": f"{reduction:.2f}",
            }
        )

    csv_path = OUTPUT / "reducao_ast.csv"
    with csv_path.open("w", newline="", encoding="utf-8") as handle:
        writer = csv.DictWriter(handle, fieldnames=rows[0].keys(), lineterminator="\n")
        writer.writeheader()
        writer.writerows(rows)

    labels = [row["caso"] for row in rows]
    before_values = [int(row["nos_antes"]) for row in rows]
    after_values = [int(row["nos_depois"]) for row in rows]
    reduction_values = [float(row["reducao_percentual"]) for row in rows]

    plt.style.use("seaborn-v0_8-whitegrid")
    fig, ax = plt.subplots(figsize=(10, 5.5))
    positions = list(range(len(labels)))
    width = 0.36
    ax.bar([p - width / 2 for p in positions], before_values, width, label="Antes", color="#64748b")
    ax.bar([p + width / 2 for p in positions], after_values, width, label="Depois", color="#2563eb")
    ax.set_title("Nós da AST antes e depois do Constant Folding")
    ax.set_ylabel("Quantidade de nós")
    ax.set_xticks(positions, labels, rotation=15, ha="right")
    ax.legend()
    fig.tight_layout()
    fig.savefig(OUTPUT / "nos_antes_depois.png", dpi=180)
    plt.close(fig)

    fig, ax = plt.subplots(figsize=(10, 5.5))
    bars = ax.bar(labels, reduction_values, color="#16a34a")
    ax.set_title("Redução percentual de nós na AST")
    ax.set_ylabel("Redução (%)")
    ax.set_ylim(0, max(100, max(reduction_values) + 15))
    ax.tick_params(axis="x", rotation=15)
    for bar, value in zip(bars, reduction_values):
        ax.text(bar.get_x() + bar.get_width() / 2, value + 1, f"{value:.2f}%", ha="center")
    fig.tight_layout()
    fig.savefig(OUTPUT / "reducao_percentual.png", dpi=180)
    plt.close(fig)

    print(f"Dados: {csv_path}")
    print(f"Gráfico: {OUTPUT / 'nos_antes_depois.png'}")
    print(f"Gráfico: {OUTPUT / 'reducao_percentual.png'}")
    for row in rows:
        print(row)


if __name__ == "__main__":
    main()
