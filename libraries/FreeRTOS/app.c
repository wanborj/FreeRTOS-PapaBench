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
 
 
xSemaphoreHandle xSemaphoreTable[NUMBEROFTASK];

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

portTickType xPeriodTable[NUMBEROFTASK] = 
{
    125,
    125,
    250,
    250,
    250,
    125,
    250,
    250,
    1250,
    1250,
    1250,
    1250,
    500
};

/* explemented in main.c */
extern void to_autopilot_from_last_radio();
extern void check_mega128_values_task();
extern void check_failsafe_task();

extern void radio_control_task(); 
extern void send_gps_pos();
extern void send_radIR();
extern void send_takeOff();

extern void  send_boot();
extern void  send_attitude();
extern void  send_adc();
extern void  send_settings();
extern void  send_desired();
extern void  send_bat();
extern void  send_climb();
extern void  send_mode();
extern void  send_debug();
extern void  send_nav_ref();

void vTask_0()
{
    last_radio_from_ppm(); //ppm.h
}

void vTask_1()
{
    to_autopilot_from_last_radio();  //main_auto.c
    spi_reset(); // spi_fbw.h
}

void vTask_2()
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
    send_gps_pos();
    send_radIR();
    send_takeOff();
}

void vTask_9()
{
    nav_home();
    nav_update();
    course_pid_run();
}

void vTask_10()
{
    altitude_pid_run();
}

void vTask_11()
{
    climb_pid_run();
}

void vTask_12()
{
    send_boot();
    send_attitude();
    send_adc();
    send_settings();
    send_desired();
    send_bat();
    send_climb();
    send_mode();
    send_debug();
    send_nav_ref();
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
