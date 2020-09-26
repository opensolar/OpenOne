/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
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
  uint8_t spare;
} CONTROL;
enum control_bits { SPARE1 = 0x02, STANDBY = 0x01 };


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
#define HALL0_Pin GPIO_PIN_0
#define HALL0_GPIO_Port GPIOA
#define HALL1_Pin GPIO_PIN_1
#define HALL1_GPIO_Port GPIOA
#define HALL2_Pin GPIO_PIN_4
#define HALL2_GPIO_Port GPIOA
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
