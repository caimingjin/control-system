#include "stc15.h"
#include "usart.h"
#include "OLED.h"
#include "kongtiao.h"
#include "IR.h"
#define BLE_RST_WAIT_TIME 4 //4*20 80ms 
#define Slave_adrr 0x41 //���豸ʶ���ַA 0x41  B 0x42  C 0x43 D 0x44
extern unsigned char temper;//0-14 ����17-30 ��
extern unsigned char model;//ģʽ
extern unsigned char flag;
unsigned char  BLEReceive[2];
extern unsigned char BLEReceive_buff[2];
extern unsigned char  send_data[6];
extern bit full;
unsigned char time_flag=0;
bit read_buff=0;
bit BLE_RST_Flag_bit=0;
unsigned char BLE_RST_Flag=1;//������λ������ʹ����ʱ����Ϊ��ʱ��λ��ʼ��
void Delay25ms()		//@12.000MHz
{
	unsigned char i, j, k;

	_nop_();
	_nop_();
	i = 2;
	j = 36;
	k = 206;
	do
	{
		do
		{
			while (--k);
		} while (--j);
	} while (--i);
}


void Timer1Init_main(void)		//20����@12.000MHz
{
	AUXR &= 0xBF;		//��ʱ��ʱ��12Tģʽ
	TMOD &= 0x0F;		//���ö�ʱ��ģʽ
	TL1 = 0xE0;		//���ö�ʱ��ֵ
	TH1 = 0xB1;		//���ö�ʱ��ֵ
	TF1 = 0;		//���TF1��־
	TR1 = 1;		//��ʱ��1��ʼ��ʱ
	ET1=1;
	EA=1;
}
void Delay1ms()		//@12.000MHz
{
	unsigned char i, j;

	i = 12;
	j = 169;
	do
	{
		while (--j);
	} while (--i);
}
void system_init(void)
{
	BLE=0;//��λ����
	Delay1ms();
	BLE=1;//��λ����
	P15=1;
	P14=0;
	OLED_Init();
	Usart_init();
	Timer1Init_main();
	IR_init();
	WriteChinese16x16(0,0,"ģ��յ�");
	WriteStr_8x16(0,80,"CLOSE");
	WriteChinese16x16(1,0,"��");	
	WriteStr_8x16(1,18,":");
	WriteChinese16x16(2,0,"ģʽ ��");
	WriteStr_8x16(2,34,":");
}

void Check_uart_buff(void)
{
	unsigned char i=0;
	if(BLEReceive_buff[0]==Slave_adrr)
	{
//		if(BLEReceive_buff[0]==Slave_adrr && BLEReceive_buff[2]==Slave_adrr )
//				if(BLEReceive_buff[1]== BLEReceive_buff[3] )
//				{
					BLEReceive[0]=BLEReceive_buff[0];
					BLEReceive[1]=BLEReceive_buff[1];
//				}
			for(i=0;i<2;i++)
			{
				BLEReceive_buff[i]=0x00;
			}
	}
	if(BLEReceive_buff[1]==Slave_adrr)
	{
//			if(BLEReceive_buff[1]==Slave_adrr && BLEReceive_buff[3]==Slave_adrr )
//				if(BLEReceive_buff[2]== BLEReceive_buff[4] )
//				{
					BLEReceive[0]=BLEReceive_buff[1];
					BLEReceive[1]=BLEReceive_buff[0];
//				}
			for(i=0;i<2;i++)
			{
				BLEReceive_buff[i]=0x00;
			}
		
	}

}


/*
���յ� 000
�յ��� 000
�򿪿յ� 000    Aa
�յ��� 000

�յ��� 001
�ؿյ� 001
�رտյ� 001    Ab
�յ��ر� 001

��һ�� 002
̫���� 002
�¶ȵ�һ�� 002   Ac
�����¶� 002

��һ�� 003
̫���� 003       Ad
�����¶� 003
�¶ȸ�һ�� 003

����ģʽ 004    Ae

���� 005      Af
����ģʽ 005

���� 006        Ag
����ģʽ  006

��ʪ 007
��ʪģʽ  007   Ah

�Զ�ģʽ 008    Ai
*/
void main()
{
	unsigned char sum=0;
	system_init();
	while(1)
	{
		if(BLEReceive[0]==Slave_adrr   )//�жϽ��յ�����ǰ��λ�Ƿ�Ϊ�ӻ���ַ
		{
			BLEReceive[0]=0x00;//���
			switch(BLEReceive[1])//�жϺ��λ��Ӧ����
			{
				case 'a':
					AIR_On_on();
				break;
				case 'b':
					AIR_On_oFF();
				break;
				
				case 'c':
					AIR_temper_mul();			
				break;
				
				case 'd':
					AIR_temper_add();
				break;
				
					//����ģʽ 004    Ae
				case 'e':
					AIR_Model();
				break;
					
			//���� 005      Af  �Զ�0,"����1","��ʪ2","����3"  
				case 'f':   
					Model_Change(COOl);
				break;	
			//	���� 006        Ag
				case 'g':
					Model_Change(Hot);	
				break;	
			//	��ʪģʽ  007   Ah
				case 'h':
				Model_Change(WETTED);		
				break;	
			//	�Զ�ģʽ 008    Ai
				case 'i':
				Model_Change(Auto);			
				break;			
				case 'j':
				break;	

				default :
			if(BLEReceive[1]>'m')
				{
					AIR_temper_Set(BLEReceive[1]-'m');//��ֵ0-13
				}
				
			}
			

			BLEReceive[1]=0x00;

		}//
			
		PrintNum_8x16(3,0,sum);
			if(time_flag==1)
			{
				time_flag=0;
				sum++;

					Send_info();

				if(sum>9)sum=0;
				
			}
			if(read_buff)
			{
				read_buff=0;
				Check_uart_buff();
				
			}
			//��־��������������λ��������Ӧ��ʱ������Ӧ
			if(BLE_RST_Flag>=BLE_RST_WAIT_TIME &&BLE_RST_Flag_bit)
			{		
				BLE_reset();
				BLE_RST_Flag=0;
				BLE_RST_Flag_bit=0;
			}
			

	}
}
//20ms �ж�
void tm1_isr() interrupt 3 using 1
{
    static unsigned char mun;
	mun++;
	if(BLE_RST_Flag_bit)
	{
		if(BLE_RST_Flag<BLE_RST_WAIT_TIME)
		BLE_RST_Flag++;	
	}
	if(mun%3==0)  //100MS
	{
	
		read_buff=1;
		
	}
	if(mun==50)
	{
		mun=0;
		time_flag=1;
	}
}

