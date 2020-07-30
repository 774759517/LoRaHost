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
#include "stm32f0xx_hal.h"

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
#define SPI1_NSS_Pin GPIO_PIN_4
#define SPI1_NSS_GPIO_Port GPIOA
#define DIO_5_Pin GPIO_PIN_0
#define DIO_5_GPIO_Port GPIOB
#define DIO_4_Pin GPIO_PIN_1
#define DIO_4_GPIO_Port GPIOB
#define DIO_3_Pin GPIO_PIN_2
#define DIO_3_GPIO_Port GPIOB
#define DIO_2_Pin GPIO_PIN_10
#define DIO_2_GPIO_Port GPIOB
#define DIO_1_Pin GPIO_PIN_11
#define DIO_1_GPIO_Port GPIOB
#define DIO_0_Pin GPIO_PIN_12
#define DIO_0_GPIO_Port GPIOB
#define SPI1_RST_Pin GPIO_PIN_13
#define SPI1_RST_GPIO_Port GPIOB
#define VC1_Pin GPIO_PIN_14
#define VC1_GPIO_Port GPIOB
#define D1_Pin GPIO_PIN_15
#define D1_GPIO_Port GPIOA
#define D2_Pin GPIO_PIN_3
#define D2_GPIO_Port GPIOB
#define D3_Pin GPIO_PIN_4
#define D3_GPIO_Port GPIOB
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
