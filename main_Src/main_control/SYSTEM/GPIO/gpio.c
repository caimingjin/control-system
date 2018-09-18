#include "gpio.h"
#include "delay.h"
/*
一些输入输出引脚初始化配置，
*/

//语音模块忙碌空闲检测引脚
void SYN_BW_init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure; //结构体变量
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); //使能PORTA时钟
  GPIO_InitStructure.GPIO_Pin = SYN_BW_IO; //LED0~LED7---A0~A7
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; //
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; //IO口速度为50MHZ
  GPIO_Init(SYN_BW_GPIO, &GPIO_InitStructure); //初始化GPIO
 // GPIO_SetBits(GPIOA,GPIO_Pin_All); //A口初始状态输出为1
}
void SYN_RCE_init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure; //结构体变量
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); //使能PORTA时钟
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6; //LED0~LED7---A0~A7
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; //
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; //IO口速度为50MHZ
  GPIO_Init(GPIOA, &GPIO_InitStructure); //初始化GPIO
 // GPIO_SetBits(GPIOA,GPIO_Pin_All); //A口初始状态输出为1
}


//蓝牙初始化引脚
void BLE_Reset_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure; //结构体变量
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); //使能PORTA时钟
  GPIO_InitStructure.GPIO_Pin = BLE_RST_IO; //
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; //开漏输出
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; //IO口速度为50MHZ
  GPIO_Init(BLE_RST_GPIO, &GPIO_InitStructure); //初始化GPIO
 // GPIO_SetBits(GPIOA,GPIO_Pin_All); //A口初始状态输出为1
		GPIO_SetBits(GPIOA,BLE_RST_IO);

GPIO_ResetBits(BLE_RST_GPIO,BLE_RST_IO);
	delay_ms(10);
GPIO_SetBits(BLE_RST_GPIO,BLE_RST_IO);
}
