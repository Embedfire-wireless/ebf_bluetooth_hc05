#ifndef __USART2_H
#define	__USART2_H


#include "stm32f4xx.h"
#include <stdio.h>


#define             BLT_USART_BAUD_RATE                       38400

#define             BLT_USARTx                                USART2
#define             BLT_USART_CLK                             RCC_APB1Periph_USART2
#define             BLT_USART_GPIO_CLK                       (RCC_AHB1Periph_GPIOD )     

#define             BLT_USART_TX_PORT                         GPIOD   
#define             BLT_USART_TX_PIN                          GPIO_Pin_5
#define 						 BLT_USART_TX_AF                       		GPIO_AF_USART2
#define             BLT_USART_TX_SOURCE                  		GPIO_PinSource5


#define             BLT_USART_RX_PORT                         GPIOD 
#define             BLT_USART_RX_PIN                          GPIO_Pin_6
#define 						 BLT_USART_RX_AF                       		GPIO_AF_USART2
#define             BLT_USART_RX_SOURCE                  		GPIO_PinSource6

#define             BLT_USART_IRQ                             USART2_IRQn
#define             BLT_USART_IRQHandler                     USART2_IRQHandler


void BLT_USART_Config(void);
void Usart_SendStr_length( USART_TypeDef * pUSARTx, uint8_t *str,uint32_t strlen );
void Usart_SendString( USART_TypeDef * pUSARTx, uint8_t *str);

void bsp_USART_Process(void);
char *get_rebuff(uint16_t *len);
void clean_rebuff(void);

#endif /* __USART2_H */
