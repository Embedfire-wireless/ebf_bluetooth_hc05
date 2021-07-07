/**
  ******************************************************************************
  * @file    Project/STM32F10x_StdPeriph_Template/stm32f10x_it.c 
  * @author  MCD Application Team
  * @version V3.5.0
  * @date    08-April-2011
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and 
  *          peripherals interrupt service routine.
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTI
  
  AL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x_it.h"
#include "./systick/bsp_SysTick.h" 
#include "./usart/bsp_usart_blt.h"
#include "./usart/bsp_usart.h"


#define TASK_DELAY_NUM  2       //总任务个数，可以自己根据实际情况修改
#define TASK_DELAY_0    200    //任务0延时 200*10 毫秒后执行：读取 DHT11 传感器数据
#define TASK_DELAY_1    50     //任务1延时 50*10 毫秒后执行：

uint32_t Task_Delay_Group[TASK_DELAY_NUM];  //任务数组，用来计时、并判断是否执行对应任务

/* 读传感器数据完成标志 */
// - 标志置 1表示完成读取，在主循环处理数据
// - 标志置 0表示未完成读取
// - 标志置-1表示读取错误
int hc05_need_inquery;



/** @addtogroup STM32F10x_StdPeriph_Template
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M3 Processor Exceptions Handlers                         */
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
  {
  }
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
  {
  }
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
  {
  }
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
  {
  }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{
}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{
}

/**
  * @brief  This function handles PendSVC exception.
  * @param  None
  * @retval None
  */
void PendSV_Handler(void)
{
}

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
    
    /* 任务0：读取 DHT11 传感器数据 */
    
  }
  
  /* 处理任务1 */
  if(Task_Delay_Group[1] >= TASK_DELAY_1)     //判断是否执行任务1
  {
    Task_Delay_Group[1] = 0;                  //置0重新计时
    
    
    /* 任务1：xxxxx */
    //printf("Test\r\n");
  }
}


/**
  * @brief  This function handles PPP interrupt request.
  * @param  None
  * @retval None
  */
/*void PPP_IRQHandler(void)
{
}*/

ReceiveData DEBUG_USART_ReceiveData;
// 串口中断服务函数
void DEBUG_USART_IRQHandler(void)
{
		uint8_t ucCh; 
    if(USART_GetITStatus(DEBUG_USARTx, USART_IT_RXNE) != RESET)
      {
				ucCh = USART_ReceiveData(DEBUG_USARTx);
				if(DEBUG_USART_ReceiveData.datanum < UART_BUFF_SIZE)
					{
						if((ucCh != 0x0a) && (ucCh != 0x0d))
						{
							DEBUG_USART_ReceiveData.uart_buff[DEBUG_USART_ReceiveData.datanum] = ucCh;                 //不接收换行回车
              DEBUG_USART_ReceiveData.datanum++;
						}

					}         
       }
		if(USART_GetITStatus( DEBUG_USARTx, USART_IT_IDLE ) == SET )                                         //数据帧接收完毕
	    {
				  DEBUG_USART_ReceiveData.receive_data_flag = 1;
		      USART_ReceiveData( DEBUG_USARTx );                                                              //由软件序列清除中断标志位(先读USART_SR，然后读USART_DR)	
      }	
} 

void BLT_USART_IRQHandler(void)
{
    bsp_USART_Process();

}

/**
  * @}
  */ 


/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
