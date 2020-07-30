/*
 * THE FOLLOWING FIRMWARE IS PROVIDED: (1) "AS IS" WITH NO WARRANTY; AND 
 * (2)TO ENABLE ACCESS TO CODING INFORMATION TO GUIDE AND FACILITATE CUSTOMER.
 * CONSEQUENTLY, SEMTECH SHALL NOT BE HELD LIABLE FOR ANY DIRECT, INDIRECT OR
 * CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE CONTENT
 * OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING INFORMATION
 * CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
 * 
 * Copyright (C) SEMTECH S.A.
 */
/*! 
 * \file       sx1276-Hal.c
 * \brief      SX1276 Hardware Abstraction Layer
 *
 * \version    2.0.B2 
 * \date       Nov 21 2012
 * \author     Miguel Luis
 *
 * Last modified by Miguel Luis on Jun 19 2013
 */
#include <stdint.h>
#include <stdbool.h> 

#include "platform.h"
#include "sx1276-Hal.h"

#include "stm32f0xx_hal.h"
#include "spi.h"
/*!
 * SX1276 RESET I/O definitions
 */
#define RESET_IOPORT                                SPI1_RST_GPIO_Port
#define RESET_PIN                                   SPI1_RST_Pin

/*!
 * SX1276 SPI NSS I/O definitions
 */
#define NSS_IOPORT                                  SPI1_NSS_GPIO_Port
#define NSS_PIN                                     SPI1_NSS_Pin

/*!
 * SX1276 DIO pins  I/O definitions
 */
#define DIO0_IOPORT                                 DIO_0_GPIO_Port
#define DIO0_PIN                                    DIO_0_Pin
#define DIO1_IOPORT                                 DIO_1_GPIO_Port
#define DIO1_PIN                                    DIO_1_Pin
#define DIO2_IOPORT                                 DIO_2_GPIO_Port
#define DIO2_PIN                                    DIO_2_Pin
#define DIO3_IOPORT                                 DIO_3_GPIO_Port
#define DIO3_PIN                                    DIO_3_Pin
#define DIO4_IOPORT                                	DIO_4_GPIO_Port 
#define DIO4_PIN                                    DIO_4_Pin
#define DIO5_IOPORT                              	DIO_5_GPIO_Port   
#define DIO5_PIN                                    DIO_5_Pin



void SX1276InitIo( void )
{
    GPIO_InitTypeDef GPIO_InitStruct;
	
	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_GPIOC_CLK_ENABLE();
	__HAL_RCC_GPIOB_CLK_ENABLE();

	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	
	// Configure RESET as output
	GPIO_InitStruct.Pin = RESET_PIN;
	HAL_GPIO_WritePin(RESET_IOPORT, RESET_PIN, GPIO_PIN_SET);
	HAL_GPIO_Init(RESET_IOPORT, &GPIO_InitStruct);
    
    // Configure NSS as output  
  GPIO_InitStruct.Pin = NSS_PIN;
	HAL_GPIO_WritePin(NSS_IOPORT, NSS_PIN, GPIO_PIN_SET);
  HAL_GPIO_Init(NSS_IOPORT, &GPIO_InitStruct);
	
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	
  GPIO_InitStruct.Pin = DIO0_PIN|DIO1_PIN|DIO2_PIN|DIO3_PIN|DIO4_PIN|DIO5_PIN;
  HAL_GPIO_Init(DIO5_IOPORT, &GPIO_InitStruct);	
}

void SX1276SetReset( uint8_t state )
{
	if( state == RADIO_RESET_ON )
	{
		HAL_GPIO_WritePin(RESET_IOPORT, RESET_PIN, GPIO_PIN_RESET);
		//SPI_RST=0;
	}
	else
	{
		HAL_GPIO_WritePin(RESET_IOPORT, RESET_PIN, GPIO_PIN_SET);
		//SPI_RST=1;
	}
}

void SX1276Write( uint8_t addr, uint8_t data )
{
    SX1276WriteBuffer( addr, &data, 1 );
}

void SX1276Read( uint8_t addr, uint8_t *data )
{
    SX1276ReadBuffer( addr, data, 1 );
}

void SX1276WriteBuffer( uint8_t addr, uint8_t *buffer, uint8_t size )
{
    uint8_t i;

//    SPI_NSS = 0;
    HAL_GPIO_WritePin(NSS_IOPORT, NSS_PIN, GPIO_PIN_RESET);

    SpiInOut( addr | 0x80 );
    for( i = 0; i < size; i++ )
    {
        SpiInOut( buffer[i] );
    }

//   SPI_NSS = 1;
    HAL_GPIO_WritePin(NSS_IOPORT, NSS_PIN, GPIO_PIN_SET);
}

void SX1276ReadBuffer( uint8_t addr, uint8_t *buffer, uint8_t size )
{
    uint8_t i;

//    SPI_NSS = 0;
    HAL_GPIO_WritePin(NSS_IOPORT, NSS_PIN, GPIO_PIN_RESET);

    SpiInOut( addr & 0x7F );

    for( i = 0; i < size; i++ )
    {
        buffer[i] = SpiInOut( 0 );
    }

//    SPI_NSS = 1;
    HAL_GPIO_WritePin(NSS_IOPORT, NSS_PIN, GPIO_PIN_SET);
}

void SX1276WriteFifo( uint8_t *buffer, uint8_t size )
{
    SX1276WriteBuffer( 0, buffer, size );
}

void SX1276ReadFifo( uint8_t *buffer, uint8_t size )
{
    SX1276ReadBuffer( 0, buffer, size );
}

inline uint8_t SX1276ReadDio0( void )
{
	return HAL_GPIO_ReadPin(DIO0_IOPORT, DIO0_PIN);
}

inline uint8_t SX1276ReadDio1( void )
{
    return HAL_GPIO_ReadPin(DIO1_IOPORT, DIO1_PIN);
}

inline uint8_t SX1276ReadDio2( void )
{
    return HAL_GPIO_ReadPin(DIO2_IOPORT, DIO2_PIN);
}

inline uint8_t SX1276ReadDio3( void )
{
   return HAL_GPIO_ReadPin(DIO3_IOPORT, DIO3_PIN);
}

inline uint8_t SX1276ReadDio4( void )
{
	return HAL_GPIO_ReadPin(DIO4_IOPORT, DIO4_PIN);
}

inline uint8_t SX1276ReadDio5( void )
{
    return HAL_GPIO_ReadPin(DIO5_IOPORT, DIO5_PIN);
}

inline void SX1276WriteRxTx( uint8_t txEnable )
{
	
}


