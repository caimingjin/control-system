#include "stm32f10x.h"
#include "UART3.h"
#include "delay.h"
/**************************************************************
*函数名：UARTInit----串口初始化
*参  数：bound----波特率
*简  例：UARTInit(115200); //串口3初始化,波特率设置为115200bps
**************************************************************/
void UARTInit3(uint32_t bound)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	
	//串口时钟使能,GPIO时钟使能,复用时钟使能
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	
	//串口复位
	USART_DeInit(USART3);
	
	//GPIO端口模式设置
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10; //USART3_TX----PB.10
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; //复用推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);   //初始化PB.10
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11; //USART3_RX----PB.11
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; //浮空输入
	GPIO_Init(GPIOB, &GPIO_InitStructure);   //初始化PB.11
	
	//串口参数初始化
	USART_InitStructure.USART_BaudRate = bound; //波特率设置
	USART_InitStructure.USART_WordLength =  USART_WordLength_8b; //字长为8位
	USART_InitStructure.USART_StopBits = USART_StopBits_1; //一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No; //无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; //无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx|USART_Mode_Tx; //收发模式
  USART_Init(USART3, &USART_InitStructure);	
	
	//串口使能
	USART_Cmd(USART3, ENABLE);
	USART_GetFlagStatus(USART3,USART_FLAG_TC); //不加这句会出现：发送字符串第一个字节丢失的情况
	
}


/**************************************************************
*函数名：UART_Put_Char----串口发送一个字节函数
*参  数：Data----待发送的字节数据
*简  例：UART_Put_Char('a');
*        Delay(1000);
**************************************************************/
void UART_Put_Char(uint16_t Data)
{
	USART_SendData(USART3, Data);
	while(USART_GetFlagStatus(USART3,USART_FLAG_TC) != SET); //等待数据发送完成
}


/**************************************************************
*函数名：UART_Get_char----串口接收一个字节函数
*参  数：无
*简  例：uint16_t Receive_Data;
*        Receive_Data = UART_Get_char();
*	       UART_Put_Char(Receive_Data);  //把接收到的数据输出出来
**************************************************************/
uint16_t UART_Get_char(void)
{
	uint16_t Data;
	while(USART_GetFlagStatus(USART3,USART_FLAG_RXNE) != SET); //等待接收寄存器满
	Data = USART_ReceiveData(USART3);
	return Data;
}


/**************************************************************
*函数名：UART_Put_String----串口发送字符串函数
*参  数：Pst：存放字符串的数组名
*        Length：字符串长度
*简  例：char Send[5] = {'a','b','c','d','e'};
*        UART_Put_String(Send,5);
**************************************************************/
void UART_Put_String(char *Pst,uint16_t Length)
{
	uint16_t i;
	for(i = 0; i < Length; i++)
	{
	  UART_Put_Char(Pst[i]);
		delay_us(50);
	}

}


/**************************************************************
*函数名：UART_Get_String----串口接收字符串函数
*参  数：Pst：存放接收到的字符串的数组名
*        Length：字符串长度
*简  例：unsigned char Receive[5] = {0};
*        UART_Get_String(Receive,5);
*	       UART_Put_String(Receive,5);//输出接收到的字符串 
**************************************************************/
void UART_Get_String(unsigned char *Pst,uint16_t Length)
{
	uint16_t i;
	for(i = 0; i < Length; i++)
	{
	  Pst[i] = UART_Get_char();
		delay_us(100);

	}
}





























