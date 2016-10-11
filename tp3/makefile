GCC=g++
CFLAGS= -O3 -Wall -Wextra -ggdb -std=c++11
#CFLAGS= -O0 -Wall -Wextra -ggdb -std=c++11
VALGRIND=valgrind --leak-check=full

EJERCICIOS = ej1 ej2 ej3 ej4 4_cuad

SOURCES = $(EJERCICIOS:%=src/%.cpp)
BINARIOS = $(EJERCICIOS:%=$(OUT_DIR)/%)

OUT_DIR = build

ENTREGABLE=entregable.tar.gz

.PHONY: all $(EJERCICIOS) clean informe

all: $(OUT_DIR) $(EJERCICIOS)

tar: $(ENTREGABLE)

$(ENTREGABLE): informe
	tar -czf $@ \
		--transform 's,^,Bárbara liskov/,' \
		--transform 's,src/ej([0-9]),ej\1/ej\1,x' \
		src/ej{1..4}.cpp -C doc informe.pdf

$(EJERCICIOS) : % : build/%

$(BINARIOS): $(OUT_DIR)/% : src/%.cpp
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
	rm -f $(BINARIOS)
	rm -f $(GENERATOR_BIN)
	rm -f $(ENTREGABLE)

