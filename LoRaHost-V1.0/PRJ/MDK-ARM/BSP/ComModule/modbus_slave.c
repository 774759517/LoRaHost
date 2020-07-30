#include "modbus_slave.h"

/*变量定义*/
PDUData_TypeDef PduData;
__IO uint8_t Tx_Buf[256];            // 发送缓存,最大256字节
uint8_t Rx_Buf[256];
__IO uint16_t RxCount=0;
__IO uint16_t Reg_Val[256];

/*函数定义*/
static uint8_t MB_JudgeAddr(uint16_t _Addr,uint16_t _RegNum);//判断地址是否符合协议范围
static uint8_t MB_JudgeNum(uint16_t _RegNum,uint8_t _FunCode,uint16_t _ByteNum);
static uint8_t MB_10H_WR_NReg(uint16_t*Ptr,uint16_t NumofReg, uint8_t*buf);
static uint8_t MB_RSP_10H(uint16_t _TxCount,uint16_t _AddrOffset,uint16_t _ByteNum);
static uint16_t MB_RSP_01H(uint16_t _TxCount,uint8_t *_AddrOffset ,uint16_t _CoilNum);
static uint8_t MB_RSP_03H(uint16_t _TxCount,uint16_t *_AddrOffset,uint16_t _RegNum );
static uint8_t MB_RSP_05H(uint16_t _TxCount,uint16_t _AddrOffset ,uint8_t *_AddrAbs);
static uint8_t MB_RSP_06H(uint16_t _TxCount,uint16_t _AddrOffset ,uint16_t *_AddrAbs);
static uint8_t MB_RSP_10H(uint16_t _TxCount,uint16_t _AddrOffset,uint16_t _ByteNum);
/*函数体*/

/* 提取数据帧,进行解析数据帧 */
void MB_Parse_Data()
{
  PduData.Code = Rx_Buf[1];                   // 功能码
  PduData.Addr = ((Rx_Buf[2]<<8) | Rx_Buf[3]);// 寄存器起始地址
  PduData.Num  = ((Rx_Buf[4]<<8) | Rx_Buf[5]);// 数量(Coil,Input,Holding Reg,Input Reg)
  PduData._CRC = MB_CRC16((uint8_t*)&Rx_Buf,RxCount-2);             // CRC校验码
  PduData.byteNums = Rx_Buf[6];               // 获得字节数
  PduData.ValueReg = (uint8_t*)&Rx_Buf[7];                          // 寄存器值起始地址
  PduData.PtrCoilOffset = PduData.PtrCoilbase + PduData.Addr;       // 离散量的内存起始地址
  PduData.PtrHoldingOffset = PduData.PtrHoldingbase + PduData.Addr; // 保持寄存器的起始地址
}
/** 
  * 函数功能: 对接收到的数据进行分析并执行
  * 输入参数: 无
  * 返 回 值: 异常码或0x00
  * 说    明: 判断功能码,验证地址是否正确.数值内容是否溢出,数据没错误就发送响应信号
  */
uint8_t MB_Analyze_Execute(void )
{
  uint16_t ExCode = EX_CODE_NONE;
  /* 校验功能码 */
  if( IS_NOT_FUNCODE(PduData.Code) ) // 不支持的功能码
  {
    /* Modbus异常响应 */
    ExCode = EX_CODE_01H;            // 异常码01H
    return ExCode;
  }
  /* 根据功能码分别做判断 */
  switch(PduData.Code)
  {
    /* 这里认为01H功能码和02功能码是一样的,其实也没什么不一样
     * 只是操作地址可能不一样,这一点结合具体来实现,可以在main函数
     * 申请单独的内存使用不同的功能码,在实际应用中必须加以区分使用
     * 不同的内存空间
     */
/* ---- 01H  02H 读取离散量输入(Coil Input)---------------------- */
    case FUN_CODE_01H:
    case FUN_CODE_02H:
      /* 判断线圈数量是否正确 */  
      ExCode = MB_JudgeNum(PduData.Num,PduData.Code,1);
      if(ExCode != EX_CODE_NONE )
        return ExCode;      
      
      /* 判断地址是否正确*/
      ExCode = MB_JudgeAddr( PduData.Addr,PduData.Num);
      if(ExCode != EX_CODE_NONE )
        return ExCode;  
      
      /* 处理请求,读取线圈状态
       */

      break;
/* ---- 03H  04H 读取保持/输入寄存器---------------------- */
    case FUN_CODE_03H:
    case FUN_CODE_04H:
      /* 判断寄存器数量是否正确 */
      ExCode = MB_JudgeNum(PduData.Num,PduData.Code,PduData.byteNums);
      if(ExCode != EX_CODE_NONE )
        return ExCode;  
      
      /* 判断地址是否正确*/
      ExCode = MB_JudgeAddr( PduData.Addr,PduData.Num);
      if(ExCode != EX_CODE_NONE )
        return ExCode;  
      
      /* 处理请求,读保持寄存器

       */

      break;
/* ---- 05H 写入单个离散量---------------------- */
    case FUN_CODE_05H:
      /* 输出值!=OK */
      if((PduData.Num != 0x0000) && PduData.Num != 0xFF00)
      {
        ExCode = EX_CODE_03H;
        return ExCode;        
      }
      /* 写入一个线圈值 */
      if(PduData.Num == 0xFF00)
      { 
        *PduData.PtrCoilOffset = 1;
        /* 读取写入值,验证是否写入成功 */
        if( *PduData.PtrCoilOffset != 1)
        {
          ExCode = EX_CODE_04H;
          return ExCode;
        }
      }
      else 
      {
        *PduData.PtrCoilOffset = 0;
        /* 读取写入值,验证是否写入成功 */
        if( *PduData.PtrCoilOffset != 0)
        { 
          ExCode = EX_CODE_04H;
          return ExCode;
        }
      }
      break;
/* ---- 06H 写单个保持寄存器 ---------------------- */
    case FUN_CODE_06H:
      
      /* 写入寄存器值*/
      *PduData.PtrHoldingOffset = PduData.Num;
      /* 验证写成功 */
      if(*PduData.PtrHoldingOffset != PduData.Num)
      {
        ExCode = EX_CODE_04H;
         return ExCode;
      }
      break;
/* ---- 10H 写多个保持寄存器 ---------------------- */
    case FUN_CODE_10H:
      /* 判断寄存器数量是否正确 */
      ExCode = MB_JudgeNum(PduData.Num,PduData.Code,PduData.byteNums);
      if(ExCode != EX_CODE_NONE )
        return ExCode;    
      
      /* 判断地址是否正确*/
      ExCode = MB_JudgeAddr( PduData.Addr,PduData.Num);
      if(ExCode != EX_CODE_NONE )
        return ExCode;  
      
      /* 写入多个寄存器 */
      ExCode = MB_10H_WR_NReg((uint16_t*)PduData.PtrHoldingOffset,PduData.Num,(uint8_t*)PduData.ValueReg);
      if(ExCode != EX_CODE_NONE )
        return ExCode;  
      break;
  }
  /* 数据帧没有异常 */
  return ExCode; //   EX_CODE_NONE
}
/**
  * 函数功能: 写,读N个寄存器
  * 输入参数: _AddrOffset:偏移地址,_RegNum:寄存器数量,_Datebuf:数据指针
  * 返 回 值: 异常码:04H或NONE
  * 说    明: 在_AddrOffset所指向的空间里写入_RegNum*2个数据,并且读取验证是否写入成功
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
	 //数据发往savle 
//		data.NetAddr=MB_SLAVEADDR;
//		data.SlaveAddr=PduData.Addr+i;
//		data.Cmd=PduData.Code;
//		data.Val=Value;
//		xQueueSendToBack(DataQueueHandle,&data,NULL);//数据入队，在队列后插入
		_Datebuf+=2;
  }
  
  /* 读取验证写入是否成功 */
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
  * 函数功能: 判断操作的数据量是否符合协议范围
  * 输入参数: _RegNum:寄存器数量,_FunCode:功能码,_ByteNum:字节数量
  * 返 回 值: 异常码:03或NONE
  * 说    明: 对可操作连续内存空间的功能码需要验证操作的地址是否符合范围
  */
uint8_t MB_JudgeNum(uint16_t _RegNum,uint8_t _FunCode,uint16_t _ByteNum)
{
  uint8_t Excode = EX_CODE_NONE;
  uint16_t _CoilNum = _RegNum; // 线圈(离散量)的数量
  switch(_FunCode)
  {
    case FUN_CODE_01H: 
    case FUN_CODE_02H:
      if( (_CoilNum<0x0001) || (_CoilNum>0x07D0))
        Excode = EX_CODE_03H;// 异常码03H;
      break;
    case FUN_CODE_03H:
    case FUN_CODE_04H:
      if( (_RegNum<0x0001) || (_RegNum>0x007D))
        Excode = EX_CODE_03H;// 异常码03H;      
      break;
    case FUN_CODE_10H:
      if( (_RegNum<0x0001) || (_RegNum>0x007B))
        Excode = EX_CODE_03H;// 异常码03H
      if( _ByteNum != (_RegNum<<1))
        Excode = EX_CODE_03H;// 异常码03H
      break;
  }
  return Excode;
}
/**
  * 函数功能: 判断地址是否符合协议范围
  * 输入参数: _Addr:起始地址,_RegNum:寄存器数量,_FunCode:功能码
  * 返 回 值: 异常码:02H或NONE
  * 说    明: 地址范围是0x0000~0xFFFF,可操作的空间范围不能超过这个区域
  */
uint8_t MB_JudgeAddr(uint16_t _Addr,uint16_t _RegNum)
{
  uint8_t Excode = EX_CODE_NONE;
  /* 地址+寄存器数量不能超过0xFFFF */
  if( ((uint32_t)_RegNum+(uint32_t)_Addr) > (uint32_t)0xFFFF)
  {
    Excode = EX_CODE_02H;// 异常码 02H
  }
  return Excode;
}
/**
  * 函数功能: 读取离散输出
  * 输入参数: _TxCount :发送计数器,_AddrOffset地址偏移量,_CoilNum:线圈数量
  * 返 回 值: Tx_Buf的数组元素坐标
  * 说    明: 读取离散输出,并且填充Tx_Buf
  */
uint16_t MB_RSP_01H(uint16_t _TxCount,uint8_t *_AddrOffset ,uint16_t _CoilNum)
{
  uint16_t ByteNum = 0;
  uint16_t i = 0;
  uint16_t r = 0 ;
  ByteNum = _CoilNum/8;
  /* 如果存在余数,需要将多余的位置0 */
  r = _CoilNum%8;
  if(r != 0)
  {
    ByteNum += 1; //字节数+1
    Tx_Buf[_TxCount++] = ByteNum;  
    
    /* 在忽略余数的情况下读取线圈(bit)的状态 */
    for(i=0;i<ByteNum-1;i++)
    {
      Tx_Buf[_TxCount] = 0x00;
      /* 每8个线圈(bit)一次循环,读取8个线圈的状态放置在一个Byte*/
      for(uint8_t j=0;j<8;j++)
      {
        /* 如果是1,则将byte对应位(bit)置1 */
        if(*(_AddrOffset++))
        {
          /* 在这里相当于将一个Byte当成一个线圈(bit)映射到一个Byte上
           * 由8个Byte组成一个Byte的8个bit
           */
          Tx_Buf[_TxCount] |= (0x01<<j);
        }
      }
      _TxCount++;
    }
    /* 有余数部分组成一个字节,多余的bit设置为0 */
    Tx_Buf[_TxCount] = 0x00;
    for(uint8_t j=0;j<r;j++)
    {
      if(*(_AddrOffset++))
      {
        Tx_Buf[_TxCount] |= (0x01<<j);// 读取数据
      }
    }
    _TxCount++;
  }
  /* 如果余数r==0,说明需要读取的线圈数量(bits)刚好是Byte的整数倍 */
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
          Tx_Buf[_TxCount] |= (0x01<<j);// 读取数据
        }
      }
      _TxCount++;
    }
  }
  return _TxCount;
}
/**
  * 函数功能: 读取保持寄存器
  * 输入参数: _TxCount :发送计数器,_AddrOffset地址偏移量,_RegNum:寄存器数量
  * 返 回 值: Tx_Buf的数组元素坐标
  * 说    明: 读取保持寄存器的内容,并且填充Tx_Buf
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
  * 函数功能: 根据05H功能码填充发送缓冲区
  * 输入参数: _TxCount :发送计数器,_AddrOffset地址偏移量,_AddrAbs:绝对地址值
  * 返 回 值: Tx_Buf的数组元素坐标
  * 说    明: 填充Tx_Buf
  */
uint8_t MB_RSP_05H(uint16_t _TxCount,uint16_t _AddrOffset ,uint8_t *_AddrAbs)
{
  /* 填充地址值 */
  Tx_Buf[_TxCount++] = _AddrOffset>>8;
  Tx_Buf[_TxCount++] = _AddrOffset;
  /* 填充输出值 */
  if((*_AddrAbs) == 1)
    Tx_Buf[_TxCount++] = 0xFF;
  else Tx_Buf[_TxCount++] = 0x00;
  Tx_Buf[_TxCount++] = 0x00;
  return _TxCount;
}
/**
  * 函数功能: 根据06H功能码填充发送缓冲区
  * 输入参数: _TxCount :发送计数器,_AddrOffset地址偏移量,_AddrAbs:绝对地址值
  * 返 回 值: Tx_Buf的数组元素坐标
  * 说    明: 填充Tx_Buf
  */
uint8_t MB_RSP_06H(uint16_t _TxCount,uint16_t _AddrOffset ,uint16_t *_AddrAbs)
{
  /* 填充地址值 */
  Tx_Buf[_TxCount++] = _AddrOffset>>8;
  Tx_Buf[_TxCount++] = _AddrOffset;
  /* 填充输出值 */
  Tx_Buf[_TxCount++] = (*_AddrAbs)>>8;
  Tx_Buf[_TxCount++] = *_AddrAbs;
  return _TxCount;
}
/**
  * 函数功能: 根据10H功能码填充发送缓冲区
  * 输入参数: _TxCount :发送计数器,_AddrOffset地址偏移量,_ByteNum:字节数量
  * 返 回 值: Tx_Buf的数组元素坐标
  * 说    明: 填充Tx_Buf
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
  * 函数功能: 异常响应
  * 输入参数: _FunCode :发送异常的功能码,_ExCode:异常码
  * 返 回 值: 无
  * 说    明: 当通信数据帧发生异常时,发送异常响应
  */
void MB_Exception_RSP(uint8_t _FunCode,uint8_t _ExCode)
{
	uint16_t TxCount = 0;
	uint16_t crc = 0;
	Tx_Buf[TxCount++] = MB_SLAVEADDR;		    /* 从站地址 */
	Tx_Buf[TxCount++] = _FunCode|0x80;		  /* 功能码 + 0x80*/	
	Tx_Buf[TxCount++] = _ExCode ;	          /* 异常码*/
	
	crc = MB_CRC16((uint8_t*)&Tx_Buf,TxCount);
	Tx_Buf[TxCount++] = crc;	          /* crc 低字节 */
	Tx_Buf[TxCount++] = crc>>8;		      /* crc 高字节 */
	USART2_TX((uint8_t *)Tx_Buf,TxCount);
}
/**
  * 函数功能: 正常响应
  * 输入参数: _FunCode :功能码
  * 返 回 值: 无
  * 说    明: 当通信数据帧没有异常时并且成功执行之后,发送响应数据帧
  */
void MB_RSP(uint8_t _FunCode)
{
//	tsData data;
	uint16_t TxCount = 0;
	uint16_t crc = 0;	
	Tx_Buf[TxCount++] = MB_SLAVEADDR;		 /* 从站地址 */
	Tx_Buf[TxCount++] = _FunCode;        /* 功能码   */	
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
		//数据入队
//		data.NetAddr=MB_SLAVEADDR;
//		data.SlaveAddr=PduData.Addr;
//		data.Cmd=PduData.Code;
//		data.Val=PduData.Num;
//		xQueueSendToFront(DataQueueHandle,&data,NULL);//数据入队,在队列前插入
//		//重发一次查询
//		retyCmd_Data=data;
//		osTimerStart(retryTimerHandle,RETRY_TIMER*1000);
      break;
    case FUN_CODE_10H:
      TxCount = MB_RSP_10H(TxCount,PduData.Addr,PduData.Num);
      break;
  }
	crc = MB_CRC16((uint8_t*)&Tx_Buf,TxCount);
	Tx_Buf[TxCount++] = crc;	          /* crc 低字节 */
	Tx_Buf[TxCount++] = crc>>8;		      /* crc 高字节 */
	USART2_TX((uint8_t *)Tx_Buf,TxCount);
}
/*
 * 上电查询从10开始到num
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
//		xQueueSendToBack(DataQueueHandle,&data,NULL);//数据入队，在队列后插入	
//	}
//	
//}

