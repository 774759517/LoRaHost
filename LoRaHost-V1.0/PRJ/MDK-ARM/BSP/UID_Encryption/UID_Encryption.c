#include "UID_Encryption.h"


uint8_t checkResult = 0; //验证结果
uint8_t myID[12] = {
                0x01, 0x03, 0x08, 0x03,
                0x08, 0x03, 0x01, 0x03, 
                0x09, 0x03, 0x05, 0x00};//用户自定义ID

   
/****************************************** UID加密算法 start ******************************************/
/* UID加密算法0 */
static void Algorithm_0(char *pCustomID, char *pUID, char KeyLength, char *pKey)
{    
    pKey[0] = pUID[0]    ^ (pCustomID[1] & pCustomID[2]) ^ pCustomID[3];
    pKey[1] = pCustomID[1] | (pUID[1]    ^ (pCustomID[3] & pCustomID[4]));
    pKey[2] = (pCustomID[2] ^ pCustomID[1]) | (pUID[2]    ^ pCustomID[5]);
    pKey[3] = ((pCustomID[3] & pCustomID[1]) ^ pCustomID[5]) | pUID[3];
    
    if((KeyLength == 8) || (KeyLength == 12))  
    {
        pKey[4] = pUID[4]    ^ (pCustomID[5] & pCustomID[6]) ^ pCustomID[7];
        pKey[5] = pCustomID[5] | (pUID[5]    ^ (pCustomID[7] & pCustomID[8]));
        pKey[6] = (pCustomID[6] ^ pCustomID[7]) | (pUID[6]    ^ pCustomID[9]);
        pKey[7] = ((pCustomID[7] & pCustomID[8]) ^ pCustomID[9]) | pUID[7];
    }
    if(KeyLength == 12)
    {
        pKey[8] = pUID[8]    ^ (pCustomID[9] & pCustomID[10]) ^ pCustomID[11];
        pKey[9] = pCustomID[9] | (pUID[9]    ^ (pCustomID[11] & pCustomID[0]));
        pKey[10]= (pCustomID[10] ^ pCustomID[11]) | (pUID[0]   ^ pCustomID[1]);
        pKey[11]= ((pCustomID[11] & pCustomID[0]) ^ pCustomID[1]) | pUID[2];
    }
       
}

/* UID加密算法1 */
static void Algorithm_1(char *pCustomID, char *pUID, char KeyLength, char *pKey)
{
    pKey[0] = pUID[0]    ^ pCustomID[1] ^ pCustomID[2] ^ pCustomID[3];
    pKey[1] = pCustomID[1] ^ pUID[1]    ^ pCustomID[3] ^ pCustomID[4];
    pKey[2] = pCustomID[2] ^ pCustomID[1] ^ pUID[2]    ^ pCustomID[5];
    pKey[3] = pCustomID[3] ^ pCustomID[1] ^ pCustomID[5] ^ pUID[3];
    
    if((KeyLength == 8) || (KeyLength == 12))  
    {
        pKey[4] = pUID[4]    ^ pCustomID[5] ^ pCustomID[6] ^ pCustomID[7];
        pKey[5] = pCustomID[5] ^ pUID[5]    ^ pCustomID[7] ^ pCustomID[8];
        pKey[6] = pCustomID[6] ^ pCustomID[7] ^ pUID[6]    ^ pCustomID[9];
        pKey[7] = pCustomID[7] ^ pCustomID[8] ^ pCustomID[9] ^ pUID[7];
    }
    if(KeyLength == 12)
    {
        pKey[8] = pUID[8]    ^ pCustomID[9] ^ pCustomID[10] ^ pCustomID[11];
        pKey[9] = pCustomID[9] ^ pUID[9]    ^ pCustomID[11] ^ pCustomID[0];
        pKey[10]= pCustomID[10] ^ pCustomID[11] ^ pUID[0]   ^ pCustomID[1];
        pKey[11]= pCustomID[11] ^ pCustomID[0] ^ pCustomID[1] ^ pUID[2];
    }
}
    
/* UID加密算法2 */
static void Algorithm_2(char *pCustomID, char *pUID, char KeyLength, char *pKey)
{
    pKey[0] = (pUID[0]    & pCustomID[1]) ^ pCustomID[2] ^ pCustomID[3];
    pKey[1] = pCustomID[1] ^ (pUID[1]    & pCustomID[3]) ^ pCustomID[4];
    pKey[2] = pCustomID[2] | (pCustomID[1] ^ pUID[2])    | pCustomID[5];
    pKey[3] = (pCustomID[3] ^ pCustomID[1]) | (pCustomID[5] ^ pUID[3]);
    
    if((KeyLength == 8) || (KeyLength == 12))  
    {
        pKey[4] = pUID[4]    ^ (pCustomID[5] & pCustomID[6]) ^ pCustomID[7];
        pKey[5] = pCustomID[5] | (pUID[5]    ^ ((pCustomID[7]) & pCustomID[8]));
        pKey[6] = ((pCustomID[6] & pCustomID[7]) ^ pUID[6])    | pCustomID[9];
        pKey[7] = (pCustomID[7] ^ pCustomID[8]) | (pCustomID[9] ^ pUID[7]);
    }
    if(KeyLength == 12)
    {
        pKey[8] = pUID[8]    ^ (pCustomID[9] & pCustomID[10]) ^ pCustomID[11];
        pKey[9] = (pCustomID[9] ^ pUID[9])    | (pCustomID[11] & pCustomID[0]);
        pKey[10]= (pCustomID[10] & pCustomID[11]) ^ pUID[0]   ^ pCustomID[1];
        pKey[11]= pCustomID[11] ^ (pCustomID[0] & pCustomID[1]) ^ pUID[2];
    }
}
    
/* UID加密算法3 */
static void Algorithm_3(char *pCustomID, char *pUID, char KeyLength, char *pKey)
{
    pKey[0] = (pUID[0] & pCustomID[1]) ^ (pCustomID[2] ^ pCustomID[3]);
    pKey[1] = pUID[1] ^ ((pCustomID[2] & pCustomID[3]) ^ pCustomID[4]);
    pKey[2] = pUID[2] | (pCustomID[3] ^ pCustomID[4]) | pCustomID[5];
    pKey[3] = pUID[3] ^ pCustomID[4] | pCustomID[5] ^ pCustomID[6];
    
    if((KeyLength == 8) || (KeyLength == 12))  
    {
        pKey[4] = pCustomID[4] ^ pUID[5] & pCustomID[6] ^ pCustomID[7];
        pKey[5] = pCustomID[5] | pUID[6] ^ pCustomID[7] & pCustomID[8];
        pKey[6] = pCustomID[6] & pUID[7] ^ pCustomID[6] | pCustomID[9];
        pKey[7] = pCustomID[7] ^ pUID[8] | pCustomID[9] ^ pCustomID[10];
    }
    if(KeyLength == 12)
    {
        pKey[8] = pCustomID[8]  ^ pCustomID[9]  & pUID[10] ^ pCustomID[11];
        pKey[9] = pCustomID[9]  ^ pCustomID[10] | pUID[11] & pCustomID[0];
        pKey[10]= pCustomID[10] & pCustomID[11] ^ pUID[0]  ^ pCustomID[1];
        pKey[11]= pCustomID[11] ^ pCustomID[0]  & pUID[1]  ^ pCustomID[2];
    }
}
    
/* UID加密算法4 */
static void Algorithm_4(char *pCustomID, char *pUID, char KeyLength, char *pKey) 
{
    pKey[0] = pUID[0] & pCustomID[1] ^ pUID[2] & pCustomID[3];
    pKey[1] = pCustomID[1] ^ pUID[2] & pCustomID[3] ^ pUID[4];
    pKey[2] = pCustomID[2] | pUID[3] ^ pUID[4] | pCustomID[5];
    pKey[3] = pUID[3] - pCustomID[4] | pCustomID[5] ^ pUID[6];
    
    if((KeyLength == 8) || (KeyLength == 12))  
    {
        pKey[4] = pUID[4] ^ pCustomID[5] & pUID[6] ^ pCustomID[7];
        pKey[5] = pCustomID[5] | pUID[6] ^ pCustomID[7] & pUID[8];
        pKey[6] = pCustomID[6] & pUID[7] ^ pCustomID[6] | pUID[9];
        pKey[7] = pUID[7] - pCustomID[8] | pUID[9] ^ pCustomID[10];
    }
    if(KeyLength == 12)
    {        
        pKey[8] = pCustomID[8] ^ pUID[9] & pCustomID[10] ^ pUID[11];
        pKey[9] = pUID[9] ^ pCustomID[10] | pUID[11] & pCustomID[0];
        pKey[10]= pUID[10] & pCustomID[11] ^ pUID[0]  ^ pCustomID[1];
        pKey[11]= pCustomID[11] - pUID[0] & pCustomID[1]  ^ pUID[2];
    }
}



/* 定义UID加密算法 函数指针类型 */
typedef void (*Algorithm_Fun_Typedef)(char *pCustomID, char *pUID, char KeyLength, char *pKey);
/* 定义UID加密算法 函数指针数组 */
static const Algorithm_Fun_Typedef Algorithm_Fun_Array[5] = 
{
    Algorithm_0,
    Algorithm_1,
    Algorithm_2,
    Algorithm_3,
    Algorithm_4,
};
/****************************************** UID加密算法 end ******************************************/    



/* 大小端序切换 */      
#define BigLittleSwap32(A)			((((unsigned int)(A) & 0xFF000000) >> 24) | \
                                     (((unsigned int)(A) & 0x00FF0000) >> 8)  | \
                                     (((unsigned int)(A) & 0x0000FF00) << 8)  | \
                                     (((unsigned int)(A) & 0x000000FF) << 24)) 
                                        
void LittleEndia_BigEndia_Interconvert_32(unsigned int* pBuffer, unsigned int WordLen)
{
    unsigned int i;
    
    for(i = 0; i < WordLen; i ++)
    {
        pBuffer[i] = BigLittleSwap32(pBuffer[i]);
    }
    return;
}  



static Algorithm_Fun_Typedef   pAlgorithm_Fun;
/****************** UID 算法加密密钥验证 ******************/
char UID_Encryption_Key_Check(void *pKey,
                              void *pUID,
                              void *pCustomID,
                              eKeyLengthType keyLength, 
                              eEndiaType endiaType, 
                              eAlgorithmType AlgorithmNum)
{   
    char KeyBuf[12] = {0};//临时存储计算得到的Key
    
    
    /* 选择算法公式 */
    pAlgorithm_Fun = Algorithm_Fun_Array[AlgorithmNum];
    /* 计算KEY */
    (*pAlgorithm_Fun)(pCustomID, pUID, keyLength, KeyBuf);   

    if(endiaType == BIG_ENDIA)
    {
        LittleEndia_BigEndia_Interconvert_32((unsigned int*)KeyBuf, keyLength/sizeof(unsigned int));
    }
    /* 比较脱机下载器烧录的Key与使用相同算法计算得到的key是否相同 */
    if(memcmp(pKey, KeyBuf, keyLength)) 
    {
        return 1;
    }
    
    return 0; 
}
/****************** UID 算法加密密钥计算 ******************/
void UID_Encryption_Key_Calculate(void *pKey,
                                 void *pUID,
                                 void *pCustomID,
                                 eKeyLengthType keyLength, 
                                 eEndiaType endiaType, 
                                 eAlgorithmType AlgorithmNum)
{    
    /* 选择算法公式 */
    pAlgorithm_Fun = Algorithm_Fun_Array[AlgorithmNum];
    /* 计算KEY */
    (*pAlgorithm_Fun)(pCustomID, pUID, keyLength, pKey);   

    if(endiaType == BIG_ENDIA)
    {
        LittleEndia_BigEndia_Interconvert_32((unsigned int*)pKey, keyLength/sizeof(unsigned int));
    }
    
    return; 
}








