#include "stm32f0xx_hal.h"
#include "usart.h"
/*����STM32_MCU������*/
typedef enum
{
	F0,
	F1,
	F2,
	F3,
	F4,
	F7,
	L0,
	L1,
	L4,
	H7,
}teMCUTypedef;

void GetSTM32MCUUID(uint32_t *id,teMCUTypedef type);

