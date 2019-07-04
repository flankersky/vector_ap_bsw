#!/bin/bash

SCRIPTDIR=$(/usr/bin/dirname $0)
cd "$SCRIPTDIR"

SIP="/usr/local/amsrgen/sip"

OUTPUT="../etc"

rm -rf $OUTPUT/restfulServer
rm -rf $OUTPUT/restfulClient


"$SIP/amsrgen.sh" \
    -x arxml \
    -o "$OUTPUT" \
    -m vector

exit 0
