#!/bin/bash

SCRIPTDIR=$(/usr/bin/dirname $0)
cd "$SCRIPTDIR"

SIP="/usr/local/amsrgen/sip-R17-10"

OUTPUT="../src-gen"

# create src-gen directory if it does not exist
if [ ! -d "${OUTPUT}" ]; then
    mkdir "${OUTPUT}"
fi

rm -rf $OUTPUT/adaptiveCalculatorServer
rm -rf $OUTPUT/adaptiveCalculatorClient


"$SIP/amsrgen.sh" \
    -x arxml \
    -o "$OUTPUT" \
    -m vector

exit 0
