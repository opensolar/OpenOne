/*
 * motor.c
 *
 *  Created on: Sep 14, 2020
 *      Author: Martin Lafleur
 */
#include "main.h"
#include "motor.h"


TIM_HandleTypeDef htim2;
void enable_motors(bool enable){

  if(enable){
    HAL_GPIO_WritePin(MOTOR_STBY_GPIO_Port, MOTOR_STBY_Pin, GPIO_PIN_SET);
  } else {
    HAL_GPIO_WritePin(MOTOR_STBY_GPIO_Port, MOTOR_STBY_Pin, GPIO_PIN_RESET);
  }

}



void drive_roll_motor(MOTOR *mot){
  uint16_t period = 65535;
  uint16_t pulse = (uint32_t)period * mot->speed / 65536;

  switch(mot->mode){
  case BRAKE:
    HAL_GPIO_WritePin(ROLL_CTL1_GPIO_Port, ROLL_CTL1_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(ROLL_CTL2_GPIO_Port, ROLL_CTL2_Pin, GPIO_PIN_SET);
    break;
  case CW:
    setPWM(TIM_CHANNEL_1, period, pulse);
    HAL_GPIO_WritePin(ROLL_CTL1_GPIO_Port, ROLL_CTL1_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(ROLL_CTL2_GPIO_Port, ROLL_CTL2_Pin, GPIO_PIN_SET);
    break;
  case CCW:
    setPWM(TIM_CHANNEL_1, period, pulse);
    HAL_GPIO_WritePin(ROLL_CTL1_GPIO_Port, ROLL_CTL1_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(ROLL_CTL2_GPIO_Port, ROLL_CTL2_Pin, GPIO_PIN_RESET);
    break;
  default:
    // coast
    HAL_GPIO_WritePin(ROLL_CTL1_GPIO_Port, ROLL_CTL1_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(ROLL_CTL2_GPIO_Port, ROLL_CTL2_Pin, GPIO_PIN_RESET);
  }
}

void drive_pitch_motor(MOTOR *mot){
  uint16_t period = 32000;
  uint16_t pulse = (uint32_t)period * mot->speed / 65536;

  switch(mot->mode){
  case BRAKE:
    HAL_GPIO_WritePin(PITCH_CTL1_GPIO_Port, PITCH_CTL1_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(PITCH_CTL2_GPIO_Port, PITCH_CTL2_Pin, GPIO_PIN_SET);
    break;
  case CW:
    setPWM(TIM_CHANNEL_2, period, pulse);
    HAL_GPIO_WritePin(PITCH_CTL1_GPIO_Port, PITCH_CTL1_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(PITCH_CTL2_GPIO_Port, PITCH_CTL2_Pin, GPIO_PIN_SET);
    break;
  case CCW:
    setPWM(TIM_CHANNEL_2, period, pulse);
    HAL_GPIO_WritePin(PITCH_CTL1_GPIO_Port, PITCH_CTL1_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(PITCH_CTL2_GPIO_Port, PITCH_CTL2_Pin, GPIO_PIN_RESET);
    break;
  default:
    // coast
    HAL_GPIO_WritePin(PITCH_CTL1_GPIO_Port, PITCH_CTL1_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(PITCH_CTL2_GPIO_Port, PITCH_CTL2_Pin, GPIO_PIN_RESET);
  }
}


