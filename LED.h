/*
 * LED.h
 *
 *  Created on: Dec 29, 2019
 *      Author: Matthew Zhong
 */

#ifndef HAL_LED_H_
#define HAL_LED_H_

#include <ti/devices/msp432p4xx/driverlib/driverlib.h>
#define LAUNCHPAD_LED1_PORT         GPIO_PORT_P1
#define LAUNCHPAD_LED1_PIN          GPIO_PIN0

#define LAUNCHPAD_LED2_RED_PORT     GPIO_PORT_P2
#define LAUNCHPAD_LED2_RED_PIN      GPIO_PIN0

#define LAUNCHPAD_LED2_GREEN_PORT   GPIO_PORT_P2
#define LAUNCHPAD_LED2_GREEN_PIN    GPIO_PIN1

#define LAUNCHPAD_LED2_BLUE_PORT    GPIO_PORT_P2
#define LAUNCHPAD_LED2_BLUE_PIN     GPIO_PIN2

#define BOOSTERPACK_LED_RED_PORT    GPIO_PORT_P2
#define BOOSTERPACK_LED_RED_PIN     GPIO_PIN6

#define BOOSTERPACK_LED_GREEN_PORT  GPIO_PORT_P2
#define BOOSTERPACK_LED_GREEN_PIN   GPIO_PIN4

#define BOOSTERPACK_LED_BLUE_PORT   GPIO_PORT_P5
#define BOOSTERPACK_LED_BLUE_PIN    GPIO_PIN6

struct _LED
{
    uint8_t port;
    uint16_t pin;
    bool isLit;
};
typedef struct _LED LED;

void initLEDs();

void TurnOn_LL1();
void TurnOff_LL1();
void Toggle_LL1();

void TurnOn_LLB();
void TurnOff_LLB();
void Toggle_LLB();

void TurnOn_LLR();
void TurnOff_LLR();
void Toggle_LLR();

void TurnOn_LLG();
void TurnOff_LLG();
void Toggle_LLG();

void TurnOn_BLR();
void TurnOff_BLR();
void Toggle_BLR();


void TurnOn_BLG();
void TurnOff_BLG();
void Toggle_BLG();


void TurnOn_BLB();
void TurnOff_BLB();
void Toggle_BLB();



#endif /* HAL_LED_H_ */
