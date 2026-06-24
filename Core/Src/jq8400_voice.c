/**
  * @file    jq8400_voice.c
  * @brief   00001=闹钟触发 00002=超时提醒 00003=已取走
  */
#include "jq8400_voice.h"
#include "JQ8400.h"

#define JQ8400_DEFAULT_VOLUME  25U

static void JQ8400_PlayPath(char *path)
{
  JQ8x00_RandomPathPlay(JQ8X00_FLASH, path);
  JQ8x00_ms(10);
}

void JQ8400_Voice_Init(void)
{
  JQ8x00_Init();
  JQ8x00_ms(500);
  JQ8x00_Command_Data(SetVolume, JQ8400_DEFAULT_VOLUME);
  JQ8x00_ms(10);
}

void JQ8400_Voice_PlayAlarm(void)
{
  char path[] = JQ8400_FILE_ALARM;
  JQ8400_PlayPath(path);
}

void JQ8400_Voice_PlayRemind(void)
{
  char path[] = JQ8400_FILE_REMIND;
  JQ8400_PlayPath(path);
}

void JQ8400_Voice_PlayTaken(void)
{
  char path[] = JQ8400_FILE_TAKEN;
  JQ8400_PlayPath(path);
}
