/**
  * @file    jq8400_voice.h
  * @brief   JQ8400 语音播报（USART3 / PB10）
  */
#ifndef __JQ8400_VOICE_H__
#define __JQ8400_VOICE_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "main.h"

#define JQ8400_FILE_ALARM   "/00001"
#define JQ8400_FILE_REMIND  "/00002"
#define JQ8400_FILE_TAKEN   "/00003"

void JQ8400_Voice_Init(void);
void JQ8400_Voice_PlayAlarm(void);
void JQ8400_Voice_PlayRemind(void);
void JQ8400_Voice_PlayTaken(void);

#ifdef __cplusplus
}
#endif

#endif /* __JQ8400_VOICE_H__ */
