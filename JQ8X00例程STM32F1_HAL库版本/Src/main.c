#include "stm32f1xx_hal.h"
#include "usart.h"
#include "JQ8X00.h"
#include "delay.h"

/*****************************************************************************
*版权信息：深圳市佳仁科技有限公司
*当前版本：V3.0
*MCU 型号：STM32F1全系
*开发环境：Keil uVision5
*完成日期：2020-06-06
*作    者:JW
*程序功能:JQ8X00系列例程，基于HAL库
*其他说明：1:JQ8X00系列语音芯片的驱动全部包含于JQ8X00.c文件中，如需对文件IO做更改或者移植至
            自己的工程文件，只需修改JQ8X00.h文件中的定义。JQ8X00.c内调用函数全部在JQ8X00.h中替换
          2：连续发送指令时，请加入一定时间的延时间隔发送
          3：指定曲目播放，系统按照文件放入先后顺讯，依次编号曲目1 2 3 ...不按照文件名00001 000002...
********************************************************************************/

uint8_t aRxBuffer;

void SystemClock_Config(void)
{
    RCC_OscInitTypeDef RCC_OscInitStruct;
    RCC_ClkInitTypeDef RCC_ClkInitStruct;

    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;  // 外部晶振，8MHz
    RCC_OscInitStruct.HSEState = RCC_HSE_ON;
    RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
    RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
    RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;  // 9倍频，得到72MHz主时钟
    HAL_RCC_OscConfig(&RCC_OscInitStruct);

    RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                                  |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;       // 系统时钟：72MHz
    RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;              // AHB时钟：72MHz
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;               // APB1时钟：36MHz
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;               // APB2时钟：72MHz
    HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2);

    // HAL_RCC_GetHCLKFreq()/1000    1ms中断一次
    // HAL_RCC_GetHCLKFreq()/100000	 10us中断一次
    // HAL_RCC_GetHCLKFreq()/1000000 1us中断一次
    HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);  // 配置并启动系统滴答定时器
    /* 系统滴答定时器时钟源 */
    HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);
    /* 系统滴答定时器中断优先级配置 */
    HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
}

int main(void)
{
    char Path_Buffer[] = {"/测试1*/测试2*/00002"};         //播放路径： 根目录->测试1->测试2 下文件名为00002的音频文件
//    uint8_t ZH_Buffer[3] = {1,2,3};

    /* 复位所有外设，初始化Flash接口和系统滴答定时器 */
    HAL_Init();
    /* 配置系统时钟 */
    SystemClock_Config();
    delay_init(72);               		//初始化延时函数
    /* 初始化串口并配置串口中断优先级 */
    MX_UARTx_Init();

    JQ8x00_Init();
	HAL_Delay(500);              //等待模块稳定
	
	/*-----------串口模式下的操作-----------------*/
    JQ8x00_Command_Data(SetVolume,30);         //设置音量为30
    HAL_Delay(10);                               //连续发送指令，加入一定的延时等待模块处理完成上一条指令
    JQ8x00_Command_Data(AppointTrack,2);      //播放曲目2
//	HAL_Delay(10);
//    JQ8x00_ZuHeBoFang(ZH_Buffer,3);             //将ZH文件夹下名字为01 02 03的3个音频文件依次播放
//    HAL_Delay(10);
    JQ8x00_RandomPathPlay(JQ8X00_FLASH,Path_Buffer);      //播放 根目录->测试1->测试2 下文件名为00002的音频文件
	
	/*-----------单线模式下的操作-----------------*/
	//例程默认开启串口模式，开启单线模式需要更改JQ8X00.h中的宏定义
//	OneLine_ZHControl(10,Volume);		 //设置音量为10
//	HAL_Delay(10);                               //连续发送指令，加入一定的延时等待模块处理完成上一条指令
//	OneLine_ZHControl(1,Track);					//播放曲目一
    while (1)
    {
    }
}

/**
  * 函数功能: 串口接收完成回调函数
  * 输入参数: 无
  * 返 回 值: 无
  * 说    明：无
  */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *UartHandle)
{
    HAL_UART_Transmit(&huartx,&aRxBuffer,1,0);
    HAL_UART_Receive_IT(&huartx,&aRxBuffer,1);
}
