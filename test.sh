#!/bin/sh
make mrproper
make
./bin/deposer data/lol.txt
./bin/demon -d -i 3 /home/joseffu/Travaux/SE/projet_SE_git/ProjetOSSpooler/data/log.txt
