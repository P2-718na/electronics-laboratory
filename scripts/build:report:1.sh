#!/usr/bin/env bash
mkdir -p ./build/docs/reports/1 &&
export $(cat ./docs/reports/.env | xargs) &&
pdflatex -output-directory=./build/docs/reports/1 ./docs/reports/1/report.tex &&
bibtex ./build/docs/reports/1/report.aux &&
pdflatex -output-directory=./build/docs/reports/1 ./docs/reports/1/report.tex && # Yep. Needed for references.
pdflatex -output-directory=./build/docs/reports/1 ./docs/reports/1/report.tex;   # Yep. Needed for bibtex.
