#!/usr/bin/env bash

set -u

ROOT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")/../.." && pwd)"
TEST_DIR="$ROOT_DIR/testes/validos"
EXECUTABLE="$ROOT_DIR/interpreter"
VALGRIND="${VALGRIND:-valgrind}"

if ! command -v "$VALGRIND" >/dev/null 2>&1; then
    echo "Erro: Valgrind não foi encontrado no PATH." >&2
    echo "Instale-o com: sudo apt-get install valgrind" >&2
    exit 2
fi

echo "== Build instrumentado para Valgrind =="
make -C "$ROOT_DIR/src" rebuild \
    CFLAGS="-Wall -Wextra -std=gnu11 -g"

status=0
for source in "$TEST_DIR"/*.c; do
    [ -e "$source" ] || continue
    echo
echo "== Valgrind: ${source##*/} =="
    "$VALGRIND" \
        --leak-check=full \
        --show-leak-kinds=all \
        --track-origins=yes \
        --errors-for-leak-kinds=definite,indirect \
        --error-exitcode=99 \
        "$EXECUTABLE" "$source"
    result=$?
    if [ "$result" -eq 99 ]; then
        status=1
        echo "FALHA DE MEMÓRIA: ${source##*/}" >&2
    fi
done

if [ "$status" -eq 0 ]; then
    echo
echo "Valgrind: todos os casos válidos foram aprovados."
else
    echo
echo "Valgrind: foram encontrados erros em pelo menos um caso." >&2
fi

exit "$status"