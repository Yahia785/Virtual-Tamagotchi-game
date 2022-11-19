/*
 * Functions.h
 *
 *  Created on: Apr 21, 2022
 *      Author: Yahia Tawfik
 */

#ifndef FUNCTIONS_H_
#define FUNCTIONS_H_


void initialize();
void main_loop(HAL* hal_p, Application* app_p, timerA* time);
void sleep();
void showTitleScreen(HAL* HAL_P, Application* app_p);
void handleTitleScreen(HAL *hal_p, Application *app_p);
void showInstructionScreen(HAL* hal_p, Application* app_p);
void TimerExpired();
void showGameScreen(HAL* hal_p, Application* app_p);
void handleInstructionScreen (HAL* hal_p, Application* app_p);
void RectangleGameScreen(HAL* hal_p, Application* app_p);
void HandleMovingChild(HAL* hal_p, Application* app_p);
void HandleMovingAdult(HAL* hal_p, Application* app_p);
void HandleMovingTeen(HAL* hal_p, Application* app_p);
void HandleLifeCycle(HAL* hal_p, Application* app_p, timerA* time);
void HandleGameScreen(HAL* hal_p, Application* app_p, timerA* time);
void HandleEnergy(HAL *hal_p, Application *app_p, timerA* time);
void handleDoubleStep(HAL* hal_p, Application* app_p);
void decreaseEnergy(HAL *hal_p, Application *app_p);
void increaseEnergy(HAL* hal_p, Application* app_p);
void increaseHappiness(HAL* hal_p, Application* app_p);
void decreaseHapiness(HAL* hal_p, Application* app_p);
void showResultScreen(HAL* hal_p, Application* app_p);
void HandleGameScore(HAL* hal_p, Application* app_p);
void HandleResultScreen(HAL* hal_p, Application* app_p);
void initPWMPins();
void TimerA_SetValues(timerA* time);
void Buzz_ResetValues(timerA* time);
void Buzz_SetValues(timerA* time);
void GetChar_word(Application *app_p, HAL *hal_p);
void ShowNameScreen(HAL* hal_p, Application* app_p);
void handleNameScreen(HAL *hal_p, Application *app_p);
void updateNameScreen(HAL* hal_p, Application* app_p);
void Application_updateCommunications(Application* app_p, HAL* hal_p);

#endif /* FUNCTIONS_H_ */
