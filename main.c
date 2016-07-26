#define USE_STDPERIPH_DRIVER
#include "stm32f10x.h"
#include "stm32_p103.h"

/* Scheduler includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
#include <string.h>
#include "app.h"

#define PREEMPTION
//#define NONPREEMPTION

struct parameter
{
    portBASE_TYPE xMyId;
    pvTaskFunType xMyFun;
    portTickType xPeriod;
}parameters[NUMBEROFTASK];

xTaskHandle xTaskOfHandle[NUMBEROFTASK];

extern void vTaskSuspendAll();
extern signed portBASE_TYPE xTaskResumeAll(); 
extern portBASE_TYPE xTaskPrio[NUMBEROFTASK]; 

/* can be found in app.c */
extern portTickType xPeriodTable[NUMBEROFTASK];
extern pvTaskFunType xTaskTable[NUMBEROFTASK];
extern xSemaphoreHandle xSemaphoreTable[NUMBEROFTASK];
extern char * pcNameOfTask[NUMBEROFTASK];

portBASE_TYPE miss[NUMBEROFTASK];


void vParametersInit()
{
    portBASE_TYPE i;
    for( i = 0; i < NUMBEROFTASK; ++ i )
    {
        parameters[i].xMyId = i;
        parameters[i].xMyFun = xTaskTable[i];
        parameters[i].xPeriod = xPeriodTable[i];
        miss[i] = 0;
    }
}

void vTimeTask( void * pvParameter )
{
    struct parameter *xMyParameter = (struct parameter *)pvParameter;
    portBASE_TYPE xMyId = xMyParameter->xMyId;
    pvTaskFunType xMyFun = xMyParameter->xMyFun;
    portTickType xMyPeriod = xMyParameter->xPeriod;

    portBASE_TYPE i;
    portTickType xCurrentTime;
    
    // used for periodic task
    portTickType xLastExecutionTime = 0; 

    portBASE_TYPE xCount = 1;
    portTickType xReadyTime = xCount * xMyPeriod;
    portTickType xDeadline ;
    while(1)
    {

#ifdef PREEMPTION
        vTaskSuspendAll();
        vPrintNumber(xMyId);
        vPrintNumber( xReadyTime );
        vPrintNumber( xTaskGetTickCount() );
        xTaskResumeAll();
#endif

        #ifdef NONPREEMPTION
        vPrintNumber(xMyId);
        vPrintNumber( xReadyTime );
        vPrintNumber( xTaskGetTickCount() );
        #endif

        for( i = 0; i < 200; ++ i )
        {
            xMyFun();
        }

        if(xTaskGetTickCount() > xLastExecutionTime + xMyPeriod)
        {
            //vPrintNumber((xMyId+10)*2);
            vPrintNumber(xMyId);
            vPrintString("miss deadline\n\r");
            miss[xMyId] ++;
        }

        xCount ++;
        xReadyTime = xCount * xMyPeriod;
        xDeadline = xReadyTime ;


#ifdef PREEMPTION
        vTaskSuspendAll();
        vPrintNumber((xMyId + 10) * 3);
        vPrintNumber( xDeadline );
        vPrintNumber( xTaskGetTickCount() );
        xTaskResumeAll();
#endif
        #ifdef NONPREEMPTION
        vPrintNumber(xTaskGetTickCount());
        vPrintNumber( xDeadline );
        vPrintNumber( (xMyId + 10)* 3 );
        #endif
      
        vTaskDelayUntil( &xLastExecutionTime, xMyPeriod / portTICK_RATE_MS );
    }
}


int main()
{

	init_led();
	init_rs232();
	enable_rs232_interrupts();
	enable_rs232();

    vParametersInit();
    vPapabenchInit();

    int i;
    for( i = 0; i < NUMBEROFTASK; ++ i )
    {
        if(i == 0 || i == 1 || i == 5)
        {
            //continue;
        }
        xTaskCreate(vTimeTask, pcNameOfTask[i],  256, (void *) &parameters[i], xTaskPrio[i], &xTaskOfHandle[i]);
    }

	/* Start running the tasks. */
	vTaskStartScheduler();

	return 0;
}

void myTraceCreate      (){
}

void myTraceSwitchedIn  (){
}

void myTraceSwitchedOut	(){
}

inline float myTraceGetTick(){
}

inline unsigned long myTraceGetTimeMillisecond(){
	//return (xTaskGetTickCountFromISR() + myTraceGetTick()) * 1000 / configTICK_RATE_HZ;
}

void vApplicationTickHook()
{

}
