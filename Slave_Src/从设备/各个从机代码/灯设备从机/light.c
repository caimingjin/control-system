#include "light.h"
extern void Delay25ms();
extern void Delay1ms();
unsigned char Light_sta=0x00; //Ĭ�ϵƹر�
unsigned char Light_ON_off=0x00;
unsigned char code Light_data[]={0xfe,0xfd,0xfb,0xf7,0xef,0xdf,0xbf,0x7f};
extern unsigned char BLE_RST_Flag;
extern bit BLE_RST_Flag_bit;
// Slave_adrr   0x42
#define END_dat 0xaa
/*  
INFO[0] : ':' BLE�̶�ǰ׺
INFO[1] : ':' BLE�̶�ǰ׺
INFO[2] : 'S' ��Ϣ��ʼ
INFO[3] : ʶ��� ('B'==0x42)
INFO[4] �����ػ�״̬
INFO[5] ����������
INFO[6] ��END_dat
INFO[7] ������
INFO[8] ������
INFO[9] ������
*/

unsigned char INFO[]={':',':','S',0x42,0x00,0x00,END_dat};
void Delay50ms()		//@12.000MHz
{
	unsigned char i, j, k;

	_nop_();
	_nop_();
	i = 3;
	j = 75;
	k = 161;
	do
	{
		do
		{
			while (--k);
		} while (--j);
	} while (--i);
}

void Delay10ms()		//@12.000MHz
{
	unsigned char i, j;

	i = 117;
	j = 184;
	do
	{
		while (--j);
	} while (--i);
}


//��������ĸ���
unsigned char Count_Light_Num(void)
{
	unsigned char temp;
	unsigned char i,Light_Num=0;
	temp=Light_sta;
		for(i=0;i<8;i++)
	{
		if(temp & 0x01 ==0x01)
			;		
		else	
			Light_Num++;
		temp>>=1;
	}
	return Light_Num;
}
void BLE_reset(void)
{
		//Delay50ms();//��֤�������ٷ���һ֡
		BLE=0;  //��������200us�����ߣ�������
		Delay10ms();
		BLE=1;
}
void Send_info(void)
{
	//Delay25ms();//��֤�������ٷ���һ֡
	INFO[4]=Light_ON_off;
	INFO[5]=Count_Light_Num();
	PrintNum_8x16(1,73,INFO[5]);
	SendString_Size(INFO,7);
	//������λǰ��Ҫ��ʱ
	BLE_RST_Flag=0;//��־��ʼ
	BLE_RST_Flag_bit=1;
//	BLE_reset();
}

void Light_Off(void)
{
	P2=0xff;
	Light_sta=0xff;
	Light_ON_off=0x00;
	Send_info();

}

void Light_ON(void)
{
	if(Light_ON_off==0)
	{
			P2=0xfe;
			Light_sta=0xfe;
			Light_ON_off=0xff;
			
	}
		Send_info();
	

}
void Light_More(void)
{
	//if(Light_sta!=0x80)
	if(Light_ON_off)
	{
			Light_sta<<=1;
			P2=Light_sta;
			
			Send_info();
		
	}

}

void Light_Less(void)
{
	if(Light_ON_off)
	{
			if(Light_sta==0x00)
			{
				Light_sta=0x80;
			}

			else
			{
						Light_sta>>=1;
						Light_sta|=0x80;
						P2=Light_sta;
			}
			
		Send_info();

	}

}


void Light_Change(unsigned char temp)
{
	//if(Light_sta!=0x80)
//	if(Light_ON_off)
//	{
//			Light_sta<<=1;
//			P2=Light_sta;
//			
//	}
	if(Light_ON_off)
	{
	switch(temp)
	{
		case 3:
			Light_sta=0x00;
			break;
		case 2:
			Light_sta=0xf0;
			break;
		case 1:
			Light_sta=0xfe;
			break;
		
	}
		P2=Light_sta;
		Send_info();
	
}
	

}

