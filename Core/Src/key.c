/**
  * @file    key.c
  * @brief   按键1=PC2, 按键2=PC3, 按键3=PC4
  */
#include "key.h"

#define KEY_COUNT       3U
#define KEY_DEBOUNCE_MS 30U

typedef struct
{
  GPIO_TypeDef *port;
  uint16_t pin;
  uint8_t stable;
  uint8_t last_sample;
  uint32_t change_tick;
} KeyCtx_t;

static KeyCtx_t s_keys[KEY_COUNT] = {
  { USER_KEY1_GPIO_Port,  USER_KEY1_Pin,  0U, 0U, 0U },
  { USER_KEY2_GPIO_Port,  USER_KEY2_Pin,  0U, 0U, 0U },
  { USER_KEY3_GPIO_Port,  USER_KEY3_Pin,  0U, 0U, 0U }
};

static uint8_t Key_ReadRaw(uint8_t index)
{
  if (HAL_GPIO_ReadPin(s_keys[index].port, s_keys[index].pin) == GPIO_PIN_RESET)
  {
    return 1U;
  }
  return 0U;
}

void Key_Init(void)
{
  GPIO_InitTypeDef gpio = {0};
  uint8_t i;

  __HAL_RCC_GPIOC_CLK_ENABLE();

  gpio.Mode = GPIO_MODE_INPUT;
  gpio.Pull = GPIO_PULLUP;
  gpio.Speed = GPIO_SPEED_FREQ_LOW;

  for (i = 0U; i < KEY_COUNT; i++)
  {
    gpio.Pin = s_keys[i].pin;
    HAL_GPIO_Init(s_keys[i].port, &gpio);
    s_keys[i].last_sample = Key_ReadRaw(i);
    s_keys[i].stable = 0U;
    s_keys[i].change_tick = HAL_GetTick();
  }
}

uint8_t Key_IsPressed(uint8_t key_id)
{
  uint8_t index;
  uint8_t sample;
  uint32_t now;

  if ((key_id < KEY_ID_1) || (key_id > KEY_ID_3))
  {
    return 0U;
  }

  index = (uint8_t)(key_id - 1U);
  sample = Key_ReadRaw(index);
  now = HAL_GetTick();

  if (sample != s_keys[index].last_sample)
  {
    s_keys[index].last_sample = sample;
    s_keys[index].change_tick = now;
  }
  else if ((now - s_keys[index].change_tick) >= KEY_DEBOUNCE_MS)
  {
    s_keys[index].stable = sample;
  }

  return s_keys[index].stable;
}
