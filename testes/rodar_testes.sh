#!/usr/bin/env bash
# Pede uma pasta e roda todos os testes NN_nome.c dela.
# Exemplo: testes/1_lexico
set -u

raiz="$(cd "$(dirname "$0")/.." && pwd)"
interp="$raiz/interpreter"

read -r -p "Pasta dos testes: " pasta
if [[ -z "${pasta:-}" ]]; then
    echo "Nenhuma pasta informada."
    exit 1
fi

pasta="${pasta#"${pasta%%[![:space:]]*}"}"
pasta="${pasta%"${pasta##*[![:space:]]}"}"
pasta="${pasta%/}"

if [[ ! -d "$pasta" && -d "$raiz/testes/$pasta" ]]; then
    pasta="$raiz/testes/$pasta"
elif [[ ! -d "$pasta" && -d "$raiz/$pasta" ]]; then
    pasta="$raiz/$pasta"
fi

if [[ ! -d "$pasta" ]]; then
    echo "Pasta não encontrada: $pasta"
    exit 1
fi

if [[ ! -x "$interp" ]]; then
    make -C "$raiz"
fi

shopt -s nullglob
entradas=("$pasta"/[0-9][0-9]_*.c)
if [[ ${#entradas[@]} -eq 0 ]]; then
    echo "Nenhum teste encontrado em $pasta"
    exit 1
fi

passaram=0
falharam=0

for entrada in "${entradas[@]}"; do
    nome="$(basename "${entrada%.c}")"
    esperado="$pasta/$nome.exp"
    saida="$(mktemp)"
    erros="$(mktemp)"

    if "$interp" --tokens "$entrada" >"$saida" 2>"$erros"; then
        status=0
    else
        status=$?
    fi

    ok=1
    if [[ ! -f "$esperado" ]] || ! cmp -s "$esperado" "$saida"; then
        ok=0
    fi
    if [[ -s "$erros" || "$status" -ne 0 ]]; then
        ok=0
    fi

    if [[ "$ok" -eq 1 ]]; then
        echo "ok   $nome"
        passaram=$((passaram + 1))
    else
        echo "FALHA $nome (saída $status)"
        if [[ -f "$esperado" ]]; then
            diff -u "$esperado" "$saida" || true
        else
            echo "falta $nome.exp"
        fi
        if [[ -s "$erros" ]]; then
            echo "stderr:"
            cat "$erros"
        fi
        falharam=$((falharam + 1))
    fi

    rm -f "$saida" "$erros"
done

echo "$passaram passaram, $falharam falharam"
[[ "$falharam" -eq 0 ]]
