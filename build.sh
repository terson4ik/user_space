#!/bin/sh

mkdir bin
cd bin

gcc ../print_ascii.c    -o ASCII
gcc ../remove_letter.c  -o rm_lit
gcc ../replace_letter.c -o mv_lit

cd ..
echo "done"
