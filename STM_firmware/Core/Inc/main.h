/* USER CODE BEGIN Header */
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
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32l0xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdbool.h>
#include <stdint.h>
/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */
enum motor_modes { BRAKE = 0, CW = 1, CCW = 2, COAST = 3};

typedef struct{
  uint8_t mode; // see motor_modes enum
  uint8_t spare;
  uint16_t speed;   // 65535 = 99.9%
} MOTOR;

typedef struct{
  uint8_t digit[2];
  uint8_t bits;
  uint8_t watchdog;
} CONTROL;
enum control_bits { RUN = 0x01, RED_LED = 0x02 };


typedef struct {
  MOTOR motor[2];
  CONTROL control;
} SPI_RX_FRAME;

typedef struct{
  uint16_t hall[4];
  uint16_t status;
  uint16_t spare;
} SPI_TX_FRAME;

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

void HAL_TIM_MspPostInit(TIM_HandleTypeDef *htim);

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */
void setPWM(uint32_t channel, uint16_t period, uint16_t pulse);

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define HALL4_Pin GPIO_PIN_0
#define HALL4_GPIO_Port GPIOA
#define HALL2_Pin GPIO_PIN_1
#define HALL2_GPIO_Port GPIOA
#define HALL1_Pin GPIO_PIN_4
#define HALL1_GPIO_Port GPIOA
#define HALL3_Pin GPIO_PIN_5
#define HALL3_GPIO_Port GPIOA
#define PITCH_CTL1_Pin GPIO_PIN_6
#define PITCH_CTL1_GPIO_Port GPIOA
#define PITCH_CTL2_Pin GPIO_PIN_7
#define PITCH_CTL2_GPIO_Port GPIOA
#define PITCH_PWM_Pin GPIO_PIN_0
#define PITCH_PWM_GPIO_Port GPIOB
#define MOTOR_STBY_Pin GPIO_PIN_1
#define MOTOR_STBY_GPIO_Port GPIOB
#define ROLL_PWM_Pin GPIO_PIN_8
#define ROLL_PWM_GPIO_Port GPIOA
#define ROLL_CTL2_Pin GPIO_PIN_9
#define ROLL_CTL2_GPIO_Port GPIOA
#define ROLL_CTL1_Pin GPIO_PIN_10
#define ROLL_CTL1_GPIO_Port GPIOA
#define RED_LED_Pin GPIO_PIN_12
#define RED_LED_GPIO_Port GPIOA
#define BLUE_LED_Pin GPIO_PIN_6
#define BLUE_LED_GPIO_Port GPIOB
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
