#ifndef __SX1276_PORT_H__
#define __SX1276_PORT_H__

#include <stdint.h>									//-- addition


extern volatile uint32_t TickCounter;

void SpiInit( void );
uint8_t SpiInOut( uint8_t outData );


#endif

