#include "stc15.h"
#include "usart.h"
#include "OLED.h"
#include "light.h"
#define BLE_RST_WAIT_TIME 5  //3*20 60ms 
#define Slave_adrr 0x42 //���豸ʶ���ַA 0x41  B 0x42  C 0x43 D 0x44
unsigned char  BLEReceive[2];
extern unsigned char BLEReceive_buff[2];
extern bit full;
unsigned char time_flag=0;
extern unsigned char Light_sta;
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
	WriteChinese16x16(0,0,"ģ���");
	WriteChinese16x16(1,0,"���Ƹ���");
	WriteStr_8x16(1,65,":");
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
					WriteStr_8x16(0,80,"ON");
					Light_ON();
				break;
				
				case 'b':
					WriteStr_8x16(0,80,"OF");
					Light_Off();
				break;
				
				case 'c':
					Light_Less();
				break;
				
				case 'd':
					Light_More();
					break;
				
				case 'e':
						Light_Change(Light_MAX);
				break;
					
				case 'f':
					Light_Change(Light_MID);
				break;	
								
				case 'g':
					Light_Change(Light_MIN);
				break;	
						
				case 'h':
				break;	
				
				case 'i':
				break;	
				
				case 'j':
				break;	
			
			}
			BLEReceive[1]=0x00;

		}//
			
		PrintNum_8x16(3,0,sum);
			if(time_flag==1)
			{
				time_flag=0;
				sum++;
				//if(Light_sta)//�������
				Send_info();
				if(sum>9)sum=0;
			}
			if(read_buff)
			{
				read_buff=0;
				Check_uart_buff();
				
			}
			
			
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

	if(mun%4==0)  //
	{
		read_buff=1;	//60ms

	}
	if(mun==100)
	{
		mun=0;
		time_flag=1;
	}
}

