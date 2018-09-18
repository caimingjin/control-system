#include "gpio.h"
#include "delay.h"
/*
һЩ����������ų�ʼ�����ã�
*/

//����ģ��æµ���м������
void SYN_BW_init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure; //�ṹ�����
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); //ʹ��PORTAʱ��
  GPIO_InitStructure.GPIO_Pin = SYN_BW_IO; //LED0~LED7---A0~A7
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; //
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; //IO���ٶ�Ϊ50MHZ
  GPIO_Init(SYN_BW_GPIO, &GPIO_InitStructure); //��ʼ��GPIO
 // GPIO_SetBits(GPIOA,GPIO_Pin_All); //A�ڳ�ʼ״̬���Ϊ1
}
void SYN_RCE_init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure; //�ṹ�����
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); //ʹ��PORTAʱ��
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6; //LED0~LED7---A0~A7
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; //
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; //IO���ٶ�Ϊ50MHZ
  GPIO_Init(GPIOA, &GPIO_InitStructure); //��ʼ��GPIO
 // GPIO_SetBits(GPIOA,GPIO_Pin_All); //A�ڳ�ʼ״̬���Ϊ1
}


//������ʼ������
void BLE_Reset_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure; //�ṹ�����
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); //ʹ��PORTAʱ��
  GPIO_InitStructure.GPIO_Pin = BLE_RST_IO; //
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; //��©���
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; //IO���ٶ�Ϊ50MHZ
  GPIO_Init(BLE_RST_GPIO, &GPIO_InitStructure); //��ʼ��GPIO
 // GPIO_SetBits(GPIOA,GPIO_Pin_All); //A�ڳ�ʼ״̬���Ϊ1
		GPIO_SetBits(GPIOA,BLE_RST_IO);

GPIO_ResetBits(BLE_RST_GPIO,BLE_RST_IO);
	delay_ms(10);
GPIO_SetBits(BLE_RST_GPIO,BLE_RST_IO);
}
