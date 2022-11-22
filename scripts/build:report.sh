#!/usr/bin/env bash
mkdir -p ./build &&
cd report &&
export $(cat ../../.env | xargs) &&
pdflatex -output-directory=../build ./report.tex &&
cd .. &&
echo "1" &&
bibtex ./build/report.aux &&     # Whoever wrote bibtex needs to just stop.
cd report &&
pdflatex -output-directory=../build ./report.tex && # Yep. Needed for references.
pdflatex -output-directory=../build ./report.tex;   # Yep. Needed for bibtex.
