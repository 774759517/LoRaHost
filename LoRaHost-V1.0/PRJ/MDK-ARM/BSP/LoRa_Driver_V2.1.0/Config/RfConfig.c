#include "Rfconfig.h"

tRadioDriver *Radio=NULL;
void RF_Init()
{
	if((LoRaSysVal[0]==0xFFFF)&&(LoRaSysVal[1])==0xFFFF)//Ĭ�ϲ���
	{
		//11#->4	12#->7	3#->10	4#->13	5#->16
		//6#->19	7#->22	8#->25	9#->28	10#->31
		LoRaSysVal[0]=19;//�ŵ�
		LoRaSysVal[1]=20;
		LoRaSysVal[2]=7;
		LoRaSysVal[3]=12;
		LoRaSysVal[4]=3000;
		LoRaSysVal[5]=3000;
		LoRaSysVal[6]=128;
		LoRaSysVal[7]=6;//���ص�ַ
		LoRaSysVal[8]=20;
		LoRaSysVal[9]=0;
		LoRaSysVal[10]=0;
		LoRaSysVal[11]=0;
		LoRaSysVal[12]=0;
		LoRaSysVal[13]=103;//���ز�ѯ���豸������
		LoRaSysVal[14]=0;//��������
		//STMFLASH_Write(FLASH_SAVE_ADDR,LoRaSysVal,LoRaVal_Size);
	}
	LoRaSettings.RFFrequency=410000000+(LoRaSysVal[0]*1000000);//Ƶ��
	LoRaSettings.Power=LoRaSysVal[1];//��Դ����
	LoRaSettings.SignalBw=LoRaSysVal[2];//����
	LoRaSettings.SpreadingFactor=LoRaSysVal[3];//��Ƶ����
	LoRaSettings.ErrorCoding=1;//������
	LoRaSettings.CrcOn=true;//CrcУ��
	LoRaSettings.ImplicitHeaderOn=false;//��ͷģʽ
	LoRaSettings.RxSingleOn=1;//����ģʽ�е�singleģʽ����  0����continueģʽ
	LoRaSettings.FreqHopOn=0;//��Ƶ����
	LoRaSettings.HopPeriod=4;//��Ƶ����
	LoRaSettings.TxPacketTimeout=LoRaSysVal[4];//���ͳ�ʱʱ��
	LoRaSettings.RxPacketTimeout=LoRaSysVal[5];//���ճ�ʱʱ��
	LoRaSettings.PayloadLength=LoRaSysVal[6];//ǰ���볤��

	Radio = RadioDriverInit();// LoRa�ص�����ע��
	Radio->Init();//LoRaоƬ��ʼ��
	//SX1276LoRaSetPreambleLength(32);//���û���ģʽ
	HAL_GPIO_WritePin(VC1_GPIO_Port,VC1_Pin,GPIO_PIN_SET);
	SX1276SetRFState(RFLR_STATE_CAD_INIT);
	
}

void LoRa_SendData(uint8_t *buf,uint8_t size)
{
//	uint16_t crc=0;
	HAL_GPIO_WritePin(D3_GPIO_Port,D3_Pin,GPIO_PIN_SET);//������ָʾ��
	HAL_GPIO_WritePin(VC1_GPIO_Port,VC1_Pin,GPIO_PIN_RESET);
//	crc= MB_CRC16(buf,size);
//	buf[size++]=crc>>8;//crc��λ
//	buf[size++]=crc;//crc��λ
	for(int i=0;i<size;i++)
	{
		printf("%02X ",buf[i]);
	}
	printf("\r\n");
	Radio->SetTxPacket(buf,size);	
}

