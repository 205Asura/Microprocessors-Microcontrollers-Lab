/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
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
#include "stm32f1xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define LED_A2_Pin GPIO_PIN_2
#define LED_A2_GPIO_Port GPIOA
#define LED_A3_Pin GPIO_PIN_3
#define LED_A3_GPIO_Port GPIOA
#define LED_A4_Pin GPIO_PIN_4
#define LED_A4_GPIO_Port GPIOA
#define LED_A5_Pin GPIO_PIN_5
#define LED_A5_GPIO_Port GPIOA
#define LED_RED_Pin GPIO_PIN_0
#define LED_RED_GPIO_Port GPIOB
#define LED_GREEN_Pin GPIO_PIN_1
#define LED_GREEN_GPIO_Port GPIOB
#define LED_YELLOW_Pin GPIO_PIN_2
#define LED_YELLOW_GPIO_Port GPIOB
#define SEG0_TIMER0_Pin GPIO_PIN_3
#define SEG0_TIMER0_GPIO_Port GPIOB
#define SEG1_TIMER0_Pin GPIO_PIN_4
#define SEG1_TIMER0_GPIO_Port GPIOB
#define SEG2_TIMER0_Pin GPIO_PIN_5
#define SEG2_TIMER0_GPIO_Port GPIOB
#define SEG3_TIMER0_Pin GPIO_PIN_6
#define SEG3_TIMER0_GPIO_Port GPIOB
#define SEG4_TIMER0_Pin GPIO_PIN_7
#define SEG4_TIMER0_GPIO_Port GPIOB
#define SEG5_TIMER0_Pin GPIO_PIN_8
#define SEG5_TIMER0_GPIO_Port GPIOB
#define SEG6_TIMER0_Pin GPIO_PIN_9
#define SEG6_TIMER0_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */
//#define SCH_REPORT_ERRORS

#ifdef SCH_REPORT_ERRORS
#define Error_port GPIOA
#endif


/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
