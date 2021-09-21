pandoc -t beamer -H head.tex --number-sections --pdf-engine=xelatex -V 'monofont:JuliaMono' -V colorlinks=true -V aspectratio=169 --highlight-style zenburn slides.md -o slides.pdf
