/*
 * application.h
 *
 *  Created on: Apr 21, 2022
 *      Author: Yahia Tawfik
 */
#include <LcdDriver/Crystalfontz128x128_ST7735.h>
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>
#include <ti/grlib/grlib.h>
#include <stdio.h>

#include "HAL/LED.h"
#include "HAL/Timer.h"
#include "HAL/Button.h"


#ifndef APPLICATION_H_
#define APPLICATION_H_

#define DUTY_CYCLE_FRACTION_BLUE 0.05

#define PWM_PERIOD_CYCLES_BLUE 20000

#define BLUE_ON_CYCLES (DUTY_CYCLE_FRACTION_BLUE * PWM_PERIOD_CYCLES_BLUE)

#define BLUE_OFF_CYCLES ((1-DUTY_CYCLE_FRACTION_BLUE) * PWM_PERIOD_CYCLES_BLUE)

#define BLU_COMPARE_CYCLES    BLUE_OFF_CYCLES

#define BLU_TIMER    TIMER_A2_BASE
#define BLU_CHANNEL  TIMER_A_CAPTURECOMPARE_REGISTER_1


#define BUZZ_TIMER TIMER_A0_BASE
#define BUZZ_CHANNEL TIMER_A_CAPTURECOMPARE_REGISTER_4

#define RED_CHANNEL  TIMER_A_CAPTURECOMPARE_REGISTER_3

#define DUTY_CYCLE_FRACTION_BUZZ 0.01

#define PWM_PERIOD_CYCLES_BUZZ 8000

#define BUZZ_ON_CYCLES (DUTY_CYCLE_FRACTION_BUZZ * PWM_PERIOD_CYCLES_BUZZ)

#define BUZZ_OFF_CYCLES ((1-DUTY_CYCLE_FRACTION_BUZZ) * PWM_PERIOD_CYCLES_BUZZ)

#define BUZZ_COMPARE_CYCLES    BUZZ_OFF_CYCLES


#define TWO_POINT_SEVEN_FIVE_SEC 132000000
#define QUARTER_SEC_COUNT 12000000


//Enum that has our Screen states
enum _GameState
{
    TITLE_SCREEN, INSTRUCTIONS_SCREEN, GAME_SCREEN, RESULT_SCREEN
};
typedef enum _GameState GameState;

//Struct for timer A variables
struct _TimerA
{
    Timer_A_PWMConfig pwmConfig_blu_set;
    Timer_A_PWMConfig clockSource;
    Timer_A_PWMConfig clockSourceDivider;
    Timer_A_PWMConfig compareOutputMode;
    Timer_A_PWMConfig compareRegister;
    Timer_A_PWMConfig timerPeriod;
    Timer_A_PWMConfig dutyCycle;

    Timer_A_PWMConfig pwmConfig_red_reset;

    Timer_A_PWMConfig pwmConfig_buzz_reset;
    Timer_A_PWMConfig pwmConfig_buzz_set;
};
typedef struct _TimerA timerA;

//construct for timer A values for blue, red LED and Buzzer
timerA Timer_A_construct()
{
timerA time;

time.pwmConfig_blu_set.clockSource = TIMER_A_CLOCKSOURCE_SMCLK; // The source clock is the system clock (3MHz)
time.pwmConfig_blu_set.clockSourceDivider = TIMER_A_CLOCKSOURCE_DIVIDER_24;
time.pwmConfig_blu_set.compareOutputMode = TIMER_A_OUTPUTMODE_SET; // The OC mode. We choose this such that we can achieve the waveform we are interested in.
time.pwmConfig_blu_set.compareRegister = BLU_CHANNEL; // The output register used for OC operation. The programmer does not choose this.
time.pwmConfig_blu_set.timerPeriod = PWM_PERIOD_CYCLES_BLUE; // The # of counter cycles in one PWM round (period of PWM in terms of counter cycle)
time.pwmConfig_blu_set.dutyCycle = BLU_COMPARE_CYCLES; // The OC value. This is the value that when the counter hits it, something happens to the waveform

time.pwmConfig_buzz_set.clockSource = TIMER_A_CLOCKSOURCE_SMCLK; // The source clock is the system clock (3MHz)
time.pwmConfig_buzz_set.clockSourceDivider = TIMER_A_CLOCKSOURCE_DIVIDER_24;
time.pwmConfig_buzz_set.compareOutputMode = TIMER_A_OUTPUTMODE_SET_RESET; // The OC mode. We choose this such that we can achieve the waveform we are interested in.
time.pwmConfig_buzz_set.compareRegister = BUZZ_CHANNEL; // The output register used for OC operation. The programmer does not choose this.
time.pwmConfig_buzz_set.timerPeriod = PWM_PERIOD_CYCLES_BUZZ; // The # of counter cycles in one PWM round (period of PWM in terms of counter cycle)
time.pwmConfig_buzz_set.dutyCycle = BUZZ_COMPARE_CYCLES; // The OC value. This is the value that when the counter hits it, something happens to the waveform

time.pwmConfig_buzz_reset.clockSource = TIMER_A_CLOCKSOURCE_SMCLK; // The source clock is the system clock (3MHz)
time.pwmConfig_buzz_reset.clockSourceDivider = TIMER_A_CLOCKSOURCE_DIVIDER_24;
time.pwmConfig_buzz_reset.compareOutputMode = TIMER_A_OUTPUTMODE_SET_RESET; // The OC mode. We choose this such that we can achieve the waveform we are interested in.
time.pwmConfig_buzz_reset.compareRegister = BUZZ_CHANNEL; // The output register used for OC operation. The programmer does not choose this.
time.pwmConfig_buzz_reset.timerPeriod = 0; // The # of counter cycles in one PWM round (period of PWM in terms of counter cycle)
time.pwmConfig_buzz_reset.dutyCycle = BUZZ_COMPARE_CYCLES; // The OC value. This is the value that when the counter hits it, something happens to the waveform

return time;

}

struct _Application
{
    GameState state;
    int x;
    int E;
    int c;
    int H;
    int A;
    int J;
    int l;
    int f;
    int steps;
    int counter;
    char A_array[3];
    bool firstCall;

    UART_Baudrate baudChoice;
    char RXchar;
    char P1word[6];
    int char_count_P1;
    char P2word[6];
    char BSword[6];
};
typedef struct _Application Application;

Application app_construct()
{
    Application app;
    app.firstCall = true;
    app.state = TITLE_SCREEN;
    app.x = 0;
    app.E = 0;
    app.c = 1;
    app.H = 0;
    app.A = 0;
    app.J = 1;
    app.l = 0;
    app.steps = 0;
    app.counter = 0;
    app.f = 0;
    int i;
    for (i = 0; i < 3; i++)
    {
        app.A_array[i] = 0;
    }
    Timer32_setCount(TIMER32_1_BASE, QUARTER_SEC_COUNT); // The count related to the period of the timer
    Timer32_startTimer(TIMER32_1_BASE, true);

       app.baudChoice = BAUD_9600;
       app.RXchar = 0;
       app.state = TITLE_SCREEN;
       int u;
       for ( u = 0; u < 10; u++)
       {
           app.P1word[u] = 0;
           app.BSword[u] = 0;
       }
       app.char_count_P1 = 0;

    return app;
}


#endif /* APPLICATION_H_ */
