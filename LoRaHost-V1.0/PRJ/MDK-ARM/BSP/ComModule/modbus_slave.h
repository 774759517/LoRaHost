#ifndef __MODBUS_SLAVE_H__
#define __MODBUS_SLAVE_H__
/* 包含头文件 ----------------------------------------------------------------*/
#include "main.h"
#include "usart.h"
#include "crc.h"

/* 类型定义 ------------------------------------------------------------------*/
typedef struct {
  __IO uint8_t  Code ;  	        // 功能码
  __IO uint8_t byteNums; 	        // 字节数
  __IO uint16_t Addr ;            // 操作内存的起始地址
  __IO uint16_t Num; 	            // 寄存器或者线圈的数量
  __IO uint16_t _CRC;       	      // CRC校验码
  __IO uint8_t *ValueReg; 	      // 10H功能码的数据
  __IO uint8_t *PtrCoilbase;		  // Coil和Input内存首地址
  __IO uint8_t *PtrCoilOffset;    // Coil和Input偏移内存首地址
  __IO uint16_t *PtrHoldingbase;  // HoldingReg内存首地址
  __IO uint16_t *PtrHoldingOffset;// HoldingReg内存首地址
}PDUData_TypeDef;

/* 宏定义 --------------------------------------------------------------------*/
#define MB_SLAVEADDR            0x0001
#define MB_ALLSLAVEADDR         0x00FF
#define MB_SYSVALADDR			0x00EE

#define FUN_CODE_01H            0x01  // 功能码01H 
#define FUN_CODE_02H            0x02  // 功能码02H
#define FUN_CODE_03H            0x03  // 功能码03H
#define FUN_CODE_04H            0x04  // 功能码04H
#define FUN_CODE_05H            0x05  // 功能码05H
#define FUN_CODE_06H            0x06  // 功能码06H
#define FUN_CODE_10H            0x10  // 功能码10H


#define MSG_ERR_FLAG  0xFFFF    // 接收错误 字符间超时
#define MSG_IDLE      0x0000    // 空闲状态
#define MSG_RXING     0x0001    // 正在接收数据
#define MSG_COM       0x0002    // 接收完成
#define MSG_INC       0x8000    // 数据帧不完整(两字符间的空闲间隔大于1.5个字符时间)
#define TIME_OVERRUN  100       // 定义超时时间 ms

#define RETRY_TIMER		50

/* 本例程所支持的功能码,需要添加新功能码还需要在.c文件里面添加 */
#define IS_NOT_FUNCODE(code)  (!((code == FUN_CODE_01H)||\
                                 (code == FUN_CODE_02H)||\
                                 (code == FUN_CODE_03H)||\
                                 (code == FUN_CODE_04H)||\
                                 (code == FUN_CODE_05H)||\
                                 (code == FUN_CODE_06H)||\
                                 (code == FUN_CODE_10H)))


#define EX_CODE_NONE           0x00  // 异常码 无异常
#define EX_CODE_01H            0x01  // 异常码
#define EX_CODE_02H            0x02  // 异常码
#define EX_CODE_03H            0x03  // 异常码
#define EX_CODE_04H            0x04  // 异常码
/* 扩展变量 ------------------------------------------------------------------*/
extern PDUData_TypeDef PduData;
extern __IO uint8_t Tx_Buf[256];            // 发送缓存,最大256字节
extern uint8_t Rx_Buf[256];
extern __IO uint16_t RxCount;
extern __IO uint16_t Reg_Val[256];
/* 函数声明 ------------------------------------------------------------------*/
void MB_Parse_Data(void);
//void MB_Parse_Data(uint8_t *RxBuf,uint8_t RxCount);
void MB_WriteNumHoldingReg_10H(uint8_t _addr, uint16_t _reg, uint16_t _num,uint8_t *_databuf);
uint8_t MB_Analyze_Execute(void );
uint8_t MB_JudgeNum(uint16_t _Num,uint8_t _FunCode,uint16_t ByteNum);
uint8_t MB_JudgeAddr(uint16_t _Addr,uint16_t _Num);
void MB_Exception_RSP(uint8_t _FunCode,uint8_t _ExCode);
void MB_RSP(uint8_t _FunCode);
void Power_Query(uint8_t num);
#endif
