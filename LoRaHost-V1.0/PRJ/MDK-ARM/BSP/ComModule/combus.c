#include "combus.h"
//定义AT命令结构体
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
	* 字符串截取
	*	入口参数：
	*			s:		源字符串
	*		ch1:		开始条件
	*		ch2:		结束条件
	*		subsrt:	目标字符串

	**/
static void substring(char *s, char ch1, char ch2, char *substr)
{
    while (*s && *s++ != ch1);
    while (*s && *s != ch2) *substr++ = *s++;
    *substr = '\0';
}
/*
*********************************************************************************************************
*	函 数 名: str_to_int
*	功能说明: 将ASCII码字符串转换成整数。 遇到小数点自动越过。
*	形    参: _pStr :待转换的ASCII码串. 可以以逗号，#或0结束。 2014-06-20 修改为非0-9的字符。
*	返 回 值: 二进制整数值
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
		flag = 1;	/* 负数 */
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
		if (ucTemp == '.')	/* 遇到小数点，自动跳过1个字节 */
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
//查找命令并返回索引值
uint8_t Lookup_Str(char *buf)
{
	for(int i=0;i<LoRaVal_Size;i++)
	{
		if(strstr((char const*)buf,CmdBuf[i].Cmd)!=NULL)
			return CmdBuf[i].Index;
	}
	return 0;
}
void vParseString(char *buff)//修改本网关参数
{
	uint8_t cmd=0;
	uint16_t val=0;
	char tempbuf[20];
	cmd=Lookup_Str(buff);
	switch(cmd)
	{
		case RF_CH://修改LoRa频率
			user_info("cmd is RF_CH");
			substring(buff,'=','\0',tempbuf);
			val=str_to_int(tempbuf);
			LoRaSysVal[cmd-1]=val;
			STMFLASH_Write(FLASH_SAVE_ADDR,LoRaSysVal,LoRaVal_Size);
			user_info("RF_CH=%d",val);
			user_info("Setup complete system restart !");
			HAL_NVIC_SystemReset();
			break;
		case RF_POWER://修改LoRa功率
			user_info("cmd is RF_POWER");
			substring(buff,'=','\0',tempbuf);
			val=str_to_int(tempbuf);
			LoRaSysVal[cmd-1]=val;
			STMFLASH_Write(FLASH_SAVE_ADDR,LoRaSysVal,LoRaVal_Size);
			user_info("RF_POWER=%d",val);
			user_info("Setup complete system restart !");
			HAL_NVIC_SystemReset();
			break;
		case RF_BW://修改LoRa带宽
			user_info("cmd is RF_BW");
			substring(buff,'=','\0',tempbuf);
			val=str_to_int(tempbuf);
			LoRaSysVal[cmd-1]=val;
			STMFLASH_Write(FLASH_SAVE_ADDR,LoRaSysVal,LoRaVal_Size);
			user_info("RF_BW=%d",val);
			user_info("Setup complete system restart !");
			HAL_NVIC_SystemReset();
			break;
		case RF_SF://修改LoRa扩频因子
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
		case NET_ADDR://修改网络地址
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


