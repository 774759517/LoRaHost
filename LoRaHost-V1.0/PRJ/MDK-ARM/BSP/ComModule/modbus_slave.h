#ifndef __MODBUS_SLAVE_H__
#define __MODBUS_SLAVE_H__
/* ����ͷ�ļ� ----------------------------------------------------------------*/
#include "main.h"
#include "usart.h"
#include "crc.h"

/* ���Ͷ��� ------------------------------------------------------------------*/
typedef struct {
  __IO uint8_t  Code ;  	        // ������
  __IO uint8_t byteNums; 	        // �ֽ���
  __IO uint16_t Addr ;            // �����ڴ����ʼ��ַ
  __IO uint16_t Num; 	            // �Ĵ���������Ȧ������
  __IO uint16_t _CRC;       	      // CRCУ����
  __IO uint8_t *ValueReg; 	      // 10H�����������
  __IO uint8_t *PtrCoilbase;		  // Coil��Input�ڴ��׵�ַ
  __IO uint8_t *PtrCoilOffset;    // Coil��Inputƫ���ڴ��׵�ַ
  __IO uint16_t *PtrHoldingbase;  // HoldingReg�ڴ��׵�ַ
  __IO uint16_t *PtrHoldingOffset;// HoldingReg�ڴ��׵�ַ
}PDUData_TypeDef;

/* �궨�� --------------------------------------------------------------------*/
#define MB_SLAVEADDR            0x0001
#define MB_ALLSLAVEADDR         0x00FF
#define MB_SYSVALADDR			0x00EE

#define FUN_CODE_01H            0x01  // ������01H 
#define FUN_CODE_02H            0x02  // ������02H
#define FUN_CODE_03H            0x03  // ������03H
#define FUN_CODE_04H            0x04  // ������04H
#define FUN_CODE_05H            0x05  // ������05H
#define FUN_CODE_06H            0x06  // ������06H
#define FUN_CODE_10H            0x10  // ������10H


#define MSG_ERR_FLAG  0xFFFF    // ���մ��� �ַ��䳬ʱ
#define MSG_IDLE      0x0000    // ����״̬
#define MSG_RXING     0x0001    // ���ڽ�������
#define MSG_COM       0x0002    // �������
#define MSG_INC       0x8000    // ����֡������(���ַ���Ŀ��м������1.5���ַ�ʱ��)
#define TIME_OVERRUN  100       // ���峬ʱʱ�� ms

#define RETRY_TIMER		50

/* ��������֧�ֵĹ�����,��Ҫ����¹����뻹��Ҫ��.c�ļ�������� */
#define IS_NOT_FUNCODE(code)  (!((code == FUN_CODE_01H)||\
                                 (code == FUN_CODE_02H)||\
                                 (code == FUN_CODE_03H)||\
                                 (code == FUN_CODE_04H)||\
                                 (code == FUN_CODE_05H)||\
                                 (code == FUN_CODE_06H)||\
                                 (code == FUN_CODE_10H)))


#define EX_CODE_NONE           0x00  // �쳣�� ���쳣
#define EX_CODE_01H            0x01  // �쳣��
#define EX_CODE_02H            0x02  // �쳣��
#define EX_CODE_03H            0x03  // �쳣��
#define EX_CODE_04H            0x04  // �쳣��
/* ��չ���� ------------------------------------------------------------------*/
extern PDUData_TypeDef PduData;
extern __IO uint8_t Tx_Buf[256];            // ���ͻ���,���256�ֽ�
extern uint8_t Rx_Buf[256];
extern __IO uint16_t RxCount;
extern __IO uint16_t Reg_Val[256];
/* �������� ------------------------------------------------------------------*/
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
