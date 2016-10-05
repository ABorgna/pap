targets = informe.pdf

includes = secciones/ej1.tex secciones/ej2.tex secciones/ej3.tex secciones/ej4.tex
libs =

all: $(targets)

$(targets) : %.pdf : %.tex $(libs) $(includes)
	pdflatex $*
	pdflatex $*

clean:
	@rm $(targets) 2>/dev/null || true
	@rm ./*.aux 2>/dev/null || true
	@rm ./*.log 2>/dev/null || true
	@rm ./*.out 2>/dev/null || true
	@rm ./*.toc 2>/dev/null || true
	@rm ./*.bbl 2>/dev/null || true
	@rm ./*.blg 2>/dev/null || true

