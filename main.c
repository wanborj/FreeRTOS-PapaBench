#define USE_STDPERIPH_DRIVER
#include "stm32f10x.h"

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
    xSemaphoreHandle xMySem;

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

void vSemaphoreInitialise()
{
    portBASE_TYPE i;

    for( i = 0; i < NUMBEROFTASK; ++ i  )
    {
        vSemaphoreCreateBinary(xSemaphoreTable[i]);
        /* when created, it is initialised to 1. So, we take it away.*/
        xSemaphoreTake(xSemaphoreTable[i], portMAX_DELAY);
    }
}

void vParametersInit()
{
    portBASE_TYPE i;
    for( i = 0; i < NUMBEROFTASK; ++ i )
    {
        parameters[i].xMyId = i;
        parameters[i].xMyFun = xTaskTable[i];
        parameters[i].xPeriod = xPeriodTable[i];
        parameters[i].xMySem = xSemaphoreTable[i];
        miss[i] = 0;
    }
}

void vTimeTask( void * pvParameter )
{
    struct parameter *xMyParameter = (struct parameter *)pvParameter;
    portBASE_TYPE xMyId = xMyParameter->xMyId;
    pvTaskFunType xMyFun = xMyParameter->xMyFun;
    portTickType xMyPeriod = xMyParameter->xPeriod;
    xSemaphoreHandle xMySem = xMyParameter->xMySem;

    portBASE_TYPE i;
    portTickType xCurrentTime;
    portBASE_TYPE IS_FIRST_TIME_TO_EXE = 1;
    
    // used for periodic task
    portTickType xLastExecutionTime = xMyPeriod ; 
    portTickType xSystemDeadline = 20000;

    portBASE_TYPE xCount = 1;
    portTickType xReadyTime = xCount * xMyPeriod;
    portTickType xDeadline ;
    while(1)
    {
        // all the task start to execute from 100 ms.
        if( IS_FIRST_TIME_TO_EXE == 1 )
        {
            xSemaphoreTake(xMySem, portMAX_DELAY);
            IS_FIRST_TIME_TO_EXE = 0;
        }
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
      
        if(xTaskGetTickCount() > 100000)
        {
            break;
        }

        vTaskDelayUntil( &xLastExecutionTime, xMyPeriod / portTICK_RATE_MS );
    }
}


int main()
{

	init_led();
	init_rs232();
	enable_rs232_interrupts();
	enable_rs232();

    vSemaphoreInitialise();
    vParametersInit();

    portBASE_TYPE i;
    for( i = 0; i < NUMBEROFTASK; ++ i )
    {
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
    portBASE_TYPE i;
    portTickType xCurrentTime = xTaskGetTickCount();
    
    if( xCurrentTime > 0 && xCurrentTime <1001 )
    {
        for( i = 0; i < NUMBEROFTASK; ++ i )
        {
            if( xCurrentTime == xPeriodTable[i] )
            xSemaphoreGive( xSemaphoreTable[i] );
        }
    }
}
