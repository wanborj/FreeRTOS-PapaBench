#!/bin/bash

# get the execution time of all task under the preemptable scheduler
awk '
BEGIN{
    for(i=0;i<13;i++){ 
        flag[i]=0;
        time[i]=0;
        error[i]=0;
    }
}
{
    if($1<14 && flag[$1]==0){
        flag[$1] = 1;
        time[$1] = $2;
    }
    else{
        if(flag[ $1/3 -10 ] == 1){
            print ($1/3 -10), ($2-time[$1/3-10])/1000000.0;
            flag[$1/3 -10] = 0;
        }
        else{
            error[$1/3 -10] ++; 
        }
    }
}
END{
for( i=0;i<13;++i ){
#    print error[i];
}
}
' datafile.data > datafile.out


awk '
BEGIN{ 
    for(i=0;i<13;++i) {
        count[i]=0; 
        ave[i]=0.0; 
        total[i]=0.0; 
        max[i]=0.0; 
        min[i]=1000.0
    } 
}
{
    tmp = $2;
    total[$1] += tmp;
    count[$1] ++;
    if(tmp > max[$1]){
        max[$1] = tmp;

    }

    if( tmp < min[$1] ){
        min[$1] = tmp;
    }
}
END{ 
    print "ID", "Cycles", "Max", "Average", "Min"; 
    for(i=0;i<13;++i) {
        ave[i]=total[i]/count[i]; 
        print i, count[i], max[i], ave[i], min[i];
    }  
}
' datafile.out > datafile.ave

