#ifndef __JQ8400_H
#define __JQ8400_H

#include "stm32f4xx_hal.h"
#include "usart.h"
#include <string.h>

#define JQ8x00_Workmode   0
#define JQ8x00_BusyCheck  0
#define ZH_MAX            50

#define JQ8x00_ms(ms)  HAL_Delay(ms)
#define JQ8x00_UART(pointer, len)  HAL_UART_Transmit(&huart3, (pointer), (len), 0xFFFFU)

typedef enum {
    OverallCycle      = 0x00,
    SingleCycle       = 0x01,
    SingleStop        = 0x02,
    OverallRandom     = 0x03,
    CatalogCycle      = 0x04,
    CatalogRandom     = 0x05,
    CatalogTurnPlay   = 0x06,
    OverallTurnPlay   = 0x07,
} LoopModeSelect;

typedef enum {
    CheckPlayState        = 0x01,
    Play                  = 0x02,
    pause                 = 0x03,
    Stop                  = 0x04,
    LastSong              = 0x05,
    NextSong              = 0x06,
    CheckOnelineDisksign  = 0x09,
    CheckCurrentDisksign  = 0x0A,
    CheckTotalTrack       = 0x0C,
    CurrentTrack          = 0x0D,
    LastFloder            = 0x0E,
    NextFloder            = 0x0F,
    EndPlay               = 0x10,
    CheckFloderFirstTrack = 0x11,
    CheckFloderAllTrack   = 0x12,
    AddVolume             = 0x14,
    DecVolume             = 0x15,
    EndZHPlay             = 0x1C,
    CheckSongShortName    = 0x1E,
    EndLoop               = 0x21,
    GetTotalSongTime      = 0x24,
    OpenPlayTime          = 0x25,
    ClosePlayTime         = 0x26,
} UartCommand;

typedef enum {
    AppointTrack        = 0x07,
    SetCycleCount       = 0x19,
    SetEQ               = 0x1A,
    SelectTrackNoPlay   = 0x19,
    GoToDisksign        = 0x0B,
    SetVolume           = 0x13,
    SetLoopMode         = 0x18,
    SetChannel          = 0x1D,
    AppointTimeBack     = 0x22,
    AppointTimeFast     = 0x23,
} UartCommandData;

typedef enum {
    JQ8X00_USB    = 0x00,
    JQ8X00_SD     = 0x01,
    JQ8X00_FLASH  = 0x02,
} JQ8X00_Symbol;

void JQ8x00_Init(void);
void JQ8x00_Command(UartCommand MODE);
void JQ8x00_Command_Data(UartCommandData MODE, uint8_t DATA);
void JQ8x00_ZuHeBoFang(uint8_t *DATA, uint8_t Len);
void JQ8x00_RandomPathPlay(JQ8X00_Symbol MODE, char *DATA);

#endif
