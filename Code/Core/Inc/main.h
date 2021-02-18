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
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
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
#define A1_Pin GPIO_PIN_14
#define A1_GPIO_Port GPIOC
#define A2_Pin GPIO_PIN_15
#define A2_GPIO_Port GPIOC
#define A3_Pin GPIO_PIN_0
#define A3_GPIO_Port GPIOD
#define A4_Pin GPIO_PIN_1
#define A4_GPIO_Port GPIOD
#define POWER_Pin GPIO_PIN_2
#define POWER_GPIO_Port GPIOA
#define B1_Pin GPIO_PIN_5
#define B1_GPIO_Port GPIOC
#define B2_Pin GPIO_PIN_0
#define B2_GPIO_Port GPIOB
#define B3_Pin GPIO_PIN_1
#define B3_GPIO_Port GPIOB
#define B4_Pin GPIO_PIN_2
#define B4_GPIO_Port GPIOB
#define CS_SD_Pin GPIO_PIN_6
#define CS_SD_GPIO_Port GPIOC
#define A5_Pin GPIO_PIN_8
#define A5_GPIO_Port GPIOC
#define A6_Pin GPIO_PIN_9
#define A6_GPIO_Port GPIOC
#define A7_Pin GPIO_PIN_9
#define A7_GPIO_Port GPIOA
#define A8_Pin GPIO_PIN_10
#define A8_GPIO_Port GPIOA
#define REC_Pin GPIO_PIN_11
#define REC_GPIO_Port GPIOA
#define CS_LCD_Pin GPIO_PIN_6
#define CS_LCD_GPIO_Port GPIOB
#define D_C_LCD_Pin GPIO_PIN_7
#define D_C_LCD_GPIO_Port GPIOB
#define RESET_LCD_Pin GPIO_PIN_8
#define RESET_LCD_GPIO_Port GPIOB
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
