/*
 * motor.h
 *
 *  Created on: Sep 14, 2020
 *      Author: User
 */

#ifndef INC_MOTOR_H_
#define INC_MOTOR_H_

void drive_pitch_motor(MOTOR *mot);
void drive_roll_motor(MOTOR *mot);
void enable_motors(bool enable);

#endif /* INC_MOTOR_H_ */
