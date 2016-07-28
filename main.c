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

//#define PREEMPTION
#define NONPREEMPTION

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

    portTickType xReadyTime = 0;
    portTickType xDeadline = xMyPeriod ;
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

        for( i = 0; i < 20; ++ i )
        {
            xMyFun();
        }

        if(xTaskGetTickCount() > xDeadline)
        {
            //vPrintNumber((xMyId+10)*2);
            vPrintString("miss deadline\n\r");
            miss[xMyId] ++;
        }

        xReadyTime += xMyPeriod;
        xDeadline += xMyPeriod;


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
      
#ifdef configUSE_EDF_SCHEDULING
        vSetTaskDeadline( xDeadline );
#endif
        vTaskDelayUntil( &xLastExecutionTime, xMyPeriod / portTICK_RATE_MS );
        //vTaskDelay( xMyPeriod / portTICK_RATE_MS );
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
#ifdef configUSE_EDF_SCHEDULING
        xTaskPeriodicCreate(vTimeTask, pcNameOfTask[i],  256, (void *) &parameters[i], xTaskPrio[i], &xTaskOfHandle[i], parameters[i].xPeriod);
#else
        xTaskCreate( vTimeTask, pcNameOfTask[i], 256, (void *) &parameters[i], xTaskPrio[i], &xTaskOfHandle[i] );
#endif
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

void vApplicationTickHook()
{

}
