#!/usr/bin/env python3
"""Regressões de escopo: python3 testes/testar_escopos.py [executável]."""
from pathlib import Path
import subprocess
import sys
import tempfile

INTERPRETER = Path(sys.argv[1]).resolve() if len(sys.argv) > 1 else Path(__file__).resolve().parents[1] / 'interpreter'
VALID = {
    'sombreamento': ('int x=1; {int x=2; printf("%d,",x);} printf("%d",x);', '2,1'),
    'atribuicao_externa': ('int x=1; {x=3;} printf("%d",x);', '3'),
    'escopos_aninhados': ('int x=1; {int x=2; {int x=3; x=4; printf("%d,",x);} printf("%d,",x);} printf("%d",x);', '4,2,1'),
    'local_no_while': ('int i=0; while(i<3){int local=i; printf("%d",local); i=i+1;}', '012'),
    'blocos_irmaos': ('{int x=2; printf("%d,",x);} {int x=3; printf("%d",x);}', '2,3'),
    'comando_apos_if': ('if(1){int x=2;} int y=3; printf("%d",y);', '3'),
    'texto_local': ('char x=\'a\'; {char x=\'b\'; printf("%c,",x);} printf("%c",x);', 'b,a'),
    'bloco_vazio': ('int x=1; {} printf("%d",x);', '1'),
}
INVALID = {
    'fora_do_bloco': '{int x=1;} printf("%d",x);',
    'fora_do_while': 'int i=0; while(i<1){int local=i; i=i+1;} printf("%d",local);',
    'uso_antes_declaracao': 'printf("%d",x); int x=1;',
    'redeclaracao': 'int x=1; int x=2;',
    'redeclaracao_local': '{int x=1; int x=2;}',
    'erro_apos_if': 'if(1){} printf("%d",x);',
    'expressao_fora_do_bloco': '{int x=1;} x;',
    'atribuicao_fora_do_bloco': '{int x=1;} x=2;',
    'inicializador_fora_do_bloco': '{int x=1;} int y=x;',
}


def run(source, options, directory):
    path = directory / 'entrada.c'
    path.write_text(source)
    return subprocess.run([str(INTERPRETER), *options, str(path)], capture_output=True, text=True, timeout=5)


with tempfile.TemporaryDirectory(prefix='testes-escopos-') as directory:
    for options in ([], ['-O']):
        for name, (source, expected) in VALID.items():
            result = run(source, options, Path(directory))
            assert result.returncode == 0, (name, options, result.stdout, result.stderr)
            output = result.stdout.split('--- Executando o Programa (Runtime) ---\n', 1)[1]
            output = output.split('\n--- Estado Final', 1)[0]
            assert output == expected, (name, options, output, expected)
        for name, source in INVALID.items():
            result = run(source, options, Path(directory))
            assert result.returncode == 1, (name, options, result.stdout, result.stderr)
            assert '[ERRO SEMÂNTICO]' in result.stderr, (name, result.stderr)
            assert '--- Executando o Programa' not in result.stdout, name
print(f'{2 * (len(VALID) + len(INVALID))} verificações de escopo passaram (com e sem otimização).')
