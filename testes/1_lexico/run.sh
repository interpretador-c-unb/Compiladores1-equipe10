#!/usr/bin/env bash
# Roda os 30 casos do analisador léxico.
# Cada NN_nome.c é a entrada; NN_nome.exp é a lista linha:coluna TOKEN.
set -u

aqui="$(cd "$(dirname "$0")" && pwd)"
raiz="$(cd "$aqui/../.." && pwd)"
interp="$raiz/interpreter"

if [[ ! -x "$interp" ]]; then
    make -C "$raiz"
fi

passaram=0
falharam=0

for entrada in "$aqui"/[0-9][0-9]_*.c; do
    nome="$(basename "${entrada%.c}")"
    esperado="$aqui/$nome.exp"
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
