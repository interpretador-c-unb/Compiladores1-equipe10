# Interpretador C

Projeto de desenvolvimento de um interpretador para um subconjunto da linguagem C, utilizando as ferramentas **Flex** (análise léxica) e **Bison** (análise sintática), com execução baseada em **Árvore Sintática Abstrata (AST)**.

**Universidade de Brasília | FCTE**

| Informações | Detalhes |
| :--- | :--- |
| **Disciplina** | Compiladores 1 |
| **Professor** | Dr. Sergio Antônio Andrade de Freitas |
| **Semestre** | 2º / 2026 |
| **Grupo** | 10 |

---

## Integrantes

| Nome | Matrícula |
| :--- | :--- |
| Bernardo Campos Caxito Silveira | 232024966 |
| Danielly Mendes do Espírito Santo | 242015817 |
| Nicole Jovita Fernandes | 241012347 |
| Paulo Nina Denczuk de Alencar | 242004920 |
| Rafael Souto Lopes Laube | 211062428 |

---

## Metodologia

A equipe adota a metodologia **SCRUM/XP**, organizada em *sprints* semanais de quarta a quarta-feira. O desenvolvimento incremental prioriza commits frequentes, testes automatizados e integração contínua.

---

## Documentação do Projeto

A documentação detalhada exigida para o projeto encontra-se organizada na pasta `documentos/`:

* **[Estrutura do Projeto](documentos/README.md#estrutura-do-projeto)**: Arquitetura de pastas e módulos do sistema.
* **[Decisões Técnicas](documentos/README.md#decisoes-tecnicas)**: Especificação das escolhas para gramática, AST, tabela de símbolos e interpretação.
* **[Planejamento das Sprints](documentos/sprints.md)**: Detalhamento das metas, cronograma e entregas de cada sprint.
* **[Problemas Encontrados e Soluções](documentos/README.md#problemas-encontrados-e-solucoes-adotadas)**: Registro de desafios técnicos e correções efetuadas ao longo do desenvolvimento.

---

## Como Compilar e Executar

!!! note "Pré-requisitos"
    Certifique-se de ter os utilitários `gcc`, `flex`, `bison` e `make` instalados no seu ambiente.

```bash
# 1. Clonar o repositório
git clone [https://github.com/Danielly-Mendes/Compiladores1-equipe10.git](https://github.com/Danielly-Mendes/Compiladores1-equipe10.git)
cd Compiladores1-equipe10

# 2. Compilar o projeto
make

# 3. Executar um programa de teste
./interpreter testes/exemplo.c

# 4. Limpar arquivos de compilação
make clean