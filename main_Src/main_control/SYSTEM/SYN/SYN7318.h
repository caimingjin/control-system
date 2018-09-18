#include "stm32f10x.h"
#include "delay.h"
#include "UART3.h"
#include "usart.h"
#include "Slave.h"
#ifndef _SYN7318_H_
#define _SYN7318_H_
//ʶ����� ����
#define  COM_ID_NUM  26
#define IDENTIFY_DISTANCE  0x02  
//��ʶ�ȼ�
#define IDENTIFY_LEVEL_SO_LOW  0x05 // ���� ������
#define IDENTIFY_LEVEL__LOW  0x04  //��
#define IDENTIFY_LEVEL_MIDDLE  0x03   //��
#define IDENTIFY_LEVEL_HIGHT  0x02   //��
#define IDENTIFY_LEVEL_SO_HIGHT  0x01 //  ���� ������

extern unsigned char Back[4];
extern unsigned char ASR[6];
extern unsigned char S[4];
extern unsigned char Busy_Free;	
extern u8 SYN_Wake_Rsurt;
void IDENTIFY_INIT(void);
void SYN_TTS( char *Pst); //�����ϳɲ��ź���
void SYN_MP3(/*char *Pst,unsigned */unsigned char control);//����MP3
void Start_ASR(char Dict); //��ʼʶ����
void Dict_TTS(char Dict); //������������
void Stop_ASR(void); //ֹͣ����ʶ����
void Start_WakeUp(char Wake_ID); //�������Ѻ���
void Three_One(char Dict, char Wake_ID, char Sound, char *Pst); //����һ����
void Status_Query(void); //ģ��״̬��ѯ����
void Clear_USART_RX_BUF3(void);
void Dict_TTS_OK(char Dict);
#endif
