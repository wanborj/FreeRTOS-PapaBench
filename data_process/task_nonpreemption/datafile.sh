#!/bin/bash

awk ' 
BEGIN{
    maxtime[0]= 9;
    maxtime[1]= 3;
    maxtime[2]= 1; 
    maxtime[3]= 6; 
    maxtime[4]= 6; 
    maxtime[5]= 12;
    maxtime[6]= 8;
    maxtime[7]= 0.7;
    maxtime[8]= 1.6;
    maxtime[9]= 8.9; 
    maxtime[10]= 2.8; 
    maxtime[11]= 8; 
    maxtime[12]= 4; 
}
{
    if($1<20){
        id = $1;
        time1 = $2;
    }else{
        time2 = ($2 - time1)/1000000.0
        if(time2 < maxtime[id]){
            print id, ($2-time1)/1000000.0;
        }
    }
}
' datafile.data > datafile.out

awk '
BEGIN{ for(i=0;i<13;++i) {count[i]=0; ave[i]=0.0; var[i]=0.0; total[i]=0.0; max[i]=0.0; min[i]=1000000.0}}
{
    tmp = $2;
    total[$1] += tmp;
    count[$1] ++;
    if(tmp > max[$1]){
        max[$1] = tmp;
    }

    if( tmp < min[$1]){
        min[$1] = tmp;
    }
}
END{ print "ID", "Cycles", "Max", "Average", "Min"; for(i=0;i<13;++i) {ave[i]=total[i]/count[i]; print i, count[i], max[i], ave[i], min[i];} }
' datafile.out > datafile.ave
