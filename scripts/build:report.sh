#!/usr/bin/env bash
mkdir -p ./build &&
export $(cat ../.env | xargs) &&
pdflatex -output-directory=./build ./report/report.tex &&
bibtex ./build/report.aux &&
pdflatex -output-directory=./build ./report/report.tex && # Yep. Needed for references.
pdflatex -output-directory=./build ./report/report.tex;   # Yep. Needed for bibtex.
