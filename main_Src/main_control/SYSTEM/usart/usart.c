#include "sys.h"
#include "delay.h"
#include "usart.h"	 
#include "gizwits_protocol.h"
////////////////////////////////////////////////////////////////////////////////// 	 
//如果使用ucos,则包括下面的头文件即可.
#if SYSTEM_SUPPORT_OS
#include "includes.h"					//ucos 使用	  
#endif
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
////////////////////////////////////////////////////////////////////////////////// 	  



 //液晶显示变量
 uint16_t imbuff[3], Receive_full;
unsigned int i = 0,LED_Flag=0,Fridge_Flag=0,TV_Flag=0;
unsigned int Fridge_C_Flag=0,Fridge_W_Flag = 0,TV_Vd_Flag=0,TV_Vp_Flag = 0,TV_Cp_Flag = 0,TV_Cd_Flag=0;



//////////////////////////////////////////////////////////////////
//加入以下代码,支持printf函数,而不需要选择use MicroLIB	  
#if 1
#pragma import(__use_no_semihosting)             
//标准库需要的支持函数                 
struct __FILE 
{ 
	int handle; 

}; 

FILE __stdout;       
//定义_sys_exit()以避免使用半主机模式    
void _sys_exit(int x) 
{ 
	x = x; 
} 
//重定义fputc函数 
int fputc(int ch, FILE *f)
{      
if((USART2->SR&0X40)==0);//循环发送,直到发送完毕   
    USART2->DR = (u8) ch;      
	return ch;
}
#endif 

/*使用microLib的方法*/
 /* 
int fputc(int ch, FILE *f)
{
	USART_SendData(USART1, (uint8_t) ch);

	while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET) {}	
   
    return ch;
}
int GetKey (void)  { 

    while (!(USART1->SR & USART_FLAG_RXNE));

    return ((int)(USART1->DR & 0x1FF));
}
*/
 
#if EN_USART1_RX   //如果使能了接收
//串口1中断服务程序
//注意,读取USARTx->SR能避免莫名其妙的错误   	
u8 USART_RX_BUF1[USART1_REC_LEN]={0};     //接收缓冲,最大USART_REC_LEN个字节.
u8 USART_RX_BUF2[USART2_REC_LEN]={0};  
u8  USART_RX_BUF3[USART3_REC_LEN]={0,0,0,0,0,0};
u8 USART_RX_BUF3_NUM =0;
u8 USART_RX_BUF3_FULL =0;

u8 USART_RX_BUF2_FULL=0;
//接收状态
//bit15，	接收完成标志
//bit14，	接收到0x0d
//bit13~0，	接收到的有效字节数目
u16 USART1_RX_STA=0;       //接收状态标记	  
#endif  

void uart1_init(u32 bound){
  //GPIO端口设置
  GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1|RCC_APB2Periph_GPIOA, ENABLE);	//使能USART1，GPIOA时钟
  
	//USART1_TX   GPIOA.9
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9; //PA.9
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//复用推挽输出
  GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化GPIOA.9
   
  //USART1_RX	  GPIOA.10初始化
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;//PA10
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//浮空输入
  GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化GPIOA.10  

  //Usart1 NVIC 配置
  NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;//抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器
  
   //USART 初始化设置

	USART_InitStructure.USART_BaudRate = bound;//串口波特率
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式

  USART_Init(USART1, &USART_InitStructure); //初始化串口1
 	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);	//开启串口接受中断
  USART_Cmd(USART1, ENABLE);                    //使能串口1 
	USART_ClearFlag(USART1, USART_FLAG_TC);   

}




void uart2_init(u32 bound){
  //GPIO端口设置
  GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	 
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);	//使能USART2，GPIOA时钟
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	//使能USART2，GPIOA时钟
  
	//USART2_TX   GPIOA.2
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2; //PA.2
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//复用推挽输出
  GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化GPIOA.9
   
  //USART2_RX	  GPIOA.3初始化
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;//PA3
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//浮空输入
  GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化GPIOA.10  

  //Usart1 NVIC 配置
  NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2 ;//抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器
  
   //USART 初始化设置

	USART_InitStructure.USART_BaudRate = bound;//串口波特率
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式

  USART_Init(USART2, &USART_InitStructure); //初始化串口1
  USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);//开启串口接受中断
  USART_Cmd(USART2, ENABLE);                    //使能串口1 
	USART_GetFlagStatus(USART2,USART_FLAG_TC); //不加这句会出现：发送字符串第一个字节丢失的情况
}


void uart3_init(u32 bound){
  //GPIO端口设置
  GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	 
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);	//使能USART3,GPIOA时钟
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	//使能USART3，GPIOA时钟
  
	//USART1_TX  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10; 
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//复用推挽输出
  GPIO_Init(GPIOB, &GPIO_InitStructure);
   
  //USART1_RX	 
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//浮空输入
  GPIO_Init(GPIOB, &GPIO_InitStructure);

  //Usart1 NVIC 配置
  NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2 ;//抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 4;		//子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道不使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器
  
   //USART 初始化设置

	USART_InitStructure.USART_BaudRate = bound;//串口波特率
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式

  USART_Init(USART3, &USART_InitStructure); //初始化串口3
  USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);//开启串口接受中断
  USART_Cmd(USART3, ENABLE);                    //使能串口3 
	USART_GetFlagStatus(USART3,USART_FLAG_TC); //不加这句会出现：发送字符串第一个字节丢失的情况
}


//void USART1_IRQHandler(void)                	//串口1中断服务程序
//	{
//	u8 Res;
//#if SYSTEM_SUPPORT_OS 		//如果SYSTEM_SUPPORT_OS为真，则需要支持OS.
//	OSIntEnter();    
//#endif
//	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)  //接收中断(接收到的数据必须是0x0d 0x0a结尾)
//		{
//		Res =USART_ReceiveData(USART1);	//读取接收到的数据
//		
//		if((USART_RX_STA&0x8000)==0)//接收未完成
//			{
//			if(USART_RX_STA&0x4000)//接收到了0x0d
//				{
//				if(Res!=0x0a)USART_RX_STA=0;//接收错误,重新开始
//				else USART_RX_STA|=0x8000;	//接收完成了 
//				}
//			else //还没收到0X0D
//				{	
//				if(Res==0x0d)USART_RX_STA|=0x4000;
//				else
//					{
//					USART_RX_BUF[USART_RX_STA&0X3FFF]=Res ;
//					USART_RX_STA++;
//					if(USART_RX_STA>(USART_REC_LEN-1))USART_RX_STA=0;//接收数据错误,重新开始接收	  
//					}		 
//				}
//			}   		 
//     } 
//#if SYSTEM_SUPPORT_OS 	//如果SYSTEM_SUPPORT_OS为真，则需要支持OS.
//	OSIntExit();  											 
//#endif
//} 
//#endif	

void USART1_IRQHandler(void)                	//串口1中断服务程序
	{
			 u8 res=0;

	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)  //接收中断
		{
		//	USART_RX_BUF1[flag] =USART_ReceiveData(USART1);	//读取接收到的数据
			//flag++;
			//if(flag==USART1_REC_LEN) flag=0;
			USART_ClearFlag(USART1, USART_IT_RXNE);   
			res = USART_ReceiveData(USART1);
		  gizPutData(&res, 1);
			 
     } 

} 
	

void USART2_IRQHandler(void)                	//串口1中断服务程序
	{
			static u8 flag=0;
						 u8 temp;
	if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)  //接收中断
		{ 
				USART_ClearFlag(USART2, USART_IT_RXNE);   
				temp=USART_ReceiveData(USART2);//读取接收到的数据
				if(flag==0 && temp==USART2_R_START)
			{
					USART_RX_BUF2[0] =temp;	
					flag=1;
					USART_RX_BUF2_FULL=0;
			}
			else
			{
				if(flag!=0 && USART_RX_BUF2_FULL==0)
				{
						if(temp!=USART2_R_END)
						{
							USART_RX_BUF2[flag++]=temp;	
						}
						else
						{
								flag=0;
								USART_RX_BUF2_FULL=1;
						}
				}
				else
				{
					flag=0;	
					USART_RX_BUF2_FULL=0;
				}	
			}	 
     }
}
	

void USART3_IRQHandler(void)                	//串口3中断服务程序
	{
			static u8 flag=0;
			u8 temp='0';

	if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)  //接收中断
		{
				USART_ClearFlag(USART3, USART_IT_RXNE);
			 temp=USART_ReceiveData(USART3);	//读取接收到的数据
			if(temp==SYN_GET_HEDER && flag==0)
			{
				flag=1;
				USART_RX_BUF3[0]=temp;
				USART_RX_BUF3_NUM=0;
			}
			else 
			{
				if(flag!=0 && temp!=SYN_GET_HEDER)
				{
					USART_RX_BUF3[flag]=temp;
					
					if(flag==2)
					{
						USART_RX_BUF3_NUM=temp-0x00;
						if(USART_RX_BUF3_NUM==6 ||USART_RX_BUF3_NUM==4 ||USART_RX_BUF3_NUM==1);
						else
						USART_RX_BUF3_NUM=0;
							
					}
					if(flag==USART_RX_BUF3_NUM+2) 
					{
						flag=0;
						USART_RX_BUF3_FULL	=1;
						USART_RX_BUF3_NUM=0;
					}
					else
					{
						USART_RX_BUF3_FULL	=0;	
						flag++;
					}
				}
			} 
    } 

}

	
//void USART_SendStr(USART_TypeDef* USARTx,  char *P,unsigned char Size);
// {
//	 while(*P)
//	 {
//		 USART_SendData(USARTx, *P);
//		 P++;
//		 delay_us(100);
//	 }
//	 
// }
 
void USART_Put_String(USART_TypeDef* USARTx,  unsigned char *P ,u8 size)
{
	u8 i=0;
	for(i=0;i<size;i++)
	{
		USART_SendData(USARTx, P[i]);
		delay_us(80);
	}
}

