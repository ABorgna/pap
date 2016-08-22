GCC=g++
#CFLAGS= -O3 -Wall -Wextra -ggdb -std=c++14
CFLAGS= -O0 -Wall -Wextra -ggdb -std=c++14
VALGRIND=valgrind --leak-check=full

DEPEND = makefile

EJERCICIOS = ej1 ej2 ej3 ejercicio4/ej4

SOURCES = $(EJERCICIOS:%=src/%.cpp)
BINARIOS = $(EJERCICIOS:%=$(OUT_DIR)/%)

OUT_DIR = build
EJ4_OUT_DIR = build/ejercicio4

ENTREGABLE=entregable.tar.gz

.PHONY: all $(EJERCICIOS) clean

all: $(OUT_DIR) $(EJ4_OUT_DIR) $(EJERCICIOS)

tar: $(ENTREGABLE)

$(ENTREGABLE): informe
	tar -czf $@ src -C doc informe.pdf

$(EJERCICIOS) : % : build/%

$(BINARIOS): $(OUT_DIR)/% : src/%.cpp $(DEPEND)
	$(GCC) $(CFLAGS) $< -o $@

%.o: %.cpp %.h
	$(GCC) $(CFLAGS) -c -o $@ $<

$(OUT_DIR) $(EJ4_OUT_DIR):
	mkdir -p $@

informe:
	make -C doc/

help:
	@echo "make 		para compilar los ejercicios"
	@echo "make ejX 	para compilar uno solo"
	@echo "make tar 	para generar el archivo entregable"

clean:
	rm -f build/*
	rm -f $(ENTREGABLE)

