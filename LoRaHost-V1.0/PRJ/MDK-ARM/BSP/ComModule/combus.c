#include "combus.h"
//����AT����ṹ��
static const struct
{
	char *Cmd;
	uint8_t Index;
}
CmdBuf[CMDNUM]=
{
	{"AT+RFCH",1},{"AT+RFPOWER",2},{"AT+RFBW",3},{"AT+RFSF",4},{"AT+RFTXOUT",5},{"AT+RFRXOUT",6},{"AT+RFPL",7},{"AT+NETADDR=",8},{"AT+BAUD=",2},{"AT+SPEED=",3},
};



/*
	* �ַ�����ȡ
	*	��ڲ�����
	*			s:		Դ�ַ���
	*		ch1:		��ʼ����
	*		ch2:		��������
	*		subsrt:	Ŀ���ַ���

	**/
static void substring(char *s, char ch1, char ch2, char *substr)
{
    while (*s && *s++ != ch1);
    while (*s && *s != ch2) *substr++ = *s++;
    *substr = '\0';
}
/*
*********************************************************************************************************
*	�� �� ��: str_to_int
*	����˵��: ��ASCII���ַ���ת���������� ����С�����Զ�Խ����
*	��    ��: _pStr :��ת����ASCII�봮. �����Զ��ţ�#��0������ 2014-06-20 �޸�Ϊ��0-9���ַ���
*	�� �� ֵ: ����������ֵ
*********************************************************************************************************
*/
int str_to_int(char *_pStr)
{
	unsigned char flag;
	char *p;
	int ulInt;
	unsigned char  i;
	unsigned char  ucTemp;

	p = _pStr;
	if (*p == '-')
	{
		flag = 1;	/* ���� */
		p++;
	}
	else
	{
		flag = 0;
	}

	ulInt = 0;
	for (i = 0; i < 15; i++)
	{
		ucTemp = *p;
		if (ucTemp == '.')	/* ����С���㣬�Զ�����1���ֽ� */
		{
			p++;
			ucTemp = *p;
		}
		if ((ucTemp >= '0') && (ucTemp <= '9'))
		{
			ulInt = ulInt * 10 + (ucTemp - '0');
			p++;
		}
		else
		{
			break;
		}
	}

	if (flag == 1)
	{
		return -ulInt;
	}
	return ulInt;
}
//���������������ֵ
uint8_t Lookup_Str(char *buf)
{
	for(int i=0;i<LoRaVal_Size;i++)
	{
		if(strstr((char const*)buf,CmdBuf[i].Cmd)!=NULL)
			return CmdBuf[i].Index;
	}
	return 0;
}
void vParseString(char *buff)//�޸ı����ز���
{
	uint8_t cmd=0;
	uint16_t val=0;
	char tempbuf[20];
	cmd=Lookup_Str(buff);
	switch(cmd)
	{
		case RF_CH://�޸�LoRaƵ��
			user_info("cmd is RF_CH");
			substring(buff,'=','\0',tempbuf);
			val=str_to_int(tempbuf);
			LoRaSysVal[cmd-1]=val;
			STMFLASH_Write(FLASH_SAVE_ADDR,LoRaSysVal,LoRaVal_Size);
			user_info("RF_CH=%d",val);
			user_info("Setup complete system restart !");
			HAL_NVIC_SystemReset();
			break;
		case RF_POWER://�޸�LoRa����
			user_info("cmd is RF_POWER");
			substring(buff,'=','\0',tempbuf);
			val=str_to_int(tempbuf);
			LoRaSysVal[cmd-1]=val;
			STMFLASH_Write(FLASH_SAVE_ADDR,LoRaSysVal,LoRaVal_Size);
			user_info("RF_POWER=%d",val);
			user_info("Setup complete system restart !");
			HAL_NVIC_SystemReset();
			break;
		case RF_BW://�޸�LoRa����
			user_info("cmd is RF_BW");
			substring(buff,'=','\0',tempbuf);
			val=str_to_int(tempbuf);
			LoRaSysVal[cmd-1]=val;
			STMFLASH_Write(FLASH_SAVE_ADDR,LoRaSysVal,LoRaVal_Size);
			user_info("RF_BW=%d",val);
			user_info("Setup complete system restart !");
			HAL_NVIC_SystemReset();
			break;
		case RF_SF://�޸�LoRa��Ƶ����
			user_info("cmd is RF_SF");
			substring(buff,'=','\0',tempbuf);
			val=str_to_int(tempbuf);
			LoRaSysVal[cmd-1]=val;
			STMFLASH_Write(FLASH_SAVE_ADDR,LoRaSysVal,LoRaVal_Size);
			user_info("RF_SF=%d",val);
			user_info("Setup complete system restart !");
			HAL_NVIC_SystemReset();
			break;
		case RF_RXOUT:
			 user_info("cmd is RF_RXOUT");
			 substring(buff,'=','\0',tempbuf);
			 val=str_to_int(tempbuf);
			 LoRaSysVal[cmd-1]=val;
			 STMFLASH_Write(FLASH_SAVE_ADDR,LoRaSysVal,LoRaVal_Size);
			 user_info("RF_RXOUT=%d",val);
			 user_info("Setup complete system restart !");
			 HAL_NVIC_SystemReset();
			break;
		case NET_ADDR://�޸������ַ
			user_info("cmd is NET_ADDR");
			substring(buff,'=','\0',tempbuf);
			val=str_to_int(tempbuf);
			LoRaSysVal[cmd-1]=val;
			STMFLASH_Write(FLASH_SAVE_ADDR,LoRaSysVal,LoRaVal_Size);
			user_info("NET_ADDR=%d",val);
			user_info("Setup complete system restart !");
			HAL_NVIC_SystemReset();
			break;
		
		default:
			user_info("cmd is nofind !");
			break;
	}
	
}


