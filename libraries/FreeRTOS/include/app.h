/*************************************************************************
	> File Name: app.h
	> Author: 
	> Mail: 
	> Created Time: Wed 20 Apr 2016 11:43:08 AM CST
 ************************************************************************/

#ifndef _APP_H
#define _APP_H

#include "stm32f10x.h"

/* Scheduler includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "list.h"
#include "queue.h"
#include "semphr.h"

#define NUMBEROFTASK 13

typedef void( * pvTaskFunType )(void);
void vPapabenchInit();

void vTask_0();
void vTask_1();
void vTask_2();
void vTask_3();
void vTask_4();
void vTask_5();
void vTask_6();
void vTask_7();
void vTask_8();
void vTask_9();
void vTask_10();
void vTask_11();
void vTask_12();

#endif
