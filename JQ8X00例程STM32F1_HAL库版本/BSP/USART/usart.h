#ifndef __USART_H__
#define __USART_H__

/* 包含头文件 ----------------------------------------------------------------*/
#include "stm32f1xx_hal.h"

/* 类型定义 ------------------------------------------------------------------*/
/* 宏定义 --------------------------------------------------------------------*/
#define UARTx                                 USART1
#define UARTx_BAUDRATE                        9600
#define UART_RCC_CLK_ENABLE()                 __HAL_RCC_USART1_CLK_ENABLE()
#define UART_RCC_CLK_DISABLE()                __HAL_RCC_USART1_CLK_DISABLE()

#define UARTx_GPIO_ClK_ENABLE()               __HAL_RCC_GPIOA_CLK_ENABLE()
#define UARTx_Tx_GPIO_PIN                     GPIO_PIN_9
#define UARTx_Tx_GPIO                         GPIOA
#define UARTx_Rx_GPIO_PIN                     GPIO_PIN_10
#define UARTx_Rx_GPIO                         GPIOA

#define UARTx_IRQHANDLER                      UART1_IRQHandler
#define UARTx_IRQn                            USART1_IRQn


/* 扩展变量 ------------------------------------------------------------------*/
extern UART_HandleTypeDef huartx;

/* 函数声明 ------------------------------------------------------------------*/
void MX_UARTx_Init(void);


#endif

