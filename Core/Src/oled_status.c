/**
  * @file    oled_status.c
  * @brief   显示屏倒数两行：时间(到分) + IP
  */
#include "oled_status.h"
#include "OLED.h"
#include "esp8266_uart.h"

static void OLED_Status_ClearBottom(void)
{
  OLED_ClearArea(0, (int16_t)OLED_STATUS_Y_TIME, 128, OLED_STATUS_HEIGHT);
}

void OLED_Status_InitDefaults(void)
{
  OLED_Status_Refresh();
}

void OLED_Status_Refresh(void)
{
  char line[40];

  OLED_Status_ClearBottom();

  ESP8266_UART_GetTimeHHMM(line, sizeof(line));
  OLED_ShowString(0, (int16_t)OLED_STATUS_Y_TIME, line, OLED_8X16);

  ESP8266_UART_GetIP(line, sizeof(line));
  OLED_ShowString(0, (int16_t)OLED_STATUS_Y_IP, line, OLED_6X8);

  OLED_UpdateArea(0, (int16_t)OLED_STATUS_Y_TIME, 128, OLED_STATUS_HEIGHT);
}
