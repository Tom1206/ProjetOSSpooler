#!/bin/sh
make mrproper
make
./bin/deposer data/lol.txt
./bin/demon -d -i 3
