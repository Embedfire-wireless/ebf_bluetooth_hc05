/**
  ******************************************************************************
  * @file    Project/STM32F4xx_StdPeriph_Templates/stm32f4xx_it.c 
  * @author  MCD Application Team
  * @version V1.5.0
  * @date    06-March-2015
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and 
  *          peripherals interrupt service routine.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2015 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_it.h" 
#include "./systick/bsp_SysTick.h" 
#include "./usart/bsp_blt_usart.h"
#include "./usart/bsp_debug_usart.h"


#define TASK_DELAY_NUM  2       //总任务个数，可以自己根据实际情况修改
#define TASK_DELAY_0    500    //任务0延时 500*10 毫秒后执行：检查蓝牙是否已连接
#define TASK_DELAY_1    50     //任务1延时 50*10 毫秒后执行：

uint32_t Task_Delay_Group[TASK_DELAY_NUM];  //任务数组，用来计时、并判断是否执行对应任务

/* 每5s检查蓝牙是否连接 */
int hc05_inquery_connect;



/** @addtogroup STM32F429I_DISCOVERY_Examples
  * @{
  */

/** @addtogroup FMC_SDRAM
  * @{
  */ 

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M4 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief  This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {}
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {}
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {}
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {}
}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{}

/**
  * @brief  This function handles PendSV_Handler exception.
  * @param  None
  * @retval None
  */
void PendSV_Handler(void)
{}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
void SysTick_Handler(void)
{
  int i;
  
  for(i=0; i<TASK_DELAY_NUM; i++)
  {
    Task_Delay_Group[i] ++;                   //任务计时，时间到后执行
  }
  
  
  /* 处理任务0 */
  if(Task_Delay_Group[0] >= TASK_DELAY_0)     //判断是否执行任务0
  {
    Task_Delay_Group[0] = 0;                  //置0重新计时
    
    /* 任务0：检查蓝牙是否已连接 */
    
    hc05_inquery_connect = 1; //如果蓝牙没有连接，标志位置1
    
  }
  
  /* 处理任务1 */
  if(Task_Delay_Group[1] >= TASK_DELAY_1)     //判断是否执行任务1
  {
    Task_Delay_Group[1] = 0;                  //置0重新计时
    
    
    /* 任务1：xxxxx */
    //printf("Test\r\n");
  }
}

/******************************************************************************/
/*                 STM32F4xx Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f429_439xx.s).                         */
/******************************************************************************/


ReceiveData DEBUG_USART_ReceiveData;
// 串口中断服务函数
void DEBUG_USART_IRQHandler(void)
{
		uint8_t ucCh; 
    if(USART_GetITStatus(DEBUG_USART, USART_IT_RXNE) != RESET)
    {
      ucCh = USART_ReceiveData(DEBUG_USART);
      if(DEBUG_USART_ReceiveData.datanum < UART_BUFF_SIZE)
        {
          if((ucCh != 0x0a) && (ucCh != 0x0d))
          {
            DEBUG_USART_ReceiveData.uart_buff[DEBUG_USART_ReceiveData.datanum] = ucCh;                 //不接收换行回车
            DEBUG_USART_ReceiveData.datanum++;
          }

        }         
     }
		if(USART_GetITStatus( DEBUG_USART, USART_IT_IDLE ) == SET )                                         //数据帧接收完毕
    {
        DEBUG_USART_ReceiveData.receive_data_flag = 1;
        USART_ReceiveData( DEBUG_USART );                                                              //由软件序列清除中断标志位(先读USART_SR，然后读USART_DR)	
    }	
} 

//蓝牙串口中断缓存串口数据
ReceiveData BLT_USART_ReceiveData;

void BLT_USART_IRQHandler(void)
{
    uint8_t ucCh; 
    if(USART_GetITStatus(BLT_USARTx, USART_IT_RXNE) != RESET)
    {
      ucCh = USART_ReceiveData(BLT_USARTx);
      if(BLT_USART_ReceiveData.datanum < UART_BUFF_SIZE)
        {
          if((ucCh != 0x0a) && (ucCh != 0x0d))
          {
            BLT_USART_ReceiveData.uart_buff[BLT_USART_ReceiveData.datanum] = ucCh;                 //不接收换行回车
            BLT_USART_ReceiveData.datanum++;
          }
        }         
     }
		if(USART_GetITStatus( BLT_USARTx, USART_IT_IDLE ) == SET )                                         //数据帧接收完毕
    {
        BLT_USART_ReceiveData.receive_data_flag = 1;
        USART_ReceiveData( BLT_USARTx );                                                              //由软件序列清除中断标志位(先读USART_SR，然后读USART_DR)	
    }	

}

/**
  * @}
  */ 

/**
  * @}
  */ 

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
