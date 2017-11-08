#!/bin/sh
make mrproper
make
PROJETSE="/home/joseffu/Travaux/SE/projet_SE_git/ProjetOSSpooler/data/spool"
export PROJETSE
./bin/deposer data/lol.txt data/machin_test data/image.jpg
./bin/lister
./bin/demon -d -i 3 /home/joseffu/Travaux/SE/projet_SE_git/ProjetOSSpooler/data/log.txt
