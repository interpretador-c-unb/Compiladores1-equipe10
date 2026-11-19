mdtopdf() {
    local arquivo=""
    local saida=""
    local code_flag=""
    local header_file="$HOME/header_default.tex"
    local mainfont="DejaVu Sans"
    local pdf_engine="lualatex"     # Padrão = LuaLaTeX

    while [[ $# -gt 0 ]]; do
        case "$1" in
            --code)
                code_flag="--highlight-style=breezedark"
                shift
                ;;
            -h|--header)
                if [[ -n "$2" && "$2" != -* ]]; then
                    header_file="$2"
                    shift 2
                else
                    echo "Erro: O parâmetro -h precisa receber o caminho de um arquivo .tex"
                    return 1
                fi
                ;;
           -o|--outpout)
                if [[ -n "$2" && "$2" != -* ]]; then
                    saida="$2"
                    shift 2
                else
                    echo "Erro: Parametro -o inválido! Salvando com o nome original."
                    arquivo="$1"
                fi
                ;;
            --arial)      mainfont="Arial"; shift ;;
            --times)      mainfont="Times New Roman"; shift ;;
            --inter)      mainfont="Inter"; shift ;;
            --dejavu)     mainfont="DejaVu Sans"; shift ;;
            --liberation) mainfont="Liberation Sans"; shift ;;
            --ubuntu)     mainfont="Ubuntu"; shift ;;
            --jetbrains)  mainfont="JetBrains Mono"; shift ;;
            --lualatex)
                pdf_engine="lualatex"
                shift
                ;;
            --xelatex)
                pdf_engine="xelatex"
                shift
                ;;
            
            *)
                if [[ -z "$arquivo" ]]; then
                    arquivo="$1"
                else
                    echo "Aviso: Ignorando argumento extra: $1"
                fi
                shift
                ;;
        esac
    done

    if [[ -z "$arquivo" ]]; then
        echo "Erro: Você precisa informar o caminho do arquivo .md"
        return 1
    fi

    if [[ -z "$saida" ]]; then
            saida="${arquivo%.md}.pdf"
    fi

    local pandoc_vars=()

    if [[ -n "$mainfont" ]]; then
        pandoc_vars+=(-V "mainfont=$mainfont")
    fi

    pandoc "$arquivo" -o "$saida" \
        --from=markdown+emoji \
        --pdf-engine="$pdf_engine" \
        --include-in-header="$header_file" \
        $code_flag \
        "${pandoc_vars[@]}"
}