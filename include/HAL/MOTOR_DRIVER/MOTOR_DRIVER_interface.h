/*
 * MOTOR_DRIVER_interface.h
 *
 *  Created on: Sep 3, 2026
 *      Author: Mohammed Saeed
 */

#ifndef MOTOR_DRIVER_INTERFACE_H_
#define MOTOR_DRIVER_INTERFACE_H_

#define MOTOR_1         1
#define MOTOR_2         2
#define MOTOR_3         3
#define MOTOR_4         4

#define DIR_FORWARD     1
#define DIR_BACKWARD    2
#define DIR_STOP        3

void MOTOR_SHIELD_Init(void);
void MOTOR_SHIELD_SetMotor(u8 Copy_u8MotorID, u8 Copy_u8Direction, u8 Copy_u8Speed);
void MOTOR_SHIELD_MoveForward(u8 Copy_u8Speed);
void MOTOR_SHIELD_MoveBackward(u8 Copy_u8Speed);
void MOTOR_SHIELD_TurnRight(u8 Copy_u8Speed);
void MOTOR_SHIELD_TurnLeft(u8 Copy_u8Speed);
void MOTOR_SHIELD_Stop(void);

#endif /* MOTOR_DRIVER_INTERFACE_H_ */
