/**
  * @file    oled_status.h
  * @brief   OLED 底部两行：串口时间(HH:MM) + IP
  */
#ifndef __OLED_STATUS_H__
#define __OLED_STATUS_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "main.h"

#define OLED_STATUS_Y_TIME  32U
#define OLED_STATUS_Y_IP    48U
#define OLED_STATUS_HEIGHT  32U

void OLED_Status_InitDefaults(void);
void OLED_Status_Refresh(void);

#ifdef __cplusplus
}
#endif

#endif /* __OLED_STATUS_H__ */
