# Mantém as regras de compilação centralizadas em src/Makefile.
.PHONY: all clean rebuild

all:
	$(MAKE) -C src all

clean:
	$(MAKE) -C src clean

# A limpeza deve terminar antes da compilação, inclusive com make -j.
rebuild:
	$(MAKE) -C src clean
	$(MAKE) -C src all
