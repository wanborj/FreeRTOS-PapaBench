#define USE_STDPERIPH_DRIVER
#include "stm32f10x.h"

/* Scheduler includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
#include <string.h>
#include "app.h"


struct parameter
{
    portBASE_TYPE xMyId;
    pvTaskFunType xMyFun;
    portTickType xPeriod;
    xSemaphoreHandle xMySem;

}parameters[NUMBEROFTASK];

xTaskHandle xTaskOfHandle[NUMBEROFTASK];

extern portBASE_TYPE xTaskPrio[NUMBEROFTASK]; 

/* can be found in app.c */
extern portTickType xPeriodTable[NUMBEROFTASK];
extern pvTaskFunType xTaskTable[NUMBEROFTASK];
extern xSemaphoreHandle xSemaphoreTable[NUMBEROFTASK];

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
    portTickType xLastExecutionTime = 100; 

    while(1)
    {
        // all the task start to execute from 100 ms.
        if( IS_FIRST_TIME_TO_EXE == 1 )
        {
            xSemaphoreTake(xMySem, portMAX_DELAY);
            IS_FIRST_TIME_TO_EXE = 0;
        }

        vPrintNumber(xMyId);
        xCurrentTime = xTaskGetTickCount();
        vPrintNumber(xCurrentTime);

        for( i = 0; i < 500; ++ i )
        {
            xMyFun();
        }

        xCurrentTime = xTaskGetTickCount();
        vPrintNumber(xCurrentTime);
        vPrintNumber((xMyId + 10) * 3);
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
        xTaskCreate(vTimeTask, "vTimeTask",  128, (void *) &parameters[i], xTaskPrio[i], &xTaskOfHandle[i]);
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
    
    if( xCurrentTime == 100 )
    {
        for( i = 0; i < NUMBEROFTASK; ++ i )
        {
            xSemaphoreGive( xSemaphoreTable[i] );
        }
    }
}
