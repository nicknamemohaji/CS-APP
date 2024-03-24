#! /bin/bash

for i in {1..1000}
do
    gcc main.c -D GAP=$i -o main$i.o
    valgrind --tool=cachegrind ./main$i.o >> gap.txt
    rm main$i.o
done
