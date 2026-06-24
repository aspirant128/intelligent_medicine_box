/**
  * @file    medicine_box.h
  * @brief   闹钟取药流程：舵机、30s 按键确认、超时串口提醒
  */
#ifndef __MEDICINE_BOX_H__
#define __MEDICINE_BOX_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "main.h"

#define MEDICINE_TAKE_TIMEOUT_MS  30000U

void MedicineBox_Init(void);
void MedicineBox_Process(void);

#ifdef __cplusplus
}
#endif

#endif /* __MEDICINE_BOX_H__ */
