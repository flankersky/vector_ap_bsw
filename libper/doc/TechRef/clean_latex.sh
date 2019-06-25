#!/bin/bash

function CLEAN {
    DIRECTORY=${PWD}
    EXTENSIONS=".aux .bbl .blg .idx .ilg .ind .lof .log .lol .lot .out .toc .nlo .xml .synctex.gz blx.bib *.nls"
    if [ -d ${DIRECTORY} ]; then
        for ext in ${EXTENSIONS}; do
            rm -f ${DIRECTORY}/*${ext}
        done
    fi
}

CLEAN