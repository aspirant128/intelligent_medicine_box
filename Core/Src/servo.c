/**
  * @file    servo.c
  * @brief   三路 SG90：PA0=舵机1, PA1=舵机2, PA2=舵机3
  */
#include "servo.h"
#include "tim.h"

#define SERVO_PULSE_MIN_TICKS  50U
#define SERVO_PULSE_MAX_TICKS  250U

static uint16_t Servo_AngleToPulse(uint8_t angle)
{
  uint32_t pulse;

  if (angle > 180U)
  {
    angle = 180U;
  }

  pulse = (uint32_t)SERVO_PULSE_MIN_TICKS
        + ((uint32_t)angle * (SERVO_PULSE_MAX_TICKS - SERVO_PULSE_MIN_TICKS)) / 180U;

  return (uint16_t)pulse;
}

static void Servo_SetCompare(uint8_t servo_id, uint16_t pulse)
{
  switch (servo_id)
  {
    case SERVO_ID_1:
      __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, pulse);
      break;
    case SERVO_ID_2:
      __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_2, pulse);
      break;
    case SERVO_ID_3:
      __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_3, pulse);
      break;
    default:
      break;
  }
}

void Servo_Init(void)
{
  if (HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_2) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_3) != HAL_OK)
  {
    Error_Handler();
  }

  Servo_SetAngle(SERVO_ID_1, 0U);
  Servo_SetAngle(SERVO_ID_2, 0U);
  Servo_SetAngle(SERVO_ID_3, 0U);
}

void Servo_SetAngle(uint8_t servo_id, uint8_t angle)
{
  uint16_t pulse = Servo_AngleToPulse(angle);

  Servo_SetCompare(servo_id, pulse);
}
