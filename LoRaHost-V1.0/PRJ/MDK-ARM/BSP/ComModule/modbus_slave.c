#include "modbus_slave.h"

/*��������*/
PDUData_TypeDef PduData;
__IO uint8_t Tx_Buf[256];            // ���ͻ���,���256�ֽ�
uint8_t Rx_Buf[256];
__IO uint16_t RxCount=0;
__IO uint16_t Reg_Val[256];

/*��������*/
static uint8_t MB_JudgeAddr(uint16_t _Addr,uint16_t _RegNum);//�жϵ�ַ�Ƿ����Э�鷶Χ
static uint8_t MB_JudgeNum(uint16_t _RegNum,uint8_t _FunCode,uint16_t _ByteNum);
static uint8_t MB_10H_WR_NReg(uint16_t*Ptr,uint16_t NumofReg, uint8_t*buf);
static uint8_t MB_RSP_10H(uint16_t _TxCount,uint16_t _AddrOffset,uint16_t _ByteNum);
static uint16_t MB_RSP_01H(uint16_t _TxCount,uint8_t *_AddrOffset ,uint16_t _CoilNum);
static uint8_t MB_RSP_03H(uint16_t _TxCount,uint16_t *_AddrOffset,uint16_t _RegNum );
static uint8_t MB_RSP_05H(uint16_t _TxCount,uint16_t _AddrOffset ,uint8_t *_AddrAbs);
static uint8_t MB_RSP_06H(uint16_t _TxCount,uint16_t _AddrOffset ,uint16_t *_AddrAbs);
static uint8_t MB_RSP_10H(uint16_t _TxCount,uint16_t _AddrOffset,uint16_t _ByteNum);
/*������*/

/* ��ȡ����֡,���н�������֡ */
void MB_Parse_Data()
{
  PduData.Code = Rx_Buf[1];                   // ������
  PduData.Addr = ((Rx_Buf[2]<<8) | Rx_Buf[3]);// �Ĵ�����ʼ��ַ
  PduData.Num  = ((Rx_Buf[4]<<8) | Rx_Buf[5]);// ����(Coil,Input,Holding Reg,Input Reg)
  PduData._CRC = MB_CRC16((uint8_t*)&Rx_Buf,RxCount-2);             // CRCУ����
  PduData.byteNums = Rx_Buf[6];               // ����ֽ���
  PduData.ValueReg = (uint8_t*)&Rx_Buf[7];                          // �Ĵ���ֵ��ʼ��ַ
  PduData.PtrCoilOffset = PduData.PtrCoilbase + PduData.Addr;       // ��ɢ�����ڴ���ʼ��ַ
  PduData.PtrHoldingOffset = PduData.PtrHoldingbase + PduData.Addr; // ���ּĴ�������ʼ��ַ
}
/** 
  * ��������: �Խ��յ������ݽ��з�����ִ��
  * �������: ��
  * �� �� ֵ: �쳣���0x00
  * ˵    ��: �жϹ�����,��֤��ַ�Ƿ���ȷ.��ֵ�����Ƿ����,����û����ͷ�����Ӧ�ź�
  */
uint8_t MB_Analyze_Execute(void )
{
  uint16_t ExCode = EX_CODE_NONE;
  /* У�鹦���� */
  if( IS_NOT_FUNCODE(PduData.Code) ) // ��֧�ֵĹ�����
  {
    /* Modbus�쳣��Ӧ */
    ExCode = EX_CODE_01H;            // �쳣��01H
    return ExCode;
  }
  /* ���ݹ�����ֱ����ж� */
  switch(PduData.Code)
  {
    /* ������Ϊ01H�������02��������һ����,��ʵҲûʲô��һ��
     * ֻ�ǲ�����ַ���ܲ�һ��,��һ���Ͼ�����ʵ��,������main����
     * ���뵥�����ڴ�ʹ�ò�ͬ�Ĺ�����,��ʵ��Ӧ���б����������ʹ��
     * ��ͬ���ڴ�ռ�
     */
/* ---- 01H  02H ��ȡ��ɢ������(Coil Input)---------------------- */
    case FUN_CODE_01H:
    case FUN_CODE_02H:
      /* �ж���Ȧ�����Ƿ���ȷ */  
      ExCode = MB_JudgeNum(PduData.Num,PduData.Code,1);
      if(ExCode != EX_CODE_NONE )
        return ExCode;      
      
      /* �жϵ�ַ�Ƿ���ȷ*/
      ExCode = MB_JudgeAddr( PduData.Addr,PduData.Num);
      if(ExCode != EX_CODE_NONE )
        return ExCode;  
      
      /* ��������,��ȡ��Ȧ״̬
       */

      break;
/* ---- 03H  04H ��ȡ����/����Ĵ���---------------------- */
    case FUN_CODE_03H:
    case FUN_CODE_04H:
      /* �жϼĴ��������Ƿ���ȷ */
      ExCode = MB_JudgeNum(PduData.Num,PduData.Code,PduData.byteNums);
      if(ExCode != EX_CODE_NONE )
        return ExCode;  
      
      /* �жϵ�ַ�Ƿ���ȷ*/
      ExCode = MB_JudgeAddr( PduData.Addr,PduData.Num);
      if(ExCode != EX_CODE_NONE )
        return ExCode;  
      
      /* ��������,�����ּĴ���

       */

      break;
/* ---- 05H д�뵥����ɢ��---------------------- */
    case FUN_CODE_05H:
      /* ���ֵ!=OK */
      if((PduData.Num != 0x0000) && PduData.Num != 0xFF00)
      {
        ExCode = EX_CODE_03H;
        return ExCode;        
      }
      /* д��һ����Ȧֵ */
      if(PduData.Num == 0xFF00)
      { 
        *PduData.PtrCoilOffset = 1;
        /* ��ȡд��ֵ,��֤�Ƿ�д��ɹ� */
        if( *PduData.PtrCoilOffset != 1)
        {
          ExCode = EX_CODE_04H;
          return ExCode;
        }
      }
      else 
      {
        *PduData.PtrCoilOffset = 0;
        /* ��ȡд��ֵ,��֤�Ƿ�д��ɹ� */
        if( *PduData.PtrCoilOffset != 0)
        { 
          ExCode = EX_CODE_04H;
          return ExCode;
        }
      }
      break;
/* ---- 06H д�������ּĴ��� ---------------------- */
    case FUN_CODE_06H:
      
      /* д��Ĵ���ֵ*/
      *PduData.PtrHoldingOffset = PduData.Num;
      /* ��֤д�ɹ� */
      if(*PduData.PtrHoldingOffset != PduData.Num)
      {
        ExCode = EX_CODE_04H;
         return ExCode;
      }
      break;
/* ---- 10H д������ּĴ��� ---------------------- */
    case FUN_CODE_10H:
      /* �жϼĴ��������Ƿ���ȷ */
      ExCode = MB_JudgeNum(PduData.Num,PduData.Code,PduData.byteNums);
      if(ExCode != EX_CODE_NONE )
        return ExCode;    
      
      /* �жϵ�ַ�Ƿ���ȷ*/
      ExCode = MB_JudgeAddr( PduData.Addr,PduData.Num);
      if(ExCode != EX_CODE_NONE )
        return ExCode;  
      
      /* д�����Ĵ��� */
      ExCode = MB_10H_WR_NReg((uint16_t*)PduData.PtrHoldingOffset,PduData.Num,(uint8_t*)PduData.ValueReg);
      if(ExCode != EX_CODE_NONE )
        return ExCode;  
      break;
  }
  /* ����֡û���쳣 */
  return ExCode; //   EX_CODE_NONE
}
/**
  * ��������: д,��N���Ĵ���
  * �������: _AddrOffset:ƫ�Ƶ�ַ,_RegNum:�Ĵ�������,_Datebuf:����ָ��
  * �� �� ֵ: �쳣��:04H��NONE
  * ˵    ��: ��_AddrOffset��ָ��Ŀռ���д��_RegNum*2������,���Ҷ�ȡ��֤�Ƿ�д��ɹ�
  */
//		MB_10H_WR_NReg((uint16_t*)PduData.PtrHoldingOffset,PduData.Num,(uint8_t*)PduData.ValueReg);
uint8_t MB_10H_WR_NReg(uint16_t* _AddrOffset,uint16_t _RegNum , uint8_t* _Datebuf)
{
  uint16_t i = 0;
  uint16_t Value = 0;
  uint16_t* ValAddr = _AddrOffset;
  uint8_t* tempDatabuf = _Datebuf;
	//tsData data;
  for(i=0;i<_RegNum;i++)
  {
    Value = (uint16_t)((*_Datebuf<<8 ) | (*(_Datebuf+1)));
    *_AddrOffset++ = Value ;
	 //���ݷ���savle 
//		data.NetAddr=MB_SLAVEADDR;
//		data.SlaveAddr=PduData.Addr+i;
//		data.Cmd=PduData.Code;
//		data.Val=Value;
//		xQueueSendToBack(DataQueueHandle,&data,NULL);//������ӣ��ڶ��к����
		_Datebuf+=2;
  }
  
  /* ��ȡ��֤д���Ƿ�ɹ� */
  _Datebuf = tempDatabuf;
  for(i = 0;i<_RegNum;i++)
  {
    Value = (uint16_t)((*_Datebuf<<8 ) | (*(_Datebuf+1)));
    if(*ValAddr++ != Value)
    {
      return EX_CODE_04H;
    }
    _Datebuf+=2;
  }
  return EX_CODE_NONE;
}
/**
  * ��������: �жϲ������������Ƿ����Э�鷶Χ
  * �������: _RegNum:�Ĵ�������,_FunCode:������,_ByteNum:�ֽ�����
  * �� �� ֵ: �쳣��:03��NONE
  * ˵    ��: �Կɲ��������ڴ�ռ�Ĺ�������Ҫ��֤�����ĵ�ַ�Ƿ���Ϸ�Χ
  */
uint8_t MB_JudgeNum(uint16_t _RegNum,uint8_t _FunCode,uint16_t _ByteNum)
{
  uint8_t Excode = EX_CODE_NONE;
  uint16_t _CoilNum = _RegNum; // ��Ȧ(��ɢ��)������
  switch(_FunCode)
  {
    case FUN_CODE_01H: 
    case FUN_CODE_02H:
      if( (_CoilNum<0x0001) || (_CoilNum>0x07D0))
        Excode = EX_CODE_03H;// �쳣��03H;
      break;
    case FUN_CODE_03H:
    case FUN_CODE_04H:
      if( (_RegNum<0x0001) || (_RegNum>0x007D))
        Excode = EX_CODE_03H;// �쳣��03H;      
      break;
    case FUN_CODE_10H:
      if( (_RegNum<0x0001) || (_RegNum>0x007B))
        Excode = EX_CODE_03H;// �쳣��03H
      if( _ByteNum != (_RegNum<<1))
        Excode = EX_CODE_03H;// �쳣��03H
      break;
  }
  return Excode;
}
/**
  * ��������: �жϵ�ַ�Ƿ����Э�鷶Χ
  * �������: _Addr:��ʼ��ַ,_RegNum:�Ĵ�������,_FunCode:������
  * �� �� ֵ: �쳣��:02H��NONE
  * ˵    ��: ��ַ��Χ��0x0000~0xFFFF,�ɲ����Ŀռ䷶Χ���ܳ����������
  */
uint8_t MB_JudgeAddr(uint16_t _Addr,uint16_t _RegNum)
{
  uint8_t Excode = EX_CODE_NONE;
  /* ��ַ+�Ĵ����������ܳ���0xFFFF */
  if( ((uint32_t)_RegNum+(uint32_t)_Addr) > (uint32_t)0xFFFF)
  {
    Excode = EX_CODE_02H;// �쳣�� 02H
  }
  return Excode;
}
/**
  * ��������: ��ȡ��ɢ���
  * �������: _TxCount :���ͼ�����,_AddrOffset��ַƫ����,_CoilNum:��Ȧ����
  * �� �� ֵ: Tx_Buf������Ԫ������
  * ˵    ��: ��ȡ��ɢ���,�������Tx_Buf
  */
uint16_t MB_RSP_01H(uint16_t _TxCount,uint8_t *_AddrOffset ,uint16_t _CoilNum)
{
  uint16_t ByteNum = 0;
  uint16_t i = 0;
  uint16_t r = 0 ;
  ByteNum = _CoilNum/8;
  /* �����������,��Ҫ�������λ��0 */
  r = _CoilNum%8;
  if(r != 0)
  {
    ByteNum += 1; //�ֽ���+1
    Tx_Buf[_TxCount++] = ByteNum;  
    
    /* �ں�������������¶�ȡ��Ȧ(bit)��״̬ */
    for(i=0;i<ByteNum-1;i++)
    {
      Tx_Buf[_TxCount] = 0x00;
      /* ÿ8����Ȧ(bit)һ��ѭ��,��ȡ8����Ȧ��״̬������һ��Byte*/
      for(uint8_t j=0;j<8;j++)
      {
        /* �����1,��byte��Ӧλ(bit)��1 */
        if(*(_AddrOffset++))
        {
          /* �������൱�ڽ�һ��Byte����һ����Ȧ(bit)ӳ�䵽һ��Byte��
           * ��8��Byte���һ��Byte��8��bit
           */
          Tx_Buf[_TxCount] |= (0x01<<j);
        }
      }
      _TxCount++;
    }
    /* �������������һ���ֽ�,�����bit����Ϊ0 */
    Tx_Buf[_TxCount] = 0x00;
    for(uint8_t j=0;j<r;j++)
    {
      if(*(_AddrOffset++))
      {
        Tx_Buf[_TxCount] |= (0x01<<j);// ��ȡ����
      }
    }
    _TxCount++;
  }
  /* �������r==0,˵����Ҫ��ȡ����Ȧ����(bits)�պ���Byte�������� */
  else
  {
    Tx_Buf[_TxCount++] = ByteNum;
    for(i=0;i<ByteNum;i++)
    {
      Tx_Buf[_TxCount] = 0x00;
      for(uint8_t j=0;j<8;j++)
      {
        if(*(_AddrOffset++))
        {
          Tx_Buf[_TxCount] |= (0x01<<j);// ��ȡ����
        }
      }
      _TxCount++;
    }
  }
  return _TxCount;
}
/**
  * ��������: ��ȡ���ּĴ���
  * �������: _TxCount :���ͼ�����,_AddrOffset��ַƫ����,_RegNum:�Ĵ�������
  * �� �� ֵ: Tx_Buf������Ԫ������
  * ˵    ��: ��ȡ���ּĴ���������,�������Tx_Buf
  */
uint8_t MB_RSP_03H(uint16_t _TxCount,uint16_t *_AddrOffset,uint16_t _RegNum )
{
  Tx_Buf[_TxCount++] = _RegNum<<1;
  for(uint8_t i = 0;i< _RegNum;i++)
  {
    Tx_Buf[_TxCount++] = ((*_AddrOffset)>>8);
    Tx_Buf[_TxCount++] = *_AddrOffset++;
  }
  return _TxCount;
}
/**
  * ��������: ����05H��������䷢�ͻ�����
  * �������: _TxCount :���ͼ�����,_AddrOffset��ַƫ����,_AddrAbs:���Ե�ֵַ
  * �� �� ֵ: Tx_Buf������Ԫ������
  * ˵    ��: ���Tx_Buf
  */
uint8_t MB_RSP_05H(uint16_t _TxCount,uint16_t _AddrOffset ,uint8_t *_AddrAbs)
{
  /* ����ֵַ */
  Tx_Buf[_TxCount++] = _AddrOffset>>8;
  Tx_Buf[_TxCount++] = _AddrOffset;
  /* ������ֵ */
  if((*_AddrAbs) == 1)
    Tx_Buf[_TxCount++] = 0xFF;
  else Tx_Buf[_TxCount++] = 0x00;
  Tx_Buf[_TxCount++] = 0x00;
  return _TxCount;
}
/**
  * ��������: ����06H��������䷢�ͻ�����
  * �������: _TxCount :���ͼ�����,_AddrOffset��ַƫ����,_AddrAbs:���Ե�ֵַ
  * �� �� ֵ: Tx_Buf������Ԫ������
  * ˵    ��: ���Tx_Buf
  */
uint8_t MB_RSP_06H(uint16_t _TxCount,uint16_t _AddrOffset ,uint16_t *_AddrAbs)
{
  /* ����ֵַ */
  Tx_Buf[_TxCount++] = _AddrOffset>>8;
  Tx_Buf[_TxCount++] = _AddrOffset;
  /* ������ֵ */
  Tx_Buf[_TxCount++] = (*_AddrAbs)>>8;
  Tx_Buf[_TxCount++] = *_AddrAbs;
  return _TxCount;
}
/**
  * ��������: ����10H��������䷢�ͻ�����
  * �������: _TxCount :���ͼ�����,_AddrOffset��ַƫ����,_ByteNum:�ֽ�����
  * �� �� ֵ: Tx_Buf������Ԫ������
  * ˵    ��: ���Tx_Buf
  */
uint8_t MB_RSP_10H(uint16_t _TxCount,uint16_t _AddrOffset,uint16_t _ByteNum)
{
  Tx_Buf[_TxCount++] = _AddrOffset>>8;
  Tx_Buf[_TxCount++] = _AddrOffset;
  
  Tx_Buf[_TxCount++] = _ByteNum>>8;
  Tx_Buf[_TxCount++] = _ByteNum;
  return _TxCount;
}
/**
  * ��������: �쳣��Ӧ
  * �������: _FunCode :�����쳣�Ĺ�����,_ExCode:�쳣��
  * �� �� ֵ: ��
  * ˵    ��: ��ͨ������֡�����쳣ʱ,�����쳣��Ӧ
  */
void MB_Exception_RSP(uint8_t _FunCode,uint8_t _ExCode)
{
	uint16_t TxCount = 0;
	uint16_t crc = 0;
	Tx_Buf[TxCount++] = MB_SLAVEADDR;		    /* ��վ��ַ */
	Tx_Buf[TxCount++] = _FunCode|0x80;		  /* ������ + 0x80*/	
	Tx_Buf[TxCount++] = _ExCode ;	          /* �쳣��*/
	
	crc = MB_CRC16((uint8_t*)&Tx_Buf,TxCount);
	Tx_Buf[TxCount++] = crc;	          /* crc ���ֽ� */
	Tx_Buf[TxCount++] = crc>>8;		      /* crc ���ֽ� */
	USART2_TX((uint8_t *)Tx_Buf,TxCount);
}
/**
  * ��������: ������Ӧ
  * �������: _FunCode :������
  * �� �� ֵ: ��
  * ˵    ��: ��ͨ������֡û���쳣ʱ���ҳɹ�ִ��֮��,������Ӧ����֡
  */
void MB_RSP(uint8_t _FunCode)
{
//	tsData data;
	uint16_t TxCount = 0;
	uint16_t crc = 0;	
	Tx_Buf[TxCount++] = MB_SLAVEADDR;		 /* ��վ��ַ */
	Tx_Buf[TxCount++] = _FunCode;        /* ������   */	
  switch(_FunCode)
  {
    case FUN_CODE_01H:
    case FUN_CODE_02H:
      TxCount = MB_RSP_01H(TxCount,(uint8_t*)PduData.PtrCoilOffset,PduData.Num);
      break;
    case FUN_CODE_03H:
    case FUN_CODE_04H:
      TxCount = MB_RSP_03H(TxCount,(uint16_t*)PduData.PtrHoldingOffset,PduData.Num);
      break;
    case FUN_CODE_05H:
      TxCount = MB_RSP_05H(TxCount,PduData.Addr,(uint8_t*)PduData.PtrCoilOffset);
      break;
    case FUN_CODE_06H:
      TxCount = MB_RSP_06H(TxCount,PduData.Addr,(uint16_t*)PduData.PtrHoldingOffset);
		//�������
//		data.NetAddr=MB_SLAVEADDR;
//		data.SlaveAddr=PduData.Addr;
//		data.Cmd=PduData.Code;
//		data.Val=PduData.Num;
//		xQueueSendToFront(DataQueueHandle,&data,NULL);//�������,�ڶ���ǰ����
//		//�ط�һ�β�ѯ
//		retyCmd_Data=data;
//		osTimerStart(retryTimerHandle,RETRY_TIMER*1000);
      break;
    case FUN_CODE_10H:
      TxCount = MB_RSP_10H(TxCount,PduData.Addr,PduData.Num);
      break;
  }
	crc = MB_CRC16((uint8_t*)&Tx_Buf,TxCount);
	Tx_Buf[TxCount++] = crc;	          /* crc ���ֽ� */
	Tx_Buf[TxCount++] = crc>>8;		      /* crc ���ֽ� */
	USART2_TX((uint8_t *)Tx_Buf,TxCount);
}
/*
 * �ϵ��ѯ��10��ʼ��num
 *
 * 
*/
//void Power_Query(uint8_t num)
//{
//	tsData data;
//	for(int i=10;i<num;i++)
//	{
//		data.NetAddr=MB_SLAVEADDR;
//		data.SlaveAddr=i;
//		data.Cmd=0x06;
//		data.Val=0x80;
//		xQueueSendToBack(DataQueueHandle,&data,NULL);//������ӣ��ڶ��к����	
//	}
//	
//}

