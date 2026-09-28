# Fluxo da raiz do repositório: make, ./interpreter, make clean.
.PHONY: all clean rebuild

all:
	$(MAKE) -C src

clean:
	$(MAKE) -C src clean

rebuild:
	$(MAKE) -C src rebuild
