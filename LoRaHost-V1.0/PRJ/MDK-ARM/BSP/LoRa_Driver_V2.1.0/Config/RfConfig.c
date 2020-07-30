#include "Rfconfig.h"

tRadioDriver *Radio=NULL;
void RF_Init()
{
	if((LoRaSysVal[0]==0xFFFF)&&(LoRaSysVal[1])==0xFFFF)//默认参数
	{
		//11#->4	12#->7	3#->10	4#->13	5#->16
		//6#->19	7#->22	8#->25	9#->28	10#->31
		LoRaSysVal[0]=19;//信道
		LoRaSysVal[1]=20;
		LoRaSysVal[2]=7;
		LoRaSysVal[3]=12;
		LoRaSysVal[4]=3000;
		LoRaSysVal[5]=3000;
		LoRaSysVal[6]=128;
		LoRaSysVal[7]=6;//网关地址
		LoRaSysVal[8]=20;
		LoRaSysVal[9]=0;
		LoRaSysVal[10]=0;
		LoRaSysVal[11]=0;
		LoRaSysVal[12]=0;
		LoRaSysVal[13]=103;//网关查询子设备的数量
		LoRaSysVal[14]=0;//操作保护
		//STMFLASH_Write(FLASH_SAVE_ADDR,LoRaSysVal,LoRaVal_Size);
	}
	LoRaSettings.RFFrequency=410000000+(LoRaSysVal[0]*1000000);//频率
	LoRaSettings.Power=LoRaSysVal[1];//电源功率
	LoRaSettings.SignalBw=LoRaSysVal[2];//带宽
	LoRaSettings.SpreadingFactor=LoRaSysVal[3];//扩频因子
	LoRaSettings.ErrorCoding=1;//编码率
	LoRaSettings.CrcOn=true;//Crc校验
	LoRaSettings.ImplicitHeaderOn=false;//序头模式
	LoRaSettings.RxSingleOn=1;//接收模式中的single模式开关  0代表continue模式
	LoRaSettings.FreqHopOn=0;//跳频开关
	LoRaSettings.HopPeriod=4;//跳频周期
	LoRaSettings.TxPacketTimeout=LoRaSysVal[4];//发送超时时间
	LoRaSettings.RxPacketTimeout=LoRaSysVal[5];//接收超时时间
	LoRaSettings.PayloadLength=LoRaSysVal[6];//前导码长度

	Radio = RadioDriverInit();// LoRa回调函数注册
	Radio->Init();//LoRa芯片初始化
	//SX1276LoRaSetPreambleLength(32);//设置唤醒模式
	HAL_GPIO_WritePin(VC1_GPIO_Port,VC1_Pin,GPIO_PIN_SET);
	SX1276SetRFState(RFLR_STATE_CAD_INIT);
	
}

void LoRa_SendData(uint8_t *buf,uint8_t size)
{
//	uint16_t crc=0;
	HAL_GPIO_WritePin(D3_GPIO_Port,D3_Pin,GPIO_PIN_SET);//开发送指示灯
	HAL_GPIO_WritePin(VC1_GPIO_Port,VC1_Pin,GPIO_PIN_RESET);
//	crc= MB_CRC16(buf,size);
//	buf[size++]=crc>>8;//crc高位
//	buf[size++]=crc;//crc低位
	for(int i=0;i<size;i++)
	{
		printf("%02X ",buf[i]);
	}
	printf("\r\n");
	Radio->SetTxPacket(buf,size);	
}

