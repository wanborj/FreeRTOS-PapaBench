#!/bin/bash

awk ' 
BEGIN{ ID1=0; TIME1=0; TIME2=0; T1=0; T2=0; } 
{
    if($1<20){ 
            ID1=$1;
            T1=$2;
    }
    else if($1>=TIME1 && TIME1 > 0){
        TIME2=$1;
    }
    else if($1==((ID1+10)*3)){
        print NR/4, ID1, TIME2-TIME1, ($2-T1)/1000.0;
            ID1=0; TIME1=0; TIME2=0;T1=0;T2=0; 
    }
    else {
        TIME1=$1; 
    }
}
' datafile.data > datafile.out

awk '
BEGIN{ for(i=0;i<13;++i) {count[i]=0; ave[i]=0.0; var[i]=0.0; total[i]=0.0; max[i]=0.0; min[i]=1000000.0}}
{
    tmp = $4/1000.0;
    total[$2] += tmp;
    count[$2] ++;
    if(tmp > max[$2])
        {
            max[$2] = tmp;
        }

    if( tmp < min[$2])
        {
            min[$2] = tmp;
        }
}
END{ print "ID", "Cycles", "Max", "Average", "Min"; for(i=0;i<13;++i) {ave[i]=total[i]/count[i]; print i, count[i], max[i], ave[i], min[i];} }
' datafile.out > datafile.ave
