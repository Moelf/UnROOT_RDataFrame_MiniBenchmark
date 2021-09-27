pandoc -t beamer -H head.tex --number-sections --pdf-engine=xelatex -V fontsize=10pt -V 'monofont:JuliaMono' -V colorlinks=true -V aspectratio=169 --highlight-style zenburn slides.md -o slides.pdf
