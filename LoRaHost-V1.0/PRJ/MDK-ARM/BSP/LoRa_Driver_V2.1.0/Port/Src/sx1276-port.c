#include "sx1276-port.h"
#include "stm32f0xx_hal.h"
#include "spi.h"

volatile uint32_t TickCounter = 0;

void SpiInit( void )
{
	MX_SPI1_Init();
}	

uint8_t SpiInOut( uint8_t outData )
{
	return spi_1_in_out(outData);
}





















