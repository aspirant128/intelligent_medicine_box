/**
  * @file    servo.h
  * @brief   三路 SG90 PWM（TIM2 CH1/2/3 → PA0/PA1/PA2，50Hz）
  */
#ifndef __SERVO_H__
#define __SERVO_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "main.h"

#define SERVO_ID_1  1U
#define SERVO_ID_2  2U
#define SERVO_ID_3  3U

#define SERVO_PWM_PERIOD_TICKS  2000U

void Servo_Init(void);
void Servo_SetAngle(uint8_t servo_id, uint8_t angle);

#ifdef __cplusplus
}
#endif

#endif /* __SERVO_H__ */
