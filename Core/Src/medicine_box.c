/**
  * @file    medicine_box.c
  * @brief   三路闹钟取药：RING1/2/3 对应舵机1/2/3 与按键1/2/3
  */
#include "medicine_box.h"
#include "servo.h"
#include "key.h"
#include "esp8266_uart.h"
#include "OLED.h"
#include "oled_status.h"
#include "jq8400_voice.h"

typedef enum
{
  MB_STATE_IDLE = 0,
  MB_STATE_WAIT_TAKE
} MedicineBoxState_t;

typedef enum
{
  MB_MSG_TAKE = 0,
  MB_MSG_TAKEN,
  MB_MSG_REMIND
} MedicineBoxMsg_t;

static const char s_msg_take[] = {
  0xE8, 0xAF, 0xB7, 0xE5, 0x8F, 0x96, 0xE8, 0xB5, 0xB0, 0xE8, 0x8D, 0xAF,
  0xE7, 0x89, 0xA9, 0x00
};

static const char s_msg_taken[] = {
  0xE8, 0x8D, 0xAF, 0xE7, 0x89, 0xA9, 0xE5, 0xB7, 0xB2, 0xE5, 0x8F, 0x96,
  0xE8, 0xB5, 0xB0, 0x00
};

static const char s_msg_remind[] = {
  0xE8, 0xAF, 0xB7, 0xE5, 0x8F, 0x8A, 0xE6, 0x97, 0xB6, 0xE5, 0x8F, 0x96,
  0xE8, 0xB5, 0xB0, 0xE8, 0x8D, 0xAF, 0xE7, 0x89, 0xA9, 0x00
};

static MedicineBoxState_t s_state;
static uint8_t s_active_slot;
static uint32_t s_alarm_start_tick;
static uint8_t s_reminder_sent;

static const char *MedicineBox_GetMsgText(MedicineBoxMsg_t msg)
{
  if (msg == MB_MSG_TAKEN)
  {
    return s_msg_taken;
  }
  if (msg == MB_MSG_REMIND)
  {
    return s_msg_remind;
  }
  return s_msg_take;
}

static void MedicineBox_ShowMessage(MedicineBoxMsg_t msg)
{
  OLED_ClearArea(0, 0, 128, (uint8_t)OLED_STATUS_Y_TIME);
  OLED_ShowChinese(0, 0, (char *)MedicineBox_GetMsgText(msg));
  OLED_UpdateArea(0, 0, 128, (uint8_t)OLED_STATUS_Y_TIME);
  OLED_Status_Refresh();
}

void MedicineBox_Init(void)
{
  s_state = MB_STATE_IDLE;
  s_active_slot = 0U;
  s_alarm_start_tick = 0U;
  s_reminder_sent = 0U;
}

void MedicineBox_Process(void)
{
  uint8_t slot;

  if (s_state == MB_STATE_IDLE)
  {
    if (ESP8266_UART_IsAlarmEventPending() != 0U)
    {
      slot = ESP8266_UART_GetAlarmSlot();
      ESP8266_UART_ClearAlarmEvent();

      if ((slot < SERVO_ID_1) || (slot > SERVO_ID_3))
      {
        slot = SERVO_ID_1;
      }

      s_active_slot = slot;
      Servo_SetAngle(slot, 90U);
      MedicineBox_ShowMessage(MB_MSG_TAKE);
      JQ8400_Voice_PlayAlarm();
      s_alarm_start_tick = HAL_GetTick();
      s_reminder_sent = 0U;
      s_state = MB_STATE_WAIT_TAKE;
    }
    return;
  }

  if (Key_IsPressed(s_active_slot) != 0U)
  {
    Servo_SetAngle(s_active_slot, 0U);
    MedicineBox_ShowMessage(MB_MSG_TAKEN);
    JQ8400_Voice_PlayTaken();
    s_state = MB_STATE_IDLE;
    s_active_slot = 0U;
    return;
  }

  if ((s_reminder_sent == 0U)
      && ((HAL_GetTick() - s_alarm_start_tick) >= MEDICINE_TAKE_TIMEOUT_MS))
  {
    ESP8266_UART_SendTakeMedReminder();
    MedicineBox_ShowMessage(MB_MSG_REMIND);
    JQ8400_Voice_PlayRemind();
    s_reminder_sent = 1U;
  }
}
