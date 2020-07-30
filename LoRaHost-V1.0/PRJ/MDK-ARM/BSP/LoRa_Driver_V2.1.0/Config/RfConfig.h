#ifndef __RFCONFIG_H__
#define __RFCONFIG_H__
#include "sx1276-LoRa.h"
#include "sx1276.h"
#include "main.h"
#include "radio.h"
#include "stdio.h"
#include "sx1276-LoRaMisc.h"
#include "user_dataType.h"
#include "crc.h"
#include "usart.h"
extern tRadioDriver *Radio;

void LoRa_SendData(uint8_t *buf,uint8_t size);
void RF_Init(void);
#endif
