#include "JQ8400.h"

void JQ8x00_Init(void)
{
}

void JQ8x00_ZuHeBoFang(uint8_t *DATA, uint8_t Len)
{
  uint8_t CRC_data = 0U;
  uint8_t i = 3U;
  uint8_t Buffer[ZH_MAX] = {0xAA, 0x1B};

  Buffer[2] = (uint8_t)(Len * 2U);
  CRC_data = (uint8_t)(CRC_data + 0xAAU + 0x1BU + Buffer[2]);

  while (Len--)
  {
    Buffer[i] = (uint8_t)(*DATA / 10U + 0x30U);
    CRC_data = (uint8_t)(CRC_data + Buffer[i]);
    i++;
    Buffer[i] = (uint8_t)(*DATA % 10U + 0x30U);
    CRC_data = (uint8_t)(CRC_data + Buffer[i]);
    i++;
    DATA++;
  }

  Buffer[i] = CRC_data;
  i++;
  JQ8x00_UART(Buffer, i);
}

void JQ8x00_Command(UartCommand Command)
{
  uint8_t Buffer[4] = {0xAA};

  Buffer[1] = (uint8_t)Command;
  Buffer[2] = 0x00U;
  Buffer[3] = (uint8_t)(Buffer[0] + Buffer[1] + Buffer[2]);
  JQ8x00_UART(Buffer, 4U);
}

void JQ8x00_Command_Data(UartCommandData Command, uint8_t DATA)
{
  uint8_t Buffer[6] = {0xAA};
  uint8_t DataLen = 0U;

  Buffer[1] = (uint8_t)Command;

  if ((Command != AppointTrack) && (Command != SetCycleCount) && (Command != SelectTrackNoPlay)
      && (Command != AppointTimeBack) && (Command != AppointTimeFast))
  {
    Buffer[2] = 1U;
    Buffer[3] = DATA;
    Buffer[4] = (uint8_t)(Buffer[0] + Buffer[1] + Buffer[2] + Buffer[3]);
    DataLen = 5U;
  }
  else
  {
    Buffer[2] = 2U;
    Buffer[3] = (uint8_t)(DATA / 256U);
    Buffer[4] = (uint8_t)(DATA % 256U);
    Buffer[5] = (uint8_t)(Buffer[0] + Buffer[1] + Buffer[2] + Buffer[3] + Buffer[4]);
    DataLen = 6U;
  }

  JQ8x00_UART(Buffer, DataLen);
}

void JQ8x00_RandomPathPlay(JQ8X00_Symbol symbol, char *DATA)
{
  uint8_t Buffer[52] = {0xAA, 0x08};
  uint8_t i;
  uint8_t j;

  Buffer[2] = (uint8_t)(1U + strlen(DATA) + 4U);
  Buffer[3] = (uint8_t)symbol;
  i = 4U;

  while (*DATA)
  {
    Buffer[i] = (uint8_t)(*DATA);
    i++;
    DATA++;
  }

  Buffer[i] = (uint8_t)'*';
  i++;
  Buffer[i] = (uint8_t)'?';
  i++;
  Buffer[i] = (uint8_t)'?';
  i++;
  Buffer[i] = (uint8_t)'?';
  i++;

  Buffer[i] = 0U;
  for (j = 0U; j < i; j++)
  {
    Buffer[i] = (uint8_t)(Buffer[i] + Buffer[j]);
  }
  i++;

  JQ8x00_UART(Buffer, i);
}
