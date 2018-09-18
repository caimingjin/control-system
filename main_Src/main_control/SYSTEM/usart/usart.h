#ifndef __USART_H
#define __USART_H
#include "stm32f10x_usart.h"
#include "stm32f10x_rcc.h"
#include "stdio.h"	
#include "sys.h" 

//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK STM32开发板
//串口1初始化		   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//修改日期:2012/8/18
//版本：V1.5
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2009-2019
//All rights reserved
//********************************************************************************
//V1.3修改说明 
//支持适应不同频率下的串口波特率设置.
//加入了对printf的支持
//增加了串口接收命令功能.
//修正了printf第一个字符丢失的bug
//V1.4修改说明
//1,修改串口初始化IO的bug
//2,修改了USART_RX_STA,使得串口最大接收字节数为2的14次方
//3,增加了USART_REC_LEN,用于定义串口最大允许接收的字节数(不大于2的14次方)
//4,修改了EN_USART1_RX的使能方式
//V1.5修改说明
//1,增加了对UCOSII的支持
#define USART2_R_END  0xaa  //串口2信息传递结束标志
#define USART2_R_START 'S'  //串口2信息传递结束标志
#define USART1_REC_LEN  			2  	//定义最大接收字节数 200
#define USART2_REC_LEN  			10  	//定义最大接收字节数 200
#define USART3_REC_LEN  			12  	//定义最大接收字节数 200
#define EN_USART1_RX 			1		//使能（1）/禁止（0）串口1接收
#define SYN_GET_HEDER  0XFC //SYN回传帧头
void USART_SendStr(USART_TypeDef* USARTx, u8 *P);	  	
extern u8  USART_RX_BUF1[USART1_REC_LEN]; //接收缓冲,最大USART_REC_LEN个字节.末字节为换行符 
extern u8  USART_RX_BUF2[USART2_REC_LEN]; //接收缓冲,最大USART_REC_LEN个字节.末字节为换行符 
extern u8  USART_RX_BUF3[USART3_REC_LEN]; //接收缓冲,最大USART_REC_LEN个字节.末字节为换行符 
extern u16 USART1_RX_STA;         		//接收状态标记	

extern u8 USART_RX_BUF3_NUM ;//查询数据区长度
extern u8 USART_RX_BUF3_FULL;
extern u8 USART_RX_BUF2_FULL;


//液晶显示测试变量
extern uint16_t imbuff[3];
extern uint16_t Receive_full;
extern unsigned int i;
extern unsigned int LED_Flag;
extern unsigned int Fridge_Flag;
extern unsigned int TV_Flag;
extern unsigned int Fridge_C_Flag;
extern unsigned int Fridge_W_Flag;
extern unsigned int TV_Vp_Flag;
extern unsigned int TV_Vd_Flag;
extern unsigned int TV_Cp_Flag;
extern unsigned int TV_Cd_Flag;

//如果想串口中断接收，请不要注释以下宏定义
void uart1_init(u32 bound);
void uart2_init(u32 bound);
void uart3_init(u32 bound);
void USART_Put_String(USART_TypeDef* USARTx, unsigned char *P ,u8 size);
//void USART_SendStr(USART_TypeDef* USARTx,  char *P,unsigned char Size);
#endif


