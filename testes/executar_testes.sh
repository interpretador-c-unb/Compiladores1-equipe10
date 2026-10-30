#!/usr/bin/env bash
# Roda testes/validos e testes/invalidos. Uso: ./testes/executar_testes.sh
set -u

raiz="$(cd "$(dirname "$0")/.." && pwd)"
interp="$raiz/interpreter"
[[ -x "$interp" ]] || make -C "$raiz"

shopt -s nullglob
passaram=0
falharam=0

for f in "$raiz"/testes/validos/*.c; do
    nome="$(basename "$f")"
    esperado="${f%.c}.exp"
    saida="$(mktemp)"; erros="$(mktemp)"

    "$interp" "$f" >"$saida" 2>"$erros"; status=$?

    ok=1
    [[ $status -eq 0 ]] || ok=0
    grep -qiE "Erro|AVISO" "$erros" && ok=0
    if [[ -f "$esperado" ]] && ! cmp -s "$esperado" "$saida"; then ok=0; fi

    if [[ $ok -eq 1 ]]; then
        echo "ok    validos/$nome"; passaram=$((passaram + 1))
    else
        echo "FALHA validos/$nome (saída $status)"
        [[ -f "$esperado" ]] && diff -u "$esperado" "$saida"
        cat "$erros"
        falharam=$((falharam + 1))
    fi
    rm -f "$saida" "$erros"
done

for f in "$raiz"/testes/invalidos/*.c; do
    nome="$(basename "$f")"
    erros="$(mktemp)"

    "$interp" "$f" >/dev/null 2>"$erros"; status=$?

    if [[ $status -lt 128 ]] && { [[ $status -ne 0 ]] || grep -qiE "Erro|AVISO" "$erros"; }; then
        echo "ok    invalidos/$nome"; passaram=$((passaram + 1))
    else
        echo "FALHA invalidos/$nome deveria ser rejeitado (saída $status)"
        falharam=$((falharam + 1))
    fi
    rm -f "$erros"
done

echo "$passaram passaram, $falharam falharam"
[[ $falharam -eq 0 ]]