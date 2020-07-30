#ifndef __USER_DATATYPE__H
#define __USER_DATATYPE__H
#include "main.h"
#define USART2_RX_BUFFERSIZE	256			//���ڻ����С
#define LoRaVal_Size			15			//ϵͳ������С 
#define FLASH_SAVE_ADDR 		0X0800F000	//ϵͳ����������ʼ��ַ
#define CMDNUM					15			//ϵͳ����
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
extern tsBuf tsUsart2Buf;//���崮�����ݻ���
extern tsBuf LoRa_Txdata;//LoRa�������ݻ���
extern uint16_t LoRaSysVal[LoRaVal_Size];

#endif
