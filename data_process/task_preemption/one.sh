#!/bin/bash

for i in `seq 0 12` 
do
    awk '$1=="'$i'"{print $4-$3}' datafile.out > tmp
    awk '{print NR, $1}' tmp > ./task/$i.out

    echo "
set title \"Distribution of Task $i Response Time\"

unset key
set grid
set autoscale fix
set ytics 0,1
set xlabel \"Cycles\"
set ylabel \"Task Id\"

plot '$i.out' using 1:2 w p pt 7 ps 1 
    " > ./task/$i.dem
done

rm tmp
