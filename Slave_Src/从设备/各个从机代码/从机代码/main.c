#include "stc15.h"
#include "usart.h"
#include "OLED.h"
#define Slave_adrr 0x41 //���豸ʶ���ַA 0x41  B 0x42  C 0x43 D 0x44

/*********************************************************************
���� �� ������UART1_Get_char----����1����һ���ֽں���
������˵�������޲�
���� �� ֵ�������յ����ֽ�
����    ������uchar d;
              d = UART1_Get_char();
              UART1_Put_Char(d); -----������յ����ֽ�
*********************************************************************/
unsigned char UART1_Get_char() 
{
	while(RI == 0);  //RI = 1;���ս���
	RI = 0;
	return(SBUF);	
}
/*********************************************************************
���� �� ������UART1_Get_String----����1�����ַ�������
������˵������Pst����Ž��յ����ַ�����������
              Length���ַ�������
����    ������uchar d[4] = {0};
              UART1_Get_String(d,4);
              UART1_Put_String(d,4);----������յ����ַ��� 
*********************************************************************/
void UART1_Get_String(unsigned char *Pst,unsigned char Length)
{
  unsigned char i;
  for(i=0;i<Length;i++)
  {
     Pst[i] = UART1_Get_char();
  }
}
/*
* ChanmelFunction��Ƶ���͹�������
*���255 �㹻��
*/
//unsigned char  ChanmelFunction;

/*
* BLEReceive��������������
*ǰ��λ��ַʶ�𣬺��λ����λ
*/
void Delay100ms()		//@11.0592MHz
{
	unsigned char i, j, k;

	_nop_();
	_nop_();
	i = 1;
	j = 52;
	k = 195;
	do
	{
		do
		{
			while (--k);
		} while (--j);
	} while (--i);
}

unsigned char  BLEReceive[2];
void system_init(void)
{
	P44=0;//��λ����
	Delay100ms();
	P44=1;//��λ����
	P15=1;
	P14=0;
	OLED_Init();
	Usart_init();
	WriteChinese16x16(0,32,"ģ�����");
}

void main()
{
	system_init();
	while(1)
	{
		if(BLEReceive[0]==Slave_adrr)//�жϽ��յ�����ǰ��λ�Ƿ�Ϊ�ӻ���ַ
		{
			BLEReceive[0]=0x00;//���
			switch(BLEReceive[1])//�жϺ��λ��Ӧ����
			{
				case 'a':
					//SendString("::a");
					WriteStr_8x16(3,20,"a");
					break;
				case 'b':
				//	SendString("::b");
					WriteStr_8x16(3,20,"b");
					break;
				case 'c':
					//SendString("::c");
					WriteStr_8x16(3,20,"c");
					break;
				case 'd':
					//SendString("::d");
					WriteStr_8x16(3,20,"d");
					break;
				case 'e':
				//	SendString("::e");
					WriteStr_8x16(3,20,"e");
					break;
				case 'f':
				//	SendString("::f");
					WriteStr_8x16(3,20,"f");
					break;	
				default:
					//SendString("::0");
					WriteStr_8x16(3,20,"0");
					
			}
			BLEReceive[1]=0x00;
		}
		Delay100ms();
	}
}

