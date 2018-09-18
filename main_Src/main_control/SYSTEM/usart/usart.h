#ifndef __USART_H
#define __USART_H
#include "stm32f10x_usart.h"
#include "stm32f10x_rcc.h"
#include "stdio.h"	
#include "sys.h" 

//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK STM32������
//����1��ʼ��		   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//�޸�����:2012/8/18
//�汾��V1.5
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2009-2019
//All rights reserved
//********************************************************************************
//V1.3�޸�˵�� 
//֧����Ӧ��ͬƵ���µĴ��ڲ���������.
//�����˶�printf��֧��
//�����˴��ڽ��������.
//������printf��һ���ַ���ʧ��bug
//V1.4�޸�˵��
//1,�޸Ĵ��ڳ�ʼ��IO��bug
//2,�޸���USART_RX_STA,ʹ�ô����������ֽ���Ϊ2��14�η�
//3,������USART_REC_LEN,���ڶ��崮�����������յ��ֽ���(������2��14�η�)
//4,�޸���EN_USART1_RX��ʹ�ܷ�ʽ
//V1.5�޸�˵��
//1,�����˶�UCOSII��֧��
#define USART2_R_END  0xaa  //����2��Ϣ���ݽ�����־
#define USART2_R_START 'S'  //����2��Ϣ���ݽ�����־
#define USART1_REC_LEN  			2  	//�����������ֽ��� 200
#define USART2_REC_LEN  			10  	//�����������ֽ��� 200
#define USART3_REC_LEN  			12  	//�����������ֽ��� 200
#define EN_USART1_RX 			1		//ʹ�ܣ�1��/��ֹ��0������1����
#define SYN_GET_HEDER  0XFC //SYN�ش�֡ͷ
void USART_SendStr(USART_TypeDef* USARTx, u8 *P);	  	
extern u8  USART_RX_BUF1[USART1_REC_LEN]; //���ջ���,���USART_REC_LEN���ֽ�.ĩ�ֽ�Ϊ���з� 
extern u8  USART_RX_BUF2[USART2_REC_LEN]; //���ջ���,���USART_REC_LEN���ֽ�.ĩ�ֽ�Ϊ���з� 
extern u8  USART_RX_BUF3[USART3_REC_LEN]; //���ջ���,���USART_REC_LEN���ֽ�.ĩ�ֽ�Ϊ���з� 
extern u16 USART1_RX_STA;         		//����״̬���	

extern u8 USART_RX_BUF3_NUM ;//��ѯ����������
extern u8 USART_RX_BUF3_FULL;
extern u8 USART_RX_BUF2_FULL;


//Һ����ʾ���Ա���
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

//����봮���жϽ��գ��벻Ҫע�����º궨��
void uart1_init(u32 bound);
void uart2_init(u32 bound);
void uart3_init(u32 bound);
void USART_Put_String(USART_TypeDef* USARTx, unsigned char *P ,u8 size);
//void USART_SendStr(USART_TypeDef* USARTx,  char *P,unsigned char Size);
#endif


