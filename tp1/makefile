GCC=g++
#CFLAGS= -O3 -Wall -Wextra -ggdb -std=c++11
CFLAGS= -O0 -Wall -Wextra -ggdb -std=c++11
VALGRIND=valgrind --leak-check=full

# Variables para la entrega
GROUP = 12
SEED = 64861003733618093


EJERCICIOS = ej1 ej2 ej3 ejercicio4/ej4

SOURCES = $(EJERCICIOS:%=src/%.cpp)
BINARIOS = $(EJERCICIOS:%=$(OUT_DIR)/%)

GENERATOR = groupNumberGenerator
GENERATOR_BIN = $(GENERATOR:%=$(OUT_DIR)/%)
GENERATOR_SRC = $(GENERATOR:%=tools/%.cpp)

GROUP_NUMBER_OUT = doc/groupNumber

OUT_DIR = build
EJ4_OUT_DIR = build/ejercicio4

ENTREGABLE=entregable.tar.gz

.PHONY: all $(EJERCICIOS) clean groupNumber informe

all: $(OUT_DIR) $(EJ4_OUT_DIR) $(EJERCICIOS)

tar: $(ENTREGABLE)

$(ENTREGABLE): informe
	tar -czf $@ src -C doc informe.pdf

$(EJERCICIOS) : % : build/%

$(BINARIOS): $(OUT_DIR)/% : src/%.cpp
	$(GCC) $(CFLAGS) $< -o $@

$(GENERATOR_BIN): $(GENERATOR_SRC)
	$(GCC) $(CFLAGS) $< -o $@

%.o: %.cpp %.h
	$(GCC) $(CFLAGS) -c -o $@ $<

$(OUT_DIR) $(EJ4_OUT_DIR):
	mkdir -p $@

$(GROUP_NUMBER_OUT): $(GENERATOR_BIN)
	echo $(GROUP) $(SEED) | $^ >/dev/null 2>$@

groupNumber: $(GROUP_NUMBER_OUT)

informe: $(GROUP_NUMBER_OUT)
	make -C doc/

help:
	@echo "make 		para compilar los ejercicios"
	@echo "make ejX 	para compilar uno solo"
	@echo "make tar 	para generar el archivo entregable"

clean:
	rm -f build/*
	rm -f $(ENTREGABLE)

