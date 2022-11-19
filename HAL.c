/*
 * HAL.c
 *
 *  Created on: Mar 30, 2022
 *      Author: Yahia Tawfik
 */
#include <HAL/HAL.h>

void Init_LCD();
void InitGraphics(Graphics_Context *g_sContext_p);
/**
 * Constructs a new API object. The API constructor should simply call the constructors of each
 * of its sub-members with the proper inputs.
 *
 * @return a properly constructed API object.
 */
HAL HAL_construct()
{
    // The API object which will be returned at the end of construction
    HAL hal;

    // Construct the UART module inside of this HAL struct
    hal.uart = UART_construct(USB_UART_INSTANCE, USB_UART_PORT, USB_UART_PINS);

    // Enable the UART at 9600 BPS
    // TODO: Call UART_SetBaud_Enable to achieve the above goal
    UART_SetBaud_Enable(&hal.uart, BAUD_9600);

    //Calling graphics function
    InitGraphics(&hal.g_sContext);

    // Once we have finished building the API, return the completed struct.
    return hal;
}

/**
 * Upon every new cycle of the main super-loop, we MUST UPDATE the status of
 * all inputs. In this program, this function is called only once in the
 * Application_loop() function. Since the Application_loop() function is called
 * once per loop of the while (true) loop in main, we are effectively polling
 * all inputs once per loop.
 *
 * @param hal:  The API whose input modules we wish to refresh
 */
void HAL_refresh(HAL* hal)
{
    hal->buttons = updateButtons();

}

// This function initializes the the LCD screen including setting up the SPI connection
void Init_LCD() {
    Crystalfontz128x128_Init();
    Crystalfontz128x128_SetOrientation(LCD_ORIENTATION_UP);
}

// This function initializes the graphics
void InitGraphics(Graphics_Context *g_sContext_p) {

    Graphics_initContext(g_sContext_p,
                         &g_sCrystalfontz128x128,
                         &g_sCrystalfontz128x128_funcs);
    Graphics_setForegroundColor(g_sContext_p, GRAPHICS_COLOR_BLACK);
    Graphics_setBackgroundColor(g_sContext_p, GRAPHICS_COLOR_WHITE);
    Graphics_setFont(g_sContext_p, &g_sFontFixed6x8);

    Init_LCD();

    Graphics_clearDisplay(g_sContext_p);
}



