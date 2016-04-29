#!/bin/bash

awk '
BEGIN{
    id = -1;
    time1 = 0;
    time2 = 0.0;
}
{
    if($1 < 100){
        id = $1;
    }else if( time1 == 0 ){
        time1 = $1;
    }else{
        print id, time1, $1, $2;
        id = -1;
        time1 = 0;
    }
} 
' response_time.data > datafile.data

# get the execution time of all task under the preemptable scheduler
awk '
BEGIN{
    for(i=0;i<13;i++){ 
        flag[i]=0;
        time[i]=0;
        ready[i]=0;
        error[i]=0;
    }
    maxtime[0]= 20;
    maxtime[1]= 10;
    maxtime[2]= 5;
    maxtime[3]= 20;
    maxtime[4]= 25;
    maxtime[5]= 28;
    maxtime[6]= 26;
    maxtime[7]= 5;
    maxtime[8]= 10;
    maxtime[9]= 30;
    maxtime[10]= 14;
    maxtime[11]= 30;
    maxtime[12]= 4;
}
{
    if($1<14 && flag[$1]==0){
        flag[$1] = 1;
        ready[$1] = $2;
        time[$1] = $4;
    }
    else{
        id = $1/3-10;
        if(flag[ id ] == 1){
            execution_time = ($4-time[id])/1000000.0;
            #if( execution_time < maxtime[id] ){
                print id, execution_time, ready[id], $3, $2;
            #}
            flag[id] = 0;
        }
        else{
            error[id] ++; 
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

