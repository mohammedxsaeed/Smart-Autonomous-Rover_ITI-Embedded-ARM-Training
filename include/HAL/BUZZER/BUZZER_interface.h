/*
 * BUZZER_interface.h
 *
 *  Created on: Sep 8, 2026
 *      Author: Mohammed Saeed
 */

#ifndef BUZZER_INTERFACE_H_
#define BUZZER_INTERFACE_H_


void BUZZER_Init(void);
void BUZZER_On(void);
void BUZZER_Off(void);
void BUZZER_Beep(u32 Copy_u32DelayMs);
// Using Delay
void BUZZER_SoundStartup(void);
void BUZZER_SoundReversing(void);
void BUZZER_SoundObstacleAlert(void);
void BUZZER_SoundModeSwitch(void);
// Using OS
void BUZZER_PlayStartup(void);
void BUZZER_PlayReversing(void);
void BUZZER_PlayAlert(void);
void BUZZER_PlayModeSwitch(void);

void BUZZER_Task(void);

#endif /* BUZZER_INTERFACE_H_ */
