#!/bin/bash

echo "Compiling to .BIN → .G3A"

#Remove the compiled files.
rm prgm.g3a
rm prgm.bin

#Recompile
make

echo "Completed compilation: Created prgm.G3A"
