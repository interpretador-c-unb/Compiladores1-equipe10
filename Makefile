# Mantém as regras de compilação centralizadas em src/Makefile.
.PHONY: all clean rebuild

all:
	$(MAKE) -C src all

clean:
	$(MAKE) -C src clean

rebuild:
	$(MAKE) -C src rebuild
