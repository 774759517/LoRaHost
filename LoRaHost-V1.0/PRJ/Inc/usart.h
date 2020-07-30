/**
  ******************************************************************************
  * File Name          : USART.h
  * Description        : This file provides code for the configuration
  *                      of the USART instances.
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
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __usart_H
#define __usart_H
#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* USER CODE BEGIN Includes */
#include "user_dataType.h"
#include "stdio.h"
#include "string.h"
#include "cmsis_os.h"
#include "queue.h"
/* USER CODE END Includes */

extern UART_HandleTypeDef huart1;
extern UART_HandleTypeDef huart2;

/* USER CODE BEGIN Private defines */
#define USER_DEBUG
#ifdef 	USER_DEBUG
#define user_info(format,...)		printf("[user]info:"format "\r\n",##__VA_ARGS__)
#define user_debug(format,...)		printf("[user]debug:"format "\r\n",##__VA_ARGS__)
#define user_err(format,...)		printf("[user]err:"format "\r\n",##__VA_ARGS__)
#else
#define user_info(format,...)
#define user_debug(format,...)
#define user_err(format,...)
#endif
/* USER CODE END Private defines */

void MX_USART1_UART_Init(void);
void MX_USART2_UART_Init(void);

/* USER CODE BEGIN Prototypes */
void USART_DMA_START(UART_HandleTypeDef *huart);
void HAL_UART_IdleCallback(UART_HandleTypeDef *huart);
void USART2_TX(uint8_t *Tx_Buf,uint16_t TxCount);
/* USER CODE END Prototypes */

#ifdef __cplusplus
}
#endif
#endif /*__ usart_H */

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
