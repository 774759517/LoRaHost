/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */     
#include "user_dataType.h"
#include "usart.h"
#include "RfConfig.h"
#include "combus.h"
#include "modbus_slave.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */

/* USER CODE END Variables */
osThreadId UIDTaskHandle;
osThreadId Usart2TaskHandle;
osThreadId LoRaProcessHandle;
osThreadId LoRaTaskHandle;
osThreadId Usart1TaskHandle;
osMessageQId Usart2QueueHandle;
osMessageQId LoRaQueueHandle;
osMessageQId Usart1QueueHandle;

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */
   
/* USER CODE END FunctionPrototypes */

void UID_Task(void const * argument);
void Usart2_Task(void const * argument);
void LoRa_Process(void const * argument);
void LoRa_Task(void const * argument);
void Usart1_Task(void const * argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/* GetIdleTaskMemory prototype (linked to static allocation support) */
void vApplicationGetIdleTaskMemory( StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize );

/* USER CODE BEGIN GET_IDLE_TASK_MEMORY */
static StaticTask_t xIdleTaskTCBBuffer;
static StackType_t xIdleStack[configMINIMAL_STACK_SIZE];
  
void vApplicationGetIdleTaskMemory( StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize )
{
  *ppxIdleTaskTCBBuffer = &xIdleTaskTCBBuffer;
  *ppxIdleTaskStackBuffer = &xIdleStack[0];
  *pulIdleTaskStackSize = configMINIMAL_STACK_SIZE;
  /* place for user code */
}                   
/* USER CODE END GET_IDLE_TASK_MEMORY */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */
       
  /* USER CODE END Init */

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* Create the queue(s) */
  /* definition and creation of Usart2Queue */
  osMessageQDef(Usart2Queue, 256, uint8_t);
  Usart2QueueHandle = osMessageCreate(osMessageQ(Usart2Queue), NULL);

  /* definition and creation of LoRaQueue */
  osMessageQDef(LoRaQueue, 10, tsBuf);
  LoRaQueueHandle = osMessageCreate(osMessageQ(LoRaQueue), NULL);

  /* definition and creation of Usart1Queue */
  osMessageQDef(Usart1Queue, 16, uint8_t);
  Usart1QueueHandle = osMessageCreate(osMessageQ(Usart1Queue), NULL);

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* definition and creation of UIDTask */
  osThreadDef(UIDTask, UID_Task, osPriorityNormal, 0, 128);
  UIDTaskHandle = osThreadCreate(osThread(UIDTask), NULL);

  /* definition and creation of Usart2Task */
  osThreadDef(Usart2Task, Usart2_Task, osPriorityNormal, 0, 512);
  Usart2TaskHandle = osThreadCreate(osThread(Usart2Task), NULL);

  /* definition and creation of LoRaProcess */
  osThreadDef(LoRaProcess, LoRa_Process, osPriorityNormal, 0, 128);
  LoRaProcessHandle = osThreadCreate(osThread(LoRaProcess), NULL);

  /* definition and creation of LoRaTask */
  osThreadDef(LoRaTask, LoRa_Task, osPriorityNormal, 0, 128);
  LoRaTaskHandle = osThreadCreate(osThread(LoRaTask), NULL);

  /* definition and creation of Usart1Task */
  osThreadDef(Usart1Task, Usart1_Task, osPriorityNormal, 0, 128);
  Usart1TaskHandle = osThreadCreate(osThread(Usart1Task), NULL);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

}

/* USER CODE BEGIN Header_UID_Task */
/**
  * @brief  Function implementing the UIDTask thread.
  * @param  argument: Not used 
  * @retval None
  */
/* USER CODE END Header_UID_Task */
void UID_Task(void const * argument)
{
  /* USER CODE BEGIN UID_Task */
  /* Infinite loop */
  for(;;)
  {
//	HAL_GPIO_TogglePin(D1_GPIO_Port,D1_Pin);
    osDelay(100);
  }
  /* USER CODE END UID_Task */
}

/* USER CODE BEGIN Header_Usart2_Task */
/**
* @brief Function implementing the Usart2Task thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_Usart2_Task */
void Usart2_Task(void const * argument)
{
  /* USER CODE BEGIN Usart2_Task */
	uint8_t aRxBuffer[USART2_RX_BUFFERSIZE]={0};
	uint8_t u8Index=0;
	tsBuf data;
	uint16_t crc_check=0;
	uint8_t Ex_code=0;
	__IO uint16_t Rx_MSG=MSG_IDLE;
  /* Infinite loop */
  for(;;)
  {
	u8Index=0;
	if(xQueueReceive(Usart2QueueHandle,&aRxBuffer[u8Index++],portMAX_DELAY)== pdTRUE)
	{	
		while(xQueueReceive(Usart2QueueHandle,&aRxBuffer[u8Index++],5)){}	
		//接收到一帧数据
		HAL_GPIO_WritePin(D2_GPIO_Port,D2_Pin,GPIO_PIN_RESET);//开485灯
		aRxBuffer[u8Index]='\0';
		u8Index=u8Index-1;
		//user_info("count = %d | data = %s",u8Index,aRxBuffer);
#if MODBUS_MODE
		/*复制数据*/
		memcpy((uint8_t *)Rx_Buf,aRxBuffer,u8Index);
		RxCount=u8Index;
		for(int i=0;i<RxCount;i++)
		{
			printf("%02X ",Rx_Buf[i]);
		}
		printf("\r\n");
		/*为寄存器分配内存*/
		PduData.PtrHoldingbase=Reg_Val;
		/*初始化modbus*/
		Rx_MSG=MSG_COM;
		if(Rx_MSG == MSG_COM)
		{
			//收到非本机地址的响应请求
			if((Rx_Buf[0]!=MB_SLAVEADDR)&&(Rx_Buf[0]!=MB_ALLSLAVEADDR))
			{
				Rx_MSG = MSG_IDLE;
				HAL_GPIO_WritePin(D2_GPIO_Port,D2_Pin,GPIO_PIN_SET);//关485灯
				memset((uint8_t *)Rx_Buf,0,RxCount);//清除缓冲区
				printf("NetAddr is Not!");
				continue;
			}
			//解析数据帧
			MB_Parse_Data();
			//CRC校验
			crc_check=((Rx_Buf[RxCount-1]<<8) | Rx_Buf[RxCount-2]);
			if(crc_check==PduData._CRC)
			{
				//分析数据并执行
				Ex_code = MB_Analyze_Execute();
				/* 出现异常 */
				if(Ex_code !=EX_CODE_NONE)//异常回复主机
				{
				  MB_Exception_RSP(PduData.Code,Ex_code);
				}
				else//正常回复主机
				{
					MB_RSP(PduData.Code);
				}
			}
			else
			{
				printf("crc错误%x != %x\r\n",PduData._CRC,crc_check);
			}
			/* 重新标记为空闲状态 */
			
			Rx_MSG = MSG_IDLE;
		}
		//清空接收内存	
		memcpy((uint8_t *)data.Buffer,Rx_Buf,u8Index);
		data.count=u8Index;
		memset(Rx_Buf,0,u8Index);
#endif
		memcpy((uint8_t *)data.Buffer,aRxBuffer,u8Index);
		data.count=u8Index;
		//数据入队等待无线发送
		xQueueSendToFront(LoRaQueueHandle,&data,NULL);
		HAL_GPIO_WritePin(D2_GPIO_Port,D2_Pin,GPIO_PIN_SET);//关485灯
		
	}
    //osDelay(10);
  }
  /* USER CODE END Usart2_Task */
}

/* USER CODE BEGIN Header_LoRa_Process */
/**
* @brief Function implementing the LoRaProcess thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_LoRa_Process */
void LoRa_Process(void const * argument)
{
  /* USER CODE BEGIN LoRa_Process */
	STMFLASH_Read(FLASH_SAVE_ADDR,LoRaSysVal,LoRaVal_Size);//读出系统配置参数
	user_info("RF_CH=%d|RF_POWER=%d|RF_BW=%d|RF_SF=%d|RF_TXOUT=%d|RF_RXOUT=%d|RF_NETADDR=%d",LoRaSysVal[0],LoRaSysVal[1],LoRaSysVal[2],LoRaSysVal[3],LoRaSysVal[4],LoRaSysVal[5],LoRaSysVal[7]);
	RF_Init();
	uint8_t RF_State=0;
	tsBuf LoRaBuf;
	
	uint16_t crc_check=0;
	uint8_t Ex_code=0;
	__IO uint16_t Rx_MSG=MSG_IDLE;
  /* Infinite loop */
  for(;;)
  {
	  RF_State=Radio->Process();
	  switch(RF_State)
	  {
		  case RF_TX_TIMEOUT:/*发送超时*/
			  user_info("RF_TX_TIMEOUT!");
			  break;
		  case RF_TX_DONE:/*发送完成*/
			  user_info("RF_TX_DONE!"); 
			  HAL_GPIO_WritePin(D3_GPIO_Port,D3_Pin,GPIO_PIN_RESET);//关发送指示灯
			  HAL_GPIO_WritePin(VC1_GPIO_Port,VC1_Pin,GPIO_PIN_SET);//关闭发送射频开关并打开接收射频开关
			  Radio->StartRx();
			  break;
		  case RF_RX_DONE:/*接收完成*/
//			  user_info("收到数据！");
			  LoRaSysVal[9]=0;//重发次数清0
			  Radio->GetRxPacket((uint8_t *)LoRaBuf.Buffer,&LoRaBuf.count);						// 从LoRa中读取接收数据及数据长度
			  LoRaBuf.Buffer[LoRaBuf.count] = '\0';
		      printf("Receive data | ");									// 打印接收到的字符串
			  for(int i=0;i<LoRaBuf.count;i++)
			  {
				  printf("%02X ",LoRaBuf.Buffer[i]);
			  }
			  
			  //printf("Snr = %d, Rssi = %f.\r\n", SX1276LoRaGetPacketSnr(), SX1276LoRaGetPacketRssi());	// Snr是信噪比，Rssi是信号强度
			  printf(" | Snr = %d, Rssi = %d\r\n", SX1276LoRaGetPacketSnr(), (uint8_t)SX1276LoRaGetPacketRssi());	// Snr是信噪比，Rssi是信号强度			 
			  printf("\r\n");
			  USART2_TX((uint8_t *)LoRaBuf.Buffer,LoRaBuf.count);

			  
			  HAL_GPIO_WritePin(VC1_GPIO_Port,VC1_Pin,GPIO_PIN_SET);//关闭发送射频开关并打开接收射频开关
			  SX1276SetRFState(RFLR_STATE_CAD_INIT);
			  vTaskResume(LoRaTaskHandle);//数据处理完成，恢复任务
			  break;
		  case RF_RX_TIMEOUT:/*接收超时*/
			  user_info("RF_RX_TIMEOUT!");
			  memset((uint8_t *)LoRa_Txdata.Buffer,0,LoRa_Txdata.count);//清空LoRa发送缓存
			  HAL_GPIO_WritePin(VC1_GPIO_Port,VC1_Pin,GPIO_PIN_SET);//关闭发送射频开关并打开接收射频开关
			  SX1276SetRFState(RFLR_STATE_CAD_INIT);
			  vTaskResume(LoRaTaskHandle);//恢复任务    	  
//			  if(LoRaSysVal[9]>0)//重发数据
//			  {
//				  user_info("Resend data!");
//				  LoRa_SendData((uint8_t *)LoRa_Txdata.Buffer,LoRa_Txdata.count);		//无线发送数据
//				  LoRaSysVal[9]--;//重发次数减1
//			  }
//			  else//重发次数已用完
//			  {
//				  memset((uint8_t *)LoRa_Txdata.Buffer,0,LoRa_Txdata.count);//清空LoRa发送缓存
//				  HAL_GPIO_WritePin(VC1_GPIO_Port,VC1_Pin,GPIO_PIN_SET);//关闭发送射频开关并打开接收射频开关
//				  SX1276SetRFState(RFLR_STATE_CAD_INIT);
//				  vTaskResume(LoRaTaskHandle);//恢复任务
//			  }
			  
			  break;
		  case RF_CHANNEL_EMPTY:
//			  user_info("RF_CHANNEL_EMPTY!");
			  HAL_GPIO_WritePin(VC1_GPIO_Port,VC1_Pin,GPIO_PIN_SET);
			  SX1276SetRFState(RFLR_STATE_CAD_INIT);
			  break;
		  case RF_CHANNEL_ACTIVITY_DETECTED:
			  user_info("无线网络唤醒!");
			  break;
	  }
	  //osDelay(10);
  }
  /* USER CODE END LoRa_Process */
}

/* USER CODE BEGIN Header_LoRa_Task */
/**
* @brief Function implementing the LoRaTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_LoRa_Task */
void LoRa_Task(void const * argument)
{
  /* USER CODE BEGIN LoRa_Task */
	
	uint8_t count=0;
  /* Infinite loop */
  for(;;)
  {
	if(xQueueReceive(LoRaQueueHandle,&LoRa_Txdata,portMAX_DELAY)==pdTRUE)
	{
		count=osMessageWaiting(LoRaQueueHandle);				//获取队列缓存个数
		user_info("Queue_count=%d",count);						//打印剩余队列数量
		user_info("count=%d|data=%s",LoRa_Txdata.count,LoRa_Txdata.Buffer);	//打印队列信息
		
		LoRa_SendData((uint8_t *)LoRa_Txdata.Buffer,LoRa_Txdata.count);		//无线发送数据
		LoRaSysVal[9]=2;										//设定重发次数
		vTaskSuspend(LoRaTaskHandle);							//挂起本任务等待接收
	}
    //osDelay(10);
  }
  /* USER CODE END LoRa_Task */
}

/* USER CODE BEGIN Header_Usart1_Task */
/**
* @brief Function implementing the Usart1Task thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_Usart1_Task */
void Usart1_Task(void const * argument)
{
  /* USER CODE BEGIN Usart1_Task */
	uint8_t u8Index=0;
	uint8_t u8Usart1Buffer[20]={0};
  /* Infinite loop */
  for(;;)
    {
	u8Index=0;
	if(xQueueReceive(Usart1QueueHandle,&u8Usart1Buffer[u8Index++],portMAX_DELAY)== pdTRUE)
	{	
		while(xQueueReceive(Usart1QueueHandle,&u8Usart1Buffer[u8Index++],5)){}	
		//接收到一帧数据
		u8Usart1Buffer[u8Index]='\0';
		u8Index=u8Index-1;
		user_info("Receive setup system command | %s",u8Usart1Buffer);	
		//协议处理
		vParseString((char*)u8Usart1Buffer);
		//清空内存
		memset(u8Usart1Buffer,0,u8Index);			
		
	}
    osDelay(10);
  }
  /* USER CODE END Usart1_Task */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */
     
/* USER CODE END Application */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
