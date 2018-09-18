#ifndef __SLAVE_H
#define __SLAVE_H
#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "SYN7318.h"
#include "string.h"
#include "gpio.h"
#include "gizwits_product.h"
#include "gizwits_protocol.h"
//���岥��������ֵ
#define MP3_START 0xff
#define MP3_QUIT  0x00
#define MP3_STOP  0xf0
#define MP3_CONTINUE  0x0f
#define MP3_NEXT  0xee
#define MP3_LAST  0xa0

#define MP3_SONG_MAX  8
//MP3��������



#define  BLE_USART  USART2

extern unsigned char Send2Slave[4];
extern unsigned char Slave_adrrA_AIR_BUFF[3];   //�ֱ��� ���ػ� ģʽ��0-3�� �¶ȣ�0-13��+17
extern unsigned char Slave_adrrB_Light_BUFF[2];  //���ػ�  �Ƶȼ���0-7��
extern unsigned char Slave_adrrC_TV_BUFF[2];    //���ػ�   Ƶ�� 0-99



void BLE_Send(void);
//���豸��Ĭ��255 ����0��ʾ�ػ���1��ʾ����
#define POWER_ON  0xff
#define POWER_OFF 0

#define Command_NUM  22  //���壬����ID����
//#define TV_OFF 0

//#define AIR_ON 1
//#define AIR_OFF 0
void Check_USART_RX_BUF2(void);  
u8 Execute_Result(void);
#endif

