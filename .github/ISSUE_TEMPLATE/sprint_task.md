<!-- Título sugerido: [Módulo/Sprint] Descrição sucinta da atividade -->
<!-- Exemplo: [PARSER/Sprint-3] Implementar regras para declaração de variáveis -->
<!-- Exemplo: [DOC/Sprint-5] Documentar a estrutura da AST no MkDocs -->

## 🎯 Descrição da Atividade
Descreva de forma clara o que precisa ser feito e qual o objetivo desta tarefa no projeto.

---

## 📌 Escopo & Módulo
- **Sprint:** Sprint X
- **Frente:** [ ] Documentação (DOC)  |  [ ] Desenvolvimento (DEV)
- **Responsável principal:** @usuario
- **Revisor / Par (Rodízio):** @usuario

---

## 🔗 Dependências & Bloqueios (Rastreabilidade)
- **Issues Bloqueantes (Prerequisitos):**
  - [ ] Bloqueada por # <!-- Coloque o número da Issue que precisa ser concluída antes desta (ex: #12) -->
  - [ ] Nenhuma dependência prévia
- **Arquivos/Módulos Envolvidos:** Ex: `src/parser.y`, `include/ast.h`, `documentos/decisoes-tecnicas.md`
- **Ferramentas / Bibliotecas Utilizadas:** Ex: `Flex 2.6`, `Bison 3.8`, `GCC`, `MkDocs Material`, `Valgrind`

---

## ✅ Lista de Critérios de Aceite (Checklist)
Subdivida a atividade em passos mensuráveis:
- [ ] Passo 1: Implementação ou escrita inicial do conteúdo
- [ ] Passo 2: Teste/Validação executada com sucesso
- [ ] Passo 3: [Obrigatório para DOC] Atualização do status da Sprint no GitHub Pages
- [ ] Passo 4: Código/Documentação revisada e aprovada pelo par da Sprint

---

## 🧪 Como Testar / Validar
Comandos ou instruções para verificar que a tarefa foi concluída sem regressões:
```bash
# Exemplo para tarefas DEV:
make && ./interpreter testes/validos/exemplo.c

# Exemplo para tarefas DOC:
mkdocs serve