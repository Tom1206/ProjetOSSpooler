#!/bin/sh
mkdir "res"
for i in `seq 2 10`
do
    seq 100 >"res/titi$i"
done
cd ../bin
./demon -d -i 3 ../dossier_tests/res/log.txt

for i in `seq 2 10`
do
    ./deposer "../dossier_tests/res/titi$i"
done