/**
  * @file    key.h
  * @brief   三路确认按键（PC2/PC3/PC4，按下为低电平）
  */
#ifndef __KEY_H__
#define __KEY_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "main.h"

#define KEY_ID_1  1U
#define KEY_ID_2  2U
#define KEY_ID_3  3U

void Key_Init(void);
uint8_t Key_IsPressed(uint8_t key_id);

#ifdef __cplusplus
}
#endif

#endif /* __KEY_H__ */
