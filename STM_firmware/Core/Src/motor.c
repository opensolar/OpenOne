//  _____   _____  _______ __   _ _______  _____         _______  ______   _____  _____
// |     | |_____] |______ | \  | |______ |     | |      |_____| |_____/     |   |     |
// |_____| |       |______ |  \_| ______| |_____| |_____ |     | |    \_ . __|__ |_____|

// Author: Martin Lafleur (mlafleur@opensolar.io)
// Date: Sep 2020
// Project: OpenOne
//
// Copyright 2020 opensolar.io
//
// Permission is hereby granted, free of charge, to any person obtaining a copy of this software and
// associated documentation files (the "Software"), to deal in the Software without restriction,
// including without limitation the rights to use, copy, modify, merge, publish, distribute,
// sublicense, and/or sell copies of the Software, and to permit persons to whom the Software
// is furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
// INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
// IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
// WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
// SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
//
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


