#!/bin/bash

awk ' 
BEGIN{
    id = -1;
    readytime = 0;
    starttime = 0;
    responsetime = 0;
    deadline = 0;
}
{
    if(id == -1 && $1<15){
        id = $1;
        time1 = $2;
    }else if( $1 == (id+10)*3 ){
        print id, ($2 - time1)/1000.0, readytime, responsetime, deadline;
        id = -1;
        readytime = 0;
        starttime = 0;
        responsetime = 0;
        deadline = 0;
    }else if( readytime == 0 ){
        readytime = $1;
    }else if( starttime == 0 ){
        starttime = $1;
    }else if( responsetime == 0 ){
        responsetime = $1 ;
    }else{
        deadline = $1
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
