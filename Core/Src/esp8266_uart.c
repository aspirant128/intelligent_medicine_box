/**

  * @file    esp8266_uart.c

  * @brief   USART1 接收：解析 TIME/IP/EVENT:RINGx

  */

#include "esp8266_uart.h"

#include "usart.h"



#define ESP_RX_LINE_MAX       128U

#define ESP_EVENT_MARKER      "EVENT:RING"

#define ESP_TIME_MARKER       "TIME:"

#define ESP_IP_MARKER         "IP:"

#define ESP_TIME_HHMM_LEN     5U

#define ESP_IP_MAX_LEN        39U



static uint8_t s_rx_byte;

static char s_rx_line[ESP_RX_LINE_MAX];

static uint16_t s_rx_index;

static volatile uint8_t s_alarm_pending;

static uint8_t s_alarm_slot;

static char s_time_hhmm[ESP_TIME_HHMM_LEN + 1U];

static char s_ip[ESP_IP_MAX_LEN + 1U];

static volatile uint8_t s_status_updated;



static uint8_t Str_Contains(const char *haystack, const char *needle)

{

  uint16_t i;

  uint16_t j;

  uint16_t nlen = 0U;



  if ((haystack == NULL) || (needle == NULL))

  {

    return 0U;

  }



  while (needle[nlen] != '\0')

  {

    nlen++;

  }

  if (nlen == 0U)

  {

    return 0U;

  }



  for (i = 0U; haystack[i] != '\0'; i++)

  {

    for (j = 0U; j < nlen; j++)

    {

      if (haystack[i + j] != needle[j])

      {

        break;

      }

    }

    if (j == nlen)

    {

      return 1U;

    }

  }



  return 0U;

}



static const char *Str_Find(const char *haystack, const char *needle)

{

  uint16_t i;

  uint16_t j;

  uint16_t nlen = 0U;



  if ((haystack == NULL) || (needle == NULL))

  {

    return NULL;

  }



  while (needle[nlen] != '\0')

  {

    nlen++;

  }

  if (nlen == 0U)

  {

    return haystack;

  }



  for (i = 0U; haystack[i] != '\0'; i++)

  {

    for (j = 0U; j < nlen; j++)

    {

      if (haystack[i + j] != needle[j])

      {

        break;

      }

    }

    if (j == nlen)

    {

      return &haystack[i];

    }

  }



  return NULL;

}



static void Str_Copy(char *dst, const char *src, uint16_t max_len)

{

  uint16_t i = 0U;



  if ((dst == NULL) || (src == NULL) || (max_len == 0U))

  {

    return;

  }



  while ((src[i] != '\0') && (i < (max_len - 1U)))

  {

    dst[i] = src[i];

    i++;

  }

  dst[i] = '\0';

}



static void ESP8266_UART_ParseTime(const char *line)

{

  const char *p = Str_Find(line, ESP_TIME_MARKER);

  uint16_t i;



  if (p == NULL)

  {

    return;

  }



  p += 5U;

  if ((p[0] >= '0') && (p[0] <= '9')

      && (p[1] >= '0') && (p[1] <= '9')

      && (p[2] == ':')

      && (p[3] >= '0') && (p[3] <= '9')

      && (p[4] >= '0') && (p[4] <= '9'))

  {

    for (i = 0U; i < ESP_TIME_HHMM_LEN; i++)

    {

      s_time_hhmm[i] = p[i];

    }

    s_time_hhmm[ESP_TIME_HHMM_LEN] = '\0';

    s_status_updated = 1U;

  }

}



static void ESP8266_UART_ParseIP(const char *line)

{

  const char *p = Str_Find(line, ESP_IP_MARKER);

  uint16_t i = 0U;



  if (p == NULL)

  {

    return;

  }



  p += 3U;

  while ((p[i] != '\0') && (p[i] != ',') && (i < ESP_IP_MAX_LEN))

  {

    s_ip[i] = p[i];

    i++;

  }

  s_ip[i] = '\0';



  if (i > 0U)

  {

    s_status_updated = 1U;

  }

}



static void ESP8266_UART_ResetLineBuffer(void)

{

  s_rx_index = 0U;

  s_rx_line[0] = '\0';

}



static void ESP8266_UART_ParseLine(void)

{

  uint16_t i;



  ESP8266_UART_ParseTime(s_rx_line);

  ESP8266_UART_ParseIP(s_rx_line);



  if (Str_Contains(s_rx_line, ESP_EVENT_MARKER) != 0U)

  {

    s_alarm_pending = 1U;

    s_alarm_slot = 0U;



    for (i = 0U; s_rx_line[i] != '\0'; i++)

    {

      if ((s_rx_line[i] == 'R')

          && (s_rx_line[i + 1U] == 'I')

          && (s_rx_line[i + 2U] == 'N')

          && (s_rx_line[i + 3U] == 'G'))

      {

        if ((s_rx_line[i + 4U] >= '1') && (s_rx_line[i + 4U] <= '3'))

        {

          s_alarm_slot = (uint8_t)(s_rx_line[i + 4U] - '0');

        }

        break;

      }

    }

  }

}



static void ESP8266_UART_AppendChar(char ch)

{

  if (s_rx_index >= (ESP_RX_LINE_MAX - 1U))

  {

    ESP8266_UART_ResetLineBuffer();

    return;

  }



  s_rx_line[s_rx_index] = ch;

  s_rx_index++;

  s_rx_line[s_rx_index] = '\0';

}



static void ESP8266_UART_OnByteReceived(uint8_t byte)

{

  char ch = (char)byte;



  if ((ch == '\n') || (ch == '\r'))

  {

    if (s_rx_index > 0U)

    {

      ESP8266_UART_ParseLine();

      ESP8266_UART_ResetLineBuffer();

    }

    return;

  }



  ESP8266_UART_AppendChar(ch);

}



void ESP8266_UART_StartReceive(void)

{

  ESP8266_UART_ResetLineBuffer();

  s_alarm_pending = 0U;

  s_alarm_slot = 0U;

  s_status_updated = 0U;

  Str_Copy(s_time_hhmm, "--:--", sizeof(s_time_hhmm));

  Str_Copy(s_ip, "---", sizeof(s_ip));



  if (HAL_UART_Receive_IT(&huart1, &s_rx_byte, 1U) != HAL_OK)

  {

    Error_Handler();

  }

}



uint8_t ESP8266_UART_IsAlarmEventPending(void)

{

  return s_alarm_pending;

}



uint8_t ESP8266_UART_GetAlarmSlot(void)

{

  return s_alarm_slot;

}



void ESP8266_UART_ClearAlarmEvent(void)

{

  s_alarm_pending = 0U;

}



uint8_t ESP8266_UART_IsStatusUpdated(void)

{

  return s_status_updated;

}



void ESP8266_UART_ClearStatusUpdated(void)

{

  s_status_updated = 0U;

}



void ESP8266_UART_GetTimeHHMM(char *buf, uint8_t buf_len)

{

  if ((buf == NULL) || (buf_len == 0U))

  {

    return;

  }

  Str_Copy(buf, s_time_hhmm, buf_len);

}



void ESP8266_UART_GetIP(char *buf, uint8_t buf_len)

{

  char line[48];

  uint16_t i = 0U;



  if ((buf == NULL) || (buf_len == 0U))

  {

    return;

  }



  line[i++] = 'I';

  line[i++] = 'P';

  line[i++] = ':';

  Str_Copy(&line[i], s_ip, (uint16_t)(sizeof(line) - i));

  Str_Copy(buf, line, buf_len);

}



void ESP8266_UART_SendTakeMedReminder(void)

{

  const char head[] = "STM32:CMD:REMIND_TAKE_MED,SLOT:";

  char tx_buf[48];

  uint16_t idx = 0U;

  uint16_t j;



  for (j = 0U; head[j] != '\0'; j++)

  {

    tx_buf[idx] = head[j];

    idx++;

  }



  if ((s_alarm_slot >= 1U) && (s_alarm_slot <= 3U))

  {

    tx_buf[idx] = (char)('0' + s_alarm_slot);

  }

  else

  {

    tx_buf[idx] = '0';

  }

  idx++;



  tx_buf[idx] = '\r';

  idx++;

  tx_buf[idx] = '\n';

  idx++;



  (void)HAL_UART_Transmit(&huart1, (uint8_t *)tx_buf, idx, 100U);

}



void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)

{

  if (huart->Instance == USART1)

  {

    ESP8266_UART_OnByteReceived(s_rx_byte);



    if (HAL_UART_Receive_IT(&huart1, &s_rx_byte, 1U) != HAL_OK)

    {

      Error_Handler();

    }

  }

}



void HAL_UART_ErrorCallback(UART_HandleTypeDef *huart)

{

  if (huart->Instance == USART1)

  {

    __HAL_UART_CLEAR_OREFLAG(huart);

    __HAL_UART_CLEAR_NEFLAG(huart);

    __HAL_UART_CLEAR_FEFLAG(huart);

    HAL_UART_Receive_IT(&huart1, &s_rx_byte, 1U);

  }

}


