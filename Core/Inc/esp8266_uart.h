/**

  * @file    esp8266_uart.h

  * @brief   ESP8266 串口协议：TIME/IP/EVENT:RING 解析

  */

#ifndef __ESP8266_UART_H__

#define __ESP8266_UART_H__



#ifdef __cplusplus

extern "C" {

#endif



#include "main.h"



void ESP8266_UART_StartReceive(void);

uint8_t ESP8266_UART_IsAlarmEventPending(void);

uint8_t ESP8266_UART_GetAlarmSlot(void);

void ESP8266_UART_ClearAlarmEvent(void);

uint8_t ESP8266_UART_IsStatusUpdated(void);

void ESP8266_UART_ClearStatusUpdated(void);

void ESP8266_UART_GetTimeHHMM(char *buf, uint8_t buf_len);

void ESP8266_UART_GetIP(char *buf, uint8_t buf_len);

void ESP8266_UART_SendTakeMedReminder(void);



#ifdef __cplusplus

}

#endif



#endif /* __ESP8266_UART_H__ */


