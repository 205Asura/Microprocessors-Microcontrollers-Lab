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
#define EN0_Pin GPIO_PIN_13
#define EN0_GPIO_Port GPIOC
#define EN1_Pin GPIO_PIN_14
#define EN1_GPIO_Port GPIOC
#define SEG0_MODE_Pin GPIO_PIN_0
#define SEG0_MODE_GPIO_Port GPIOA
#define SEG1_MODE_Pin GPIO_PIN_1
#define SEG1_MODE_GPIO_Port GPIOA
#define SEG2_MODE_Pin GPIO_PIN_2
#define SEG2_MODE_GPIO_Port GPIOA
#define SEG3_MODE_Pin GPIO_PIN_3
#define SEG3_MODE_GPIO_Port GPIOA
#define SEG4_MODE_Pin GPIO_PIN_4
#define SEG4_MODE_GPIO_Port GPIOA
#define SEG5_MODE_Pin GPIO_PIN_5
#define SEG5_MODE_GPIO_Port GPIOA
#define SEG6_MODE_Pin GPIO_PIN_6
#define SEG6_MODE_GPIO_Port GPIOA
#define LED7_Pin GPIO_PIN_7
#define LED7_GPIO_Port GPIOA
#define LED_RED0_Pin GPIO_PIN_0
#define LED_RED0_GPIO_Port GPIOB
#define LED_GREEN0_Pin GPIO_PIN_1
#define LED_GREEN0_GPIO_Port GPIOB
#define LED_YELLOW0_Pin GPIO_PIN_2
#define LED_YELLOW0_GPIO_Port GPIOB
#define SEG1_TIMER1_Pin GPIO_PIN_10
#define SEG1_TIMER1_GPIO_Port GPIOB
#define SEG2_TIMER1_Pin GPIO_PIN_11
#define SEG2_TIMER1_GPIO_Port GPIOB
#define SEG3_TIMER1_Pin GPIO_PIN_12
#define SEG3_TIMER1_GPIO_Port GPIOB
#define SEG4_TIMER1_Pin GPIO_PIN_13
#define SEG4_TIMER1_GPIO_Port GPIOB
#define SEG5_TIMER1_Pin GPIO_PIN_14
#define SEG5_TIMER1_GPIO_Port GPIOB
#define SEG6_TIMER1_Pin GPIO_PIN_15
#define SEG6_TIMER1_GPIO_Port GPIOB
#define SEG0_TIMER0_Pin GPIO_PIN_8
#define SEG0_TIMER0_GPIO_Port GPIOA
#define SEG1_TIMER0_Pin GPIO_PIN_9
#define SEG1_TIMER0_GPIO_Port GPIOA
#define SEG2_TIMER0_Pin GPIO_PIN_10
#define SEG2_TIMER0_GPIO_Port GPIOA
#define SEG3_TIMER0_Pin GPIO_PIN_11
#define SEG3_TIMER0_GPIO_Port GPIOA
#define SEG4_TIMER0_Pin GPIO_PIN_12
#define SEG4_TIMER0_GPIO_Port GPIOA
#define SEG5_TIMER0_Pin GPIO_PIN_13
#define SEG5_TIMER0_GPIO_Port GPIOA
#define SEG6_TIMER0_Pin GPIO_PIN_14
#define SEG6_TIMER0_GPIO_Port GPIOA
#define LED_RED1_Pin GPIO_PIN_3
#define LED_RED1_GPIO_Port GPIOB
#define LED_GREEN1_Pin GPIO_PIN_4
#define LED_GREEN1_GPIO_Port GPIOB
#define LED_YELLOW1_Pin GPIO_PIN_5
#define LED_YELLOW1_GPIO_Port GPIOB
#define BUTTON_0_Pin GPIO_PIN_6
#define BUTTON_0_GPIO_Port GPIOB
#define BUTTON_1_Pin GPIO_PIN_7
#define BUTTON_1_GPIO_Port GPIOB
#define BUTTON_2_Pin GPIO_PIN_8
#define BUTTON_2_GPIO_Port GPIOB
#define SEG0_TIMER1_Pin GPIO_PIN_9
#define SEG0_TIMER1_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */




/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
