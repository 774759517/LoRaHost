#pragma once
#include "string.h"
#include "stm32f1xx_hal.h"


#define KEY_LOCATION   0x0800E000  //��Կ�ڵ�ǰоƬ�еĴ洢��ַ
#define UID_LOCATION   0x1FFFF7E8  //��ǰоƬUID���ڵ�ַ
#define UID_ENABLE
extern uint8_t checkResult; //��֤���
extern uint8_t myID[12];
/* 
V1.1:
    
    ���£�
        MINI���ѻ�������V1,3A5��̼�ʹ��V1.1�汾���룬V1.1�汾�����޸���V1.0�汾
        ����UID_Encryption_Key_Check(),UID_Encryption_Key_Calculate()ʹ�ô��
        ģʽ�п��ܳ��ֵ��ڴ�Խ�����⡣
    Ӱ�죺
        ǰ��������
            (1).��������MINI���ѻ��������Ĺ̼���V1.3A5��ʹ��MINI�ѻ����������õ���V1.1�汾����;
            (2).��ǰ ʹ��V1.3A4��֮ǰ�汾�̼���MINI�ѻ������� ����Ϊ���ģʽ�����û�����ʹ�õ��������V1.0�汾���룻
        ��������
            (1).����ʹ��V1.3A5�汾�̼���MINI�ѻ���������������UID�Զ�����ܹ��ܣ�Ȼ�������¼������V1.0�汾������û�����
            ����V1.1�汾����Ĵ��ģʽ��Կ��V1.0�汾�������Կ��һ�£���˻ᵼ��оƬ�л���ֻ������Կ��֤��ͨ�������⣡
        ����취��
            (1).���û������е�V1.0�����ͬ�����µ�V1.1����롣
        ����Ӱ��������
            (1).��ǰʹ�� С��ģʽ ���õ��û���Ʒ�����ܵ�V1.1�汾��������Լ�MINI�ѻ��������̼����µ�Ӱ�죻
            (2).����Ѿ�ʹ��V1.0����Ĳ�Ʒ����ʹ��IAP��ʽ�����򲻻��ܵ�Ӱ�죻
            (3).����Ѿ�ʹ��V1.0����Ĳ�Ʒ����ʹ��MINI�ѻ�������������Ʒ �� ������UID�Զ�����ܹ������¸�����Կ�����ܵ�Ӱ�죬
            �������ʹ���ѻ�������������Ʒ�Ļ�����������û�������˵�UID�Զ�����ܴ���ΪV1.1��ͬʱ����UID�Զ���������¸�����Կ��
            (4)MINI-Pro���ѻ��������װ�̼���ʹ�õ���V1.1�汾�Ĵ��룬����Ӱ�죻
V1.0��
    MINI���ѻ�������V1.3A4��̼���֮ǰ�Ĺ̼��о�ʹ����V1.0�汾�Ĵ��룻
*/

typedef enum 
{
    LENGTH_4 = 4,
    LENGTH_8 = 8,
    LENGTH_12 = 12
    
}eKeyLengthType;

typedef enum 
{
    LITTLE_ENDIA = 0,
    BIG_ENDIA
    
}eEndiaType;


typedef enum 
{
    ALGORITHM_0 = 0,
    ALGORITHM_1,
    ALGORITHM_2,
    ALGORITHM_3,
    ALGORITHM_4
    
}eAlgorithmType;


/****************** �������º��������Կ��֤ ******************/
/*
    ���룺
        pKey, ָ����Կ�Ĵ洢�����ָ�룬��Կ����ʼ�洢��ַ
        pUID, ָ��UID�Ĵ洢�����ָ�룬 UID����ʼ�洢��ַ
        pCustomID, ָ���û��趨���Զ���ID�洢�����ָ��
        keyLength, ��Կ���ֽڳ��ȣ���ѡ��ο�ö������ eKeyLengthType�Ķ���
        endiaType, ��С����ѡ�񣬿�ѡ��ο�ö������ eEndiaType�Ķ���
        AlgorithmNum������UID������Կ���㷨ѡ�񣬿�ѡ��ο�ö������ eAlgorithmType�Ķ���
    �����
        ��
    ���أ�
        ������Կ��֤�Ľ����
        0����ȷ
        1������
*/
char UID_Encryption_Key_Check(void *pKey,                      //[IN]
                              void *pUID,                      //[IN]
                              void *pCustomID,                 //[IN]
                              eKeyLengthType keyLength,        //[IN]
                              eEndiaType endiaType,            //[IN]
                              eAlgorithmType AlgorithmNum);    //[IN]
/****************** �������º��������Կ��֤ ******************/
/*
    ���룺
        pUID, ָ��UID�Ĵ洢�����ָ���û�Ӧ��Ԥ�ȷ���ã� UID����ʼ�洢��ַ
        pCustomID, ָ���û��趨���Զ���ID�洢�����ָ��
        keyLength, ��Կ���ֽڳ��ȣ���ѡ��ο�ö������ eKeyLengthType�Ķ���
        endiaType, ��С����ѡ�񣬿�ѡ��ο�ö������ eEndiaType�Ķ���
        AlgorithmNum������UID������Կ���㷨ѡ�񣬿�ѡ��ο�ö������ eAlgorithmType�Ķ���
    �����
        pKey, ָ��һ���ڴ��������ڴ�ż��������Կ
    ���أ�
        ������Կ��֤�Ľ����
        0����ȷ
        1������
*/
void UID_Encryption_Key_Calculate(void *pKey,                  //[OUT]
                                  void *pUID,                  //[IN]
                                  void *pCustomID,             //[IN]
                                  eKeyLengthType keyLength,    //[IN]
                                  eEndiaType endiaType,        //[IN]
                                  eAlgorithmType AlgorithmNum);//[IN]

