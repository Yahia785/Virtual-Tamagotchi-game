// S22 project 3
#include <LcdDriver/Crystalfontz128x128_ST7735.h>
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>
#include <ti/grlib/grlib.h>
#include <stdio.h>

#include "HAL/LED.h"
#include "HAL/Timer.h"
#include "HAL/HAL.h"
#include "HAL/Button.h"
#include "application.h"
#include "Functions.h"
#include "HAL/Timer.h"
#include "Character_Images.h"

#define MOVING_CHARACTER_CONSTANT 10
#define NUMBER_OF_TAPS_LEFT -3
#define NUMBER_OF_TAPS_RIGHT 4
#define SPACING_CONSTANT 7
#define DOUBLE_STEP 2
#define THREE_SEC_COUNTER 12
#define QUARTER_SEC_COUNTER 1
#define HALF_SEC_COUNTER 2
#define THREE_QUARTERS_SEC_COUNTER 3
#define ONE_SEC_COUNTER 4



int main(void)
{
    //initialize ports and pins
    initialize();
    initPWMPins();

    //Initialize constructs
    Application app = app_construct();
    timerA time = Timer_A_construct();
    HAL hal = HAL_construct();

    //calling functions of timerA to be initialized

    TimerA_SetValues(&time);
    Buzz_SetValues(&time);
    Buzz_ResetValues(&time);

    //UART updating communication function
    Application_updateCommunications(&app, &hal);

    //calling title screen to be displayed
    showTitleScreen(&hal, &app);

    while (1) {
        main_loop(&hal, &app, &time);
        sleep();
    }
}

void sleep() {
    // The Launchpad Green LED is used to signify the processor is in low-power mode.
    // From the human perspective, it should seem the processor is always asleep except for fractions of second here and there.

    TurnOn_LLG();
    // Enters the Low Power Mode 0 - the processor is asleep and only responds to interrupts
    PCM_gotoLPM0();
    TurnOff_LLG();

}
//Our main loop function that has the main switch statement for all our screens. Code moves between screens through this switch statement
void main_loop(HAL* hal_p, Application* app_p, timerA* time)
{
    // The first step is update the tapping status of all buttons
    HAL_refresh(hal_p);


    // Do not remove this statement. This is the non-bocking code for this project
    // Tapping the Joystick push button (JSB) toggles the BoosterPack Green LED
    if (hal_p->buttons.JSBtapped)
        Toggle_BLG();

    // TODO: You probably want to define a state variable and an enum associated with it
    // You can then control the state of your application based on the hardware inputs
    // Refer to the Guess-the-color example.
    switch (app_p->state)
       {
           case TITLE_SCREEN:
               handleTitleScreen(hal_p, app_p);
               break;

           case INSTRUCTIONS_SCREEN:
               handleInstructionScreen(hal_p, app_p);
               break;

           case GAME_SCREEN:
              HandleGameScreen(hal_p, app_p, time);
               break;

           case RESULT_SCREEN:
               HandleResultScreen(hal_p, app_p);
               break;

           default:
               break;
       }

}

// Initialization part is always device dependent and therefore does not change much with HAL
void initialize()
{

    // Stop watchdog timer
    WDT_A_hold(WDT_A_BASE);
    InitSystemTiming();

    initLEDs();

    // TODO: Add other initializations here
    initButtons();

}

//Graphics display of title screen where some information about the project is displayed
void showTitleScreen(HAL *hal_p, Application *app_p)
{
    Graphics_clearDisplay(&hal_p->g_sContext);
    Graphics_drawString(&hal_p->g_sContext, (int8_t*) "ECE 2564", -1, 37, 20, true);
    Graphics_drawString(&hal_p->g_sContext, (int8_t*) "Project 3", -1, 35, 30, true);
    Graphics_drawString(&hal_p->g_sContext, (int8_t*) "Yahia Tawfik", -1, 25, 40, true);
    Graphics_drawString(&hal_p->g_sContext, (int8_t*) "Low Power", -1, 25, 60, true);
    Graphics_drawString(&hal_p->g_sContext, (int8_t*) "Tamagotchi", -1, 25, 70, true);
    Graphics_drawString(&hal_p->g_sContext, (int8_t*) "Your interrupt-", -1, 16, 90,true);
    Graphics_drawString(&hal_p->g_sContext, (int8_t*) "driven virtual pet!", -1,8 , 100,true);
}

//handling title screenby starting a quarter second timer. Each time the counter expires a counter variable is incremented
//when the counter reaches 12 this means that three seconds has passed so screen changes from title screen to instruction
//screen
void handleTitleScreen(HAL *hal_p, Application *app_p)
{
    if (Timer32_getValue(TIMER32_1_BASE) == 0)
    {
        app_p->counter++;
        Timer32_setCount(TIMER32_1_BASE, QUARTER_SEC_COUNT);
        Timer32_startTimer(TIMER32_1_BASE, true);

    }

    if (app_p->counter == THREE_SEC_COUNTER)
    {
        app_p->counter = 0;
        app_p->state = INSTRUCTIONS_SCREEN;
        showInstructionScreen(hal_p, app_p);

    }
}

//Grphics display of instruction screen. This has the instructions for the game and an image is displayed of our character
void showInstructionScreen(HAL* hal_p, Application* app_p)
{
    Graphics_clearDisplay(&hal_p->g_sContext);
    Graphics_drawString(&hal_p->g_sContext, (int8_t*) "Welcome to... ", -1, 30, 10, true);
    Graphics_drawString(&hal_p->g_sContext, (int8_t*) "T A M A G O T C H I ", -1, 10, 25, true);
    Graphics_drawString(&hal_p->g_sContext, (int8_t*) "Feed your pet with ", -1, 10, 45, true);
    Graphics_drawString(&hal_p->g_sContext, (int8_t*) "BB1. Play with your ", -1, 10, 55, true);
    Graphics_drawString(&hal_p->g_sContext, (int8_t*) "pet using LB1 and  ", -1, 10, 65, true);
    Graphics_drawString(&hal_p->g_sContext, (int8_t*) "LB2. ", -1, 10, 75, true);
    Graphics_drawString(&hal_p->g_sContext, (int8_t*) "Press BB1 to start.. ", -1, 10, 119, true);
    Graphics_drawImage(&hal_p->g_sContext, &Instruction_screen_image, 60, 80);
}

//Handling instruction screen when button 1 on boosterpack is pressed the screen changes to Game screen.
void handleInstructionScreen (HAL* hal_p, Application* app_p)
{
    if (hal_p->buttons.BB1tapped)
    {
        app_p->state = GAME_SCREEN;
        showGameScreen(hal_p, app_p);
    }
}

//Graphics display of Name Screen where user is asked to enter a name for his character and the name will be displayed
void ShowNameScreen(HAL* hal_p, Application* app_p)
{

    Graphics_clearDisplay(&hal_p->g_sContext);
    Graphics_drawString(&hal_p->g_sContext, (int8_t*) "Choose a name: ", -1, 10, 10, true);
    Graphics_drawString(&hal_p->g_sContext, (int8_t*) app_p->P1word, 6, 30, 17, true);
}


//Graphics display for game screen including image of baby character, age, energy level and happiness level..
void showGameScreen(HAL* hal_p, Application* app_p)
{
    Graphics_clearDisplay(&hal_p->g_sContext);
    Graphics_drawString(&hal_p->g_sContext, (int8_t*) "Age: ", -1, 10, 10, true);
    Graphics_drawString(&hal_p->g_sContext, (int8_t*) "Energy: ", -1, 10, 25, true);
    Graphics_drawString(&hal_p->g_sContext, (int8_t*) "Happiness: ", -1, 10, 40, true);
    RectangleGameScreen(hal_p, app_p);
    Graphics_drawImage(&hal_p->g_sContext, &BabyCharacter, 45 + (app_p->x * MOVING_CHARACTER_CONSTANT), 70);

    for (app_p->E = 0; app_p->E < 5; app_p->E++)
    {
        Graphics_drawString(&hal_p->g_sContext, (int8_t*) "*", -1, 55 + (app_p->E * SPACING_CONSTANT), 25, true);
    }
    for (app_p->H = 0; app_p->H < 3; app_p->H++)
    {
        Graphics_drawString(&hal_p->g_sContext, (int8_t*) "*", -1, 75 + (app_p->H * SPACING_CONSTANT), 40, true);
    }
    sprintf(app_p->A_array, "%d", app_p->A);
    Graphics_drawString(&hal_p->g_sContext, (int8_t*) app_p->A_array, -1, 40, 10, true);
}

//Function with the dimensions of the rectangle in game screen.
void RectangleGameScreen(HAL* hal_p, Application* app_p)
{
           Graphics_Rectangle R;
            R.xMin = 10;
            R.xMax = 115;
            R.yMin = 65;
            R.yMax = 115;
            Graphics_drawRectangle(&hal_p->g_sContext, &R);
}

//Function resposible for movement of character when is at the first stage of his life cycle only. When LB1 or LB2 is pressed the character
//moves to left or right respectiveley. A white image is drawn over old image to erease the old position and a new one is drawn in the new
//position by multiplying a moving constant to the number of pressing. Then this number is added to x coordinate
// The function also handles double steps. When the character takes two steps energy is decreased by 1 and happiness is increased by 1
void HandleMovingChild(HAL* hal_p, Application* app_p)
{
    if (app_p->E > 0)
    {
        if (hal_p->buttons.LB1tapped && app_p->x > NUMBER_OF_TAPS_LEFT)
        {
            Graphics_drawImage(&hal_p->g_sContext, &WhiteImage,
                               45 + (app_p->x * MOVING_CHARACTER_CONSTANT), 70);
            app_p->x--;
            Graphics_drawImage(&hal_p->g_sContext, &BabyCharacter,
                               45 + (app_p->x * MOVING_CHARACTER_CONSTANT), 70);
            app_p->steps++;
            handleDoubleStep(hal_p, app_p);
            decreaseEnergy(hal_p, app_p);
        }
        if (hal_p->buttons.LB2tapped && app_p->x < NUMBER_OF_TAPS_RIGHT)
        {
            Graphics_drawImage(&hal_p->g_sContext, &WhiteImage,
                               45 + (app_p->x * MOVING_CHARACTER_CONSTANT), 70);
            app_p->x++;
            Graphics_drawImage(&hal_p->g_sContext, &BabyCharacter,
                               45 + (app_p->x * MOVING_CHARACTER_CONSTANT), 70);
            app_p->steps++;
            handleDoubleStep(hal_p, app_p);
            decreaseEnergy(hal_p, app_p);
        }
    }
}

//Function resposible for movement of character when is at the second stage of his life cycle only. When LB1 or LB2 is pressed the character
//moves to left or right respectiveley. A white image is drawn over old image to erease the old position and a new one is drawn in the new
//position by multiplying a moving constant to the number of pressing. Then this number is added to x coordinate
// The function also handles double steps. When the character takes two steps energy is decreased by 1 and happiness is increased by 1
void HandleMovingTeen(HAL* hal_p, Application* app_p)
{
    if (app_p->E > 0)
    {
        Graphics_drawImage(&hal_p->g_sContext, &Teen_image,
                           45 + (app_p->x * MOVING_CHARACTER_CONSTANT), 70);
        if (hal_p->buttons.LB1tapped && app_p->x > NUMBER_OF_TAPS_LEFT)
        {
            Graphics_drawImage(&hal_p->g_sContext, &WhiteImage,
                               45 + (app_p->x * MOVING_CHARACTER_CONSTANT), 70);
            app_p->x--;
            Graphics_drawImage(&hal_p->g_sContext, &Teen_image,
                               45 + (app_p->x * MOVING_CHARACTER_CONSTANT), 70);
            app_p->steps++;
            handleDoubleStep(hal_p, app_p);
            decreaseEnergy(hal_p, app_p);
        }
        if (hal_p->buttons.LB2tapped && app_p->x < NUMBER_OF_TAPS_RIGHT)
        {
            Graphics_drawImage(&hal_p->g_sContext, &WhiteImage,
                               45 + (app_p->x * MOVING_CHARACTER_CONSTANT), 70);
            app_p->x++;
            Graphics_drawImage(&hal_p->g_sContext, &Teen_image,
                               45 + (app_p->x * MOVING_CHARACTER_CONSTANT), 70);
            app_p->steps++;
            handleDoubleStep(hal_p, app_p);
            decreaseEnergy(hal_p, app_p);
        }
    }
}


//Function resposible for movement of character when is at the last stage of his life cycle only. When LB1 or LB2 is pressed the character
//moves to left or right respectiveley. A white image is drawn over old image to erease the old position and a new one is drawn in the new
//position by multiplying a moving constant to the number of pressing. Then this number is added to x coordinate
// The function also handles double steps. When the character takes two steps energy is decreased by 1 and happiness is increased by 1
void HandleMovingAdult(HAL* hal_p, Application* app_p)
{
    if (app_p->E > 0)
    {
        Graphics_drawImage(&hal_p->g_sContext, &Adult_image,
                                       45 + (app_p->x * MOVING_CHARACTER_CONSTANT), 70);
        if (hal_p->buttons.LB1tapped && app_p->x > NUMBER_OF_TAPS_LEFT)
        {
            Graphics_drawImage(&hal_p->g_sContext, &WhiteImage,
                               45 + (app_p->x * MOVING_CHARACTER_CONSTANT), 70);
            app_p->x--;
            Graphics_drawImage(&hal_p->g_sContext, &Adult_image,
                               45 + (app_p->x * MOVING_CHARACTER_CONSTANT), 70);
            app_p->steps++;
            handleDoubleStep(hal_p, app_p);
            decreaseEnergy(hal_p, app_p);
        }
        if (hal_p->buttons.LB2tapped && app_p->x < NUMBER_OF_TAPS_RIGHT)
        {
            Graphics_drawImage(&hal_p->g_sContext, &WhiteImage,
                               45 + (app_p->x * MOVING_CHARACTER_CONSTANT), 70);
            app_p->x++;
            Graphics_drawImage(&hal_p->g_sContext, &Adult_image,
                               45 + (app_p->x * MOVING_CHARACTER_CONSTANT), 70);
            app_p->steps++;
            handleDoubleStep(hal_p, app_p);
            decreaseEnergy(hal_p, app_p);
        }
    }
}

//Function for handling game screen where main functions responsible for game screen are called. HandlEenergy function is responsible for decreasing
//energy and happiness every 3 seconds. HandleLifeCycle function is responsible for changing the character image whenever he enters a new stage
//HandleGameScore  is responsible for tracking energy and happiness level. Whenever they both reach 0 game ends
void HandleGameScreen(HAL* hal_p, Application* app_p, timerA* time)
{
    HandleEnergy(hal_p, app_p, time);
    HandleLifeCycle(hal_p, app_p, time);
    HandleGameScore(hal_p, app_p);
}

//HandlEenergy function is responsible for energy and happiness every 3 seconds. It also handles increasing age every 3 seconds. Function also handles
//light animation whenever character enters a new stage or age increases by 1. If button 1 on boosterpack is pressed energy level increases by 1.
//Timer value is quarter second. When timer expires a counter variable is incremented. When counter reaches 12 three seconds has passed.
void HandleEnergy(HAL *hal_p, Application *app_p, timerA* time)
{
    if (Timer32_getValue(TIMER32_1_BASE) == 0)
    {

        app_p->counter++;
        Timer32_setCount(TIMER32_1_BASE, QUARTER_SEC_COUNT);
        Timer32_startTimer(TIMER32_1_BASE, true);

    }
    if ((app_p->counter == QUARTER_SEC_COUNTER || app_p->counter == THREE_QUARTERS_SEC_COUNTER)  && app_p->A != 0)
    {
        Timer_A_generatePWM(BLU_TIMER, &time->pwmConfig_blu_set);
    }

   if ((app_p->counter == HALF_SEC_COUNTER || app_p->counter == ONE_SEC_COUNTER) && app_p->A != 0)
    {
         Timer_A_stopTimer(BLU_TIMER);
    }
    if (app_p->counter == THREE_SEC_COUNTER)
    {
        app_p->A++;
        sprintf(app_p->A_array, "%d", app_p->A);
        Graphics_drawString(&hal_p->g_sContext, (int8_t*) app_p->A_array, -1,
                            40, 10, true);
        if (app_p->E > 0)
        {
            app_p->E--;
            decreaseEnergy(hal_p, app_p);
        }
        if (app_p->H > 0)
        {
            app_p->H--;
            decreaseHapiness(hal_p, app_p);
        }

        app_p->counter = 0;
    }

    if (hal_p->buttons.BB1tapped && app_p->E < 5)
    {
        increaseEnergy(hal_p, app_p);
        app_p->E++;
    }
}

//Function that handles when character moves two times. Happiness is increased and energy is decreased
void handleDoubleStep(HAL *hal_p, Application *app_p)
{
    if (app_p->steps == DOUBLE_STEP)
    {
        if (app_p->E > 0)
        {
            app_p->E--;
        }
        if (app_p->H < 5)
        {
            increaseHappiness(hal_p, app_p);
            app_p->H++;
        }
        app_p->steps = 0;
    }
}

//Function that updates energy level on screen whenever it decreases
void decreaseEnergy(HAL *hal_p, Application *app_p)
{
    Graphics_setForegroundColor(&hal_p->g_sContext,
    GRAPHICS_COLOR_WHITE);
    Graphics_drawString(&hal_p->g_sContext, (int8_t*) "*", -1,
                        55 + ((app_p->E) * SPACING_CONSTANT), 25,
                        true);
    Graphics_setForegroundColor(&hal_p->g_sContext,
    GRAPHICS_COLOR_BLACK);
}

//Function that updates energy level on screen whenever it increases
void increaseEnergy(HAL *hal_p, Application *app_p)
{
    Graphics_setForegroundColor(&hal_p->g_sContext, GRAPHICS_COLOR_BLACK);
    Graphics_drawString(&hal_p->g_sContext, (int8_t*) "*", -1,
                        55 + ((app_p->E) * SPACING_CONSTANT), 25, true);
}

//Function that updates happiness level on screen whenever it increases
void increaseHappiness(HAL* hal_p, Application* app_p)
{
    Graphics_setForegroundColor(&hal_p->g_sContext,
    GRAPHICS_COLOR_BLACK);
    Graphics_drawString(&hal_p->g_sContext, (int8_t*) "*", -1,
                        75 + ((app_p->H) * SPACING_CONSTANT), 40,
                        true);
}

//Function that updates happiness level on screen whenever it decreases
void decreaseHapiness(HAL* hal_p, Application* app_p)
{
    Graphics_setForegroundColor(&hal_p->g_sContext,
    GRAPHICS_COLOR_WHITE);
    Graphics_drawString(&hal_p->g_sContext, (int8_t*) "*", -1,
                        75 + ((app_p->H) * SPACING_CONSTANT), 40,
                        true);
    Graphics_setForegroundColor(&hal_p->g_sContext,
    GRAPHICS_COLOR_BLACK);
}

//when energy level and happiness level both reach zero game ends. Screen changes to result screen
void HandleGameScore(HAL* hal_p, Application* app_p)
{
    if (app_p->E == 0 && app_p->H ==0)
    {
        app_p->state = RESULT_SCREEN;
        showResultScreen(hal_p,app_p);

    }
}

//Graphics display for result screen. A text is displayed about the character and age reached by player is displayed on screen.
//An image of character is also displayed
void showResultScreen(HAL* hal_p, Application* app_p)
{
    Graphics_clearDisplay(&hal_p->g_sContext);
    Graphics_drawString(&hal_p->g_sContext, (int8_t*) "Your Tamogatchi", -1, 15, 20, true);
    Graphics_drawString(&hal_p->g_sContext, (int8_t*) "packed its bags", -1, 15, 30, true);
    Graphics_drawString(&hal_p->g_sContext, (int8_t*) "and left....", -1, 15, 40, true);
    Graphics_drawString(&hal_p->g_sContext, (int8_t*) "Age: ", -1, 40, 60, true);
    Graphics_drawString(&hal_p->g_sContext, (int8_t*) app_p->A_array, -1, 65, 60, true);
    Graphics_drawString(&hal_p->g_sContext, (int8_t*) "Play again? (BB1)", -1, 10, 110, true);
    Graphics_drawImage(&hal_p->g_sContext, &Result_screen_image, 40, 70);
}

//Function that handles different life stages of the character. Depending on the game score the function will choose between
//our three functions that handles changing our character appearance. Whenever the character enters a new stage in his life
//a horn sound is produced through the buzzer and a red blinking light for animation.
void HandleLifeCycle(HAL *hal_p, Application *app_p, timerA *time)
{
    if (app_p->J == 1)
    {
        HandleMovingChild(hal_p, app_p);
    }
    if ((app_p->E >= 3 && app_p->H >= 4 && app_p->A >= 3) || (app_p->l == 1))
    {
        HandleMovingTeen(hal_p, app_p);
        if (app_p->f == 0)
        {
            Timer_A_generatePWM(BUZZ_TIMER, &time->pwmConfig_buzz_set);
            int i;
            for (i = 0; i < 300000; i++)
                ;
            Timer_A_generatePWM(BUZZ_TIMER, &time->pwmConfig_buzz_reset);
            app_p->f = 1;
        }
        app_p->J = 2;
        app_p->l = 1;
    }
    if (app_p->E >= 2 && app_p->H >= 2 && app_p->A >= 7 && app_p->J == 2
            || app_p->l == 3)
    {

        HandleMovingAdult(hal_p, app_p);
        if (app_p->f == 1)
        {
            Timer_A_generatePWM(BUZZ_TIMER, &time->pwmConfig_buzz_set);
            int i;
            for (i = 0; i < 300000; i++)
                ;
            Timer_A_generatePWM(BUZZ_TIMER, &time->pwmConfig_buzz_reset);
            app_p->f = 2;
        }
        app_p->l = 3;
    }

}

//handling result screen. If button 1 on booster pack is pressed the screen changes to instruction screen again. Some variables are reset to their
//initial values again since they are going to be used in the new game. So we want everything back to normal.
void HandleResultScreen(HAL* hal_p, Application* app_p)
{
    if (hal_p->buttons.BB1tapped)
    {
        app_p->state = INSTRUCTIONS_SCREEN;
        showInstructionScreen(hal_p, app_p);

        //Resetting values
        app_p->A = 0;
        app_p->J = 1;
        app_p->l = 0;
        app_p->steps = 0;
    }
}

//initialization of ports and pins for RB LED and Buzzer on booster pack
void initPWMPins()
{
    // For the Blue LED, we first configure it as a GPIO and turn it off
    // Then, we configure it to work as output channel of a timer
    // The blue LED on booster is wried to Port 5, Pin 6, we cannot change this as the programmer
    // BLUE LED
    GPIO_setAsOutputPin    (GPIO_PORT_P5,    GPIO_PIN6);
    GPIO_setOutputLowOnPin (GPIO_PORT_P5,    GPIO_PIN6);
    GPIO_setAsPeripheralModuleFunctionOutputPin(
            GPIO_PORT_P5,
            GPIO_PIN6,
            GPIO_PRIMARY_MODULE_FUNCTION);

    //Buzzer pins and ports
    GPIO_setAsOutputPin(GPIO_PORT_P2, GPIO_PIN7);
    GPIO_setOutputLowOnPin(GPIO_PORT_P2, GPIO_PIN7);
    GPIO_setAsPeripheralModuleFunctionOutputPin(
    GPIO_PORT_P2,
                                                GPIO_PIN7,
                                                GPIO_PRIMARY_MODULE_FUNCTION);

}

//Setting the values for our blue LED
void TimerA_SetValues(timerA* time)
{
    time->pwmConfig_blu_set.clockSource = TIMER_A_CLOCKSOURCE_SMCLK; // The source clock is the system clock (3MHz)
    time->pwmConfig_blu_set.clockSourceDivider = TIMER_A_CLOCKSOURCE_DIVIDER_24;
    time->pwmConfig_blu_set.compareOutputMode = TIMER_A_OUTPUTMODE_SET_RESET; // The OC mode. We choose this such that we can achieve the waveform we are interested in.
    time->pwmConfig_blu_set.compareRegister = BLU_CHANNEL; // The output register used for OC operation. The programmer does not choose this.
    time->pwmConfig_blu_set.timerPeriod = PWM_PERIOD_CYCLES_BLUE; // The # of counter cycles in one PWM round (period of PWM in terms of counter cycle)
    time->pwmConfig_blu_set.dutyCycle = BLU_COMPARE_CYCLES; // The OC value. This is the value that when the counter hits it, something happens to the waveform

}

//Setting values needed for buzzer to make a sound
void Buzz_SetValues(timerA* time)
{
    time->pwmConfig_buzz_set.clockSource = TIMER_A_CLOCKSOURCE_SMCLK; // The source clock is the system clock (3MHz)
    time->pwmConfig_buzz_set.clockSourceDivider = TIMER_A_CLOCKSOURCE_DIVIDER_24;
    time->pwmConfig_buzz_set.compareOutputMode = TIMER_A_OUTPUTMODE_SET_RESET; // The OC mode. We choose this such that we can achieve the waveform we are interested in.
    time->pwmConfig_buzz_set.compareRegister = BUZZ_CHANNEL; // The output register used for OC operation. The programmer does not choose this.
    time->pwmConfig_buzz_set.timerPeriod = PWM_PERIOD_CYCLES_BUZZ; // The # of counter cycles in one PWM round (period of PWM in terms of counter cycle)
    time->pwmConfig_buzz_set.dutyCycle = BUZZ_COMPARE_CYCLES; // The OC value. This is the value that when the counter hits it, something happens to the waveform
}

//resetting values for buzzer so that we can turn it off
void Buzz_ResetValues(timerA* time)
{
    time->pwmConfig_buzz_reset.clockSource = TIMER_A_CLOCKSOURCE_SMCLK; // The source clock is the system clock (3MHz)
    time->pwmConfig_buzz_reset.clockSourceDivider = TIMER_A_CLOCKSOURCE_DIVIDER_24;
    time->pwmConfig_buzz_reset.compareOutputMode = TIMER_A_OUTPUTMODE_SET_RESET; // The OC mode. We choose this such that we can achieve the waveform we are interested in.
    time->pwmConfig_buzz_reset.compareRegister = BUZZ_CHANNEL; // The output register used for OC operation. The programmer does not choose this.
    time->pwmConfig_buzz_reset.timerPeriod = 0; // The # of counter cycles in one PWM round (period of PWM in terms of counter cycle)
    time->pwmConfig_buzz_reset.dutyCycle = BUZZ_COMPARE_CYCLES; // The OC value. This is the value that when the counter hits it, something happens to the waveform

}

//For printing characters on UART screen. When a character is pressed "L" is displayed. When anything else including backspace is pressed
//"0" is displayed.
char Application_interpretIncomingChar(char rxChar)
{
    // The character to return back to sender. By default, we assume the letter
    // to send back is an 'O' (assume the character is an "other" character)
    char txChar = 'O';

    // Numbers - if the character entered was a number, transfer back an 'N'
    if (rxChar >= '0' && rxChar <= '9') {
        txChar = 'N';
    }

    // Letters - if the character entered was a letter, transfer back an 'L'
    if ((rxChar >= 'a' && rxChar <= 'z') || (rxChar >= 'A' && rxChar <= 'Z')) {
        txChar = 'L';
    }

    if (rxChar == '\b')
    {
        txChar = '\b';
    }
    return (txChar);
}

//Function to get character from UART.
void GetChar_word(Application *app_p, HAL *hal_p)
{
    if (UART_hasChar(&hal_p->uart))
    {
        // The character received from your serial terminal

        char rxChar = UART_getChar(&hal_p->uart);

        char txChar = Application_interpretIncomingChar(rxChar);

        // Only send a character if the UART module can send it
        if (UART_canSend(&hal_p->uart))
            UART_sendChar(&hal_p->uart, txChar);

        //printing text from UART on screen as uppercase letters only
        if (txChar == 'L')
        {
            if (rxChar >= 'a' && rxChar <= 'z')
            {
                rxChar = rxChar - 32;
            }
            app_p->RXchar = rxChar;
        }
        //Logic for backspace
        if (txChar == '\b')
        {
            if(app_p->char_count_P1 != 0)
            {
            app_p->char_count_P1--;
            app_p->BSword[app_p->char_count_P1] = '_';
            }
        }
    }
}

//This function updates the screen with the character name every time a new character is entered
void updateNameScreen(HAL* hal_p, Application* app_p)
{
    Graphics_drawString(&hal_p->g_sContext, (int8_t*) app_p->BSword, 6, 30, 17, true);
}

/**
 * A helper function which increments a value with a maximum. If incrementing
 * the number causes the value to hit its maximum, the number wraps around
 * to 0.
 */
uint32_t CircularIncrement(uint32_t value, uint32_t maximum)
{
    return (value + 1) % maximum;
}

/**
 * Updates which LEDs are lit and what baud rate the UART module communicates
 * with, based on what the application's baud choice is at the time this
 * function is called.
 *
 * @param app_p:  A pointer to the main Application object.
 * @param hal_p:  A pointer to the main HAL object
 */
void Application_updateCommunications(Application* app_p, HAL* hal_p)
{
    // When this application first loops, the proper LEDs aren't lit. The
    // firstCall flag is used to ensure that the
        uint32_t newBaudNumber = CircularIncrement((uint32_t) app_p->baudChoice, NUM_BAUD_CHOICES);
        app_p->baudChoice = (UART_Baudrate) newBaudNumber;

    // Start/update the baud rate according to the one set above.
    UART_SetBaud_Enable(&hal_p->uart, app_p->baudChoice);
}
