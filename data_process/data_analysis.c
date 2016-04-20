/*************************************************************************
	> File Name: data_analysis.c
	> Author: 
	> Mail: 
	> Created Time: Tue 12 Apr 2016 06:23:56 PM CST
 ************************************************************************/

#include<stdio.h>
#define NUMBEROFTASK 13

int count[NUMBEROFTASK];
double max[NUMBEROFTASK];
double min[NUMBEROFTASK];
double average[NUMBEROFTASK];
double variation[NUMBEROFTASK];
void init()
{
    int i;
    for( i = 0; i < NUMBEROFTASK; i ++ )
    {
        count[i] = 0;
        max[i] = 0;
        min[i] = 10000.0;
        average[i] = 0.0;
        variation[i] = 0.0;
    }
}

void get_average_and_max()
{
    int id, i;
    int duration;
    freopen("data_process.data", "r",stdin);
    while(scanf("%d %d\n", &id, &duration) != EOF)
    {
        count[id] ++; 
        if( max[id] < duration )
        {
            max[id] = duration;
        }

        if( min[id] > duration )
        {
            min[id] = duration;
        }
        average[id] += duration;
    }
    for( i = 0; i < NUMBEROFTASK; ++ i )
    {
        average[i] = average[i] / count[i];
    }
    fclose(stdin);
}

void get_variation()
{
    int id, i;
    int duration; 
    double temp[NUMBEROFTASK];
    for( i = 0; i < NUMBEROFTASK; ++ i )
    {
        temp[i] = 0;
    }

    freopen("data_process.data", "r",stdin);

    while(scanf("%d %d", &id, &duration) != EOF )
    {
        variation[id] += (duration - average[id]) * (duration - average[id]);         
    }

    for( i = 0; i < NUMBEROFTASK; ++ i )
    {
        variation[i] /= count[i];
    }

    fclose(stdin);
}

void output()
{
    int i;
    printf("%5s %5s %10s %10s %10s %10s\n", "id","Count", "Max", "Min", "Average", "Variation");

    freopen("data_analysis.data", "w",stdout);

    for( i = 0; i < NUMBEROFTASK; ++ i )
    {
        printf("%5d %5d %10lf %10lf %10lf %10lf\n", i, count[i], max[i], min[i], average[i], variation[i]);
    }

    fclose(stdout);
}

int main()
{
    init();
    get_average_and_max();
    get_variation();
    output();

    return 0;
}
