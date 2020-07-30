#ifndef __USER_DATATYPE__H
#define __USER_DATATYPE__H
#include "main.h"
#define USART2_RX_BUFFERSIZE	256			//串口缓存大小
#define LoRaVal_Size			15			//系统参数大小 
#define FLASH_SAVE_ADDR 		0X0800F000	//系统参数保存起始地址
#define CMDNUM					15			//系统命令
typedef struct
{
	volatile uint8_t	Buffer[USART2_RX_BUFFERSIZE];
	uint16_t	count;
}tsBuf;
typedef enum
{
	RF_CH=1,
	RF_POWER,
	RF_BW,
	RF_SF,
	RF_TXOUT,
	RF_RXOUT,
	RF_PL,
	NET_ADDR,

}teCmd;
extern tsBuf tsUsart2Buf;//定义串口数据缓存
extern tsBuf LoRa_Txdata;//LoRa发送数据缓存
extern uint16_t LoRaSysVal[LoRaVal_Size];

#endif
