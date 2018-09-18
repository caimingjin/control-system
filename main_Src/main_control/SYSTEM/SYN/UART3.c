#include "stm32f10x.h"
#include "UART3.h"
#include "delay.h"
/**************************************************************
*��������UARTInit----���ڳ�ʼ��
*��  ����bound----������
*��  ����UARTInit(115200); //����3��ʼ��,����������Ϊ115200bps
**************************************************************/
void UARTInit3(uint32_t bound)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	
	//����ʱ��ʹ��,GPIOʱ��ʹ��,����ʱ��ʹ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	
	//���ڸ�λ
	USART_DeInit(USART3);
	
	//GPIO�˿�ģʽ����
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10; //USART3_TX----PB.10
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; //�����������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);   //��ʼ��PB.10
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11; //USART3_RX----PB.11
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; //��������
	GPIO_Init(GPIOB, &GPIO_InitStructure);   //��ʼ��PB.11
	
	//���ڲ�����ʼ��
	USART_InitStructure.USART_BaudRate = bound; //����������
	USART_InitStructure.USART_WordLength =  USART_WordLength_8b; //�ֳ�Ϊ8λ
	USART_InitStructure.USART_StopBits = USART_StopBits_1; //һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No; //����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; //��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx|USART_Mode_Tx; //�շ�ģʽ
  USART_Init(USART3, &USART_InitStructure);	
	
	//����ʹ��
	USART_Cmd(USART3, ENABLE);
	USART_GetFlagStatus(USART3,USART_FLAG_TC); //����������֣������ַ�����һ���ֽڶ�ʧ�����
	
}


/**************************************************************
*��������UART_Put_Char----���ڷ���һ���ֽں���
*��  ����Data----�����͵��ֽ�����
*��  ����UART_Put_Char('a');
*        Delay(1000);
**************************************************************/
void UART_Put_Char(uint16_t Data)
{
	USART_SendData(USART3, Data);
	while(USART_GetFlagStatus(USART3,USART_FLAG_TC) != SET); //�ȴ����ݷ������
}


/**************************************************************
*��������UART_Get_char----���ڽ���һ���ֽں���
*��  ������
*��  ����uint16_t Receive_Data;
*        Receive_Data = UART_Get_char();
*	       UART_Put_Char(Receive_Data);  //�ѽ��յ��������������
**************************************************************/
uint16_t UART_Get_char(void)
{
	uint16_t Data;
	while(USART_GetFlagStatus(USART3,USART_FLAG_RXNE) != SET); //�ȴ����ռĴ�����
	Data = USART_ReceiveData(USART3);
	return Data;
}


/**************************************************************
*��������UART_Put_String----���ڷ����ַ�������
*��  ����Pst������ַ�����������
*        Length���ַ�������
*��  ����char Send[5] = {'a','b','c','d','e'};
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
*��������UART_Get_String----���ڽ����ַ�������
*��  ����Pst����Ž��յ����ַ�����������
*        Length���ַ�������
*��  ����unsigned char Receive[5] = {0};
*        UART_Get_String(Receive,5);
*	       UART_Put_String(Receive,5);//������յ����ַ��� 
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





























