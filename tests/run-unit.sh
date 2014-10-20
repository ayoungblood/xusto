#!/bin/bash

# run-unit.sh
# Run unit tests through interpreter
# TODO Verify output

INTERP="../c/xusto"

tests=$(ls | grep unit-)

for file in $tests; do
    $INTERP $file
    echo
done
