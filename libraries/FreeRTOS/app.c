/*************************************************************************
	> File Name: app.c
	> Author: 
	> Mail: 
	> Created Time: Wed 20 Apr 2016 11:47:10 AM CST
 ************************************************************************/

#include "app.h"

#define USE_STDPERIPH_DRIVER

 /* the include file of PapaBench */
#include "ppm.h"
#include "servo.h"
#include "spi_fbw.h"
#include "infrared.h"
#include "estimator.h"
#include "pid.h"
#include "link_fbw.h"
#include "gps.h"
#include "autopilot.h"
#include "nav.h"
#include "adc.h" 
 
xSemaphoreHandle xSemaphoreTable[NUMBEROFTASK];



#if (defined configUSE_LSF_SCHEDULING) || (defined configUSE_EDF_SCHEDULING)
portBASE_TYPE xTaskPrio[NUMBEROFTASK] = 
{
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1
};
#else
portBASE_TYPE xTaskPrio[NUMBEROFTASK] = 
{
    5,
    5,
    4,
    4,
    4,
    5,
    4,
    4,
    2,
    2,
    2,
    2,
    3
};
#endif

portTickType xPeriodTable[NUMBEROFTASK] = 
{
    250,
    250,
    500,
    500,
    500,
    250,
    500,
    500,
    2500,
    2500,
    2500,
    2500,
    1000   
};

#if defined configUSE_LSF_SCHEDULING
portTickType WCET[NUMBEROFTASK]=
{
    0,
    0,
    6,
    7,
    6,
    0,
    5,
    1,
    1,
    3,
    2,
    6,
    2
};
#endif


char * pcNameOfTask[NUMBEROFTASK] =
{
    "1",
    "2",
    "3",
    "4",
    "5",
    "6",
    "7",
    "8",
    "9",
    "10",
    "11",
    "12",
    "13"
};

/* explemented in main.c */
extern void to_autopilot_from_last_radio();
extern void check_mega128_values_task();
extern void check_failsafe_task();
extern void radio_control_task(); 
extern void use_gps_pos();
extern void navigation_update();
extern void send_nav_values();
extern void course_run();
extern void altitude_control_task();
extern void climb_control_task();
extern void reporting_task();

extern void timer_init();
extern void modem_init();
extern void adc_init();
extern void adc_buf_channel(uint8_t adc_channel, struct adc_buf *s);
extern void spi_init();
extern void link_fbw_init();
extern void gps_init();
extern void nva_init();
extern void ir_init();
extern void estimator_init();
extern void fbw_init();

extern uint8_t mode ;
extern uint8_t pprz_mode;

void vPapabenchInit()
{
    mode = MODE_AUTO;
    pprz_mode = PPRZ_MODE_HOME;
    timer_init();
    modem_init();
    adc_init();
#ifdef CTL_BRD_V1_1
    adc_buf_channel(uint8_t adc_channel, struct adc_buf *s);
#endif 
    spi_init();
    link_fbw_init();
    gps_init();
    nav_init();
    ir_init();
    estimator_init();
#ifdef PAPABENCH_SINGLE
    fbw_init();
#endif
}


void vTask_0()
{
    last_radio_from_ppm(); //ppm.h
}

void  vTask_1()
{
    to_autopilot_from_last_radio();  //main_auto.c
    spi_reset(); // spi_fbw.h
}

void  vTask_2()
{
    check_mega128_values_task(); // main_auto.c
}

void vTask_3()
{
    servo_transmit(); //servo.h
}

void vTask_4()
{
    check_failsafe_task(); //main_auto.c
}

void vTask_5()
{
    radio_control_task(); //main_auto.c
    ir_gain_calib();
}

void vTask_6()
{
    ir_update();
    estimator_update_state_infrared();
    roll_pitch_pid_run();
}

void vTask_7()
{
    link_fbw_send(); // link_fbw.h
}

void vTask_8()
{
    parse_gps_msg();
    use_gps_pos();
}

void vTask_9()
{
    navigation_update();
    send_nav_values();
    course_run();
}

void vTask_10()
{
    altitude_control_task();
}

void vTask_11()
{
    climb_control_task();
}

void vTask_12()
{
    reporting_task();
}

pvTaskFunType xTaskTable[NUMBEROFTASK] =
{
    &vTask_0,
    &vTask_1,
    &vTask_2,
    &vTask_3,
    &vTask_4,
    &vTask_5,
    &vTask_6,
    &vTask_7,
    &vTask_8,
    &vTask_9,
    &vTask_10,
    &vTask_11,
    &vTask_12
};
