#!/bin/bash

# run-unit.sh
# Run unit tests through interpreter, write to log file
# TODO Verify output

INTERP="../interp"

$INTERP unit-ivecx.l2d >> unit-log.temp
