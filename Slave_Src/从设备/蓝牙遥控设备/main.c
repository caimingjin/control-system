/*
	晶振选择12M
	文件说明：IR红外发送相关函数，硬件设计时，没有将红外口引到PCA模块口，因此只能用定时器模拟38KHZ
	串口函数：Usart.c 	Usart.h 
	OLED显示屏函数：OLED.C OLED.H 
	空调控制函数：kongtiao.c 	kongtiao.h 

*/
#include "header.h"//头文件
#define MP3_START 0xff
#define MP3_QUIT  0x00
#define MP3_STOP  0xf0
#define MP3_CONTINUE  0x0f
#define MP3_NEXT  0xee
#define MP3_LAST  0xa0
//#define MP3_SONG_MAX  0x0a
unsigned char String_arr[4]={':',':','A','0'};
unsigned char MP3_Send_arr[6]={':',':','S','D','0',0xaa};
#define BLE P44
void Delay10ms(void)		//@11.0592MHz
{
	unsigned char i, j;

	i = 108;
	j = 145;
	do
	{
		while (--j);
	} while (--i);
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


//按键检测函数
uchar Check_key_4x4(void)
{
	uchar temp,key_dat=0;
	P2=0xff;
	P27=0;
	temp=P2;
	temp&=0x0f;
	if(temp!=0x0f)
	{
		Delay10ms();
		temp=P2;
		temp&=0x0f;
		if(temp!=0x0f)
		{
			switch(temp)
			{
				case 0x0e: key_dat=4;break;
				case 0x0d: key_dat=3;break;
				case 0x0b: key_dat=2;break;
				case 0x07: key_dat=1;break;
			}
			
		}
		while(temp!=0x0f){temp=P2;temp&=0x0f;}
	}
	
	P27=1;
	P26=0;
	temp=P2;
	temp&=0x0f;
	if(temp!=0x0f)
	{
		Delay10ms();
		temp=P2;
		temp&=0x0f;
		if(temp!=0x0f)
		{
			switch(temp)
			{
				case 0x0e: key_dat=8;break;
				case 0x0d: key_dat=7;break;
				case 0x0b: key_dat=6;break;
				case 0x07: key_dat=5;break;
			}
			
		}
		while(temp!=0x0f){temp=P2;temp&=0x0f;}
	}
	
	P26=1;
	P25=0;
	temp=P2;
	temp&=0x0f;
	if(temp!=0x0f)
	{
		Delay10ms();
		temp=P2;
		temp&=0x0f;
		if(temp!=0x0f)
		{
			switch(temp)
			{
				case 0x0e: key_dat=12;break;
				case 0x0d: key_dat=11;break;
				case 0x0b: key_dat=10;break;
				case 0x07: key_dat=9;break;
			}
			
		}
		while(temp!=0x0f){temp=P2;temp&=0x0f;}
	}
	
	
	
	P25=1;
	P24=0;
	temp=P2;
	temp&=0x0f;
	if(temp!=0x0f)
	{
		Delay10ms();
		temp=P2;
		temp&=0x0f;
		if(temp!=0x0f)
		{
			switch(temp)
			{
				case 0x0e: key_dat=16;break;
				case 0x0d: key_dat=15;break;
				case 0x0b: key_dat=14;break;
				case 0x07: key_dat=13;break;
			}
			
		}
		while(temp!=0x0f){temp=P2;temp&=0x0f;}
	}
	P2=0xff;
	return key_dat;
}


void system_init(void)
{
	P44=0;//复位按键
	Delay10ms();
	P44=1;//复位按键
	P15=1;
	P14=0;
	OLED_Init();
	Usart_init();
	IR_init();
	WriteChinese16x16(0,16,"模拟遥控");
}


/*
按键定义：前四个代表从设备选择，1空调，二电视，三灯
*/

void Delay50ms()		//@12.000MHz
{
	unsigned char i, j, k;

	_nop_();
	_nop_();
	i = 3;
	j = 80;
	k = 161;
	do
	{
		do
		{
			while (--k);
		} while (--j);
	} while (--i);
}
void BLE_Init(void)
{
				Delay50ms();
				BLE=0;
				Delay10ms();
				BLE=1;
}
/*
#define MP3_START 0xff
#define MP3_QUIT  0x00
#define MP3_STOP  0xf0
#define MP3_CONTINUE  0x0f
#define MP3_NEXT  0xee
#define MP3_LAST  0xa0
#define MP3_SONG_MAX  0x0a
*/
void MP3_Send_Fun(unsigned char key_dat )
{
		switch(key_dat)
	{
		case 5:
					MP3_Send_arr[4]=MP3_START;
				break;
		case 6:
					MP3_Send_arr[4]=MP3_QUIT;
				break;
		case 7:
					MP3_Send_arr[4]=MP3_STOP;
				break;
		case 8:
					MP3_Send_arr[4]=MP3_CONTINUE;
				break;
		case 9:
					MP3_Send_arr[4]=MP3_NEXT;
				break;
		case 10:
					MP3_Send_arr[4]=MP3_LAST;
				break;
		case 11:
					MP3_Send_arr[4]='g';
				break;
		case 12:
					MP3_Send_arr[4]='h';	
				break;
		case 13:			
					MP3_Send_arr[4]='i';
				break;
		case 14:
					MP3_Send_arr[4]='j';
				break;
		case 15:
					MP3_Send_arr[4]='k';
				break;
		case 16:
					MP3_Send_arr[4]='l';
				break;
//		//////////////////////
//		case 13:
//					String_arr[2]='n';
//				break;
//		case 14:		
//					String_arr[2]='m';
//				break;
//		case 15:
//				//	String_arr[2]='a';
//				break;
//		case 16:
//				//	String_arr[2]='a';
//				break;
	
	}
}
void Send_Fun(unsigned char key_dat )
{
		switch(key_dat)
	{
		case 5:
					String_arr[3]='a';
				break;
		case 6:
					String_arr[3]='b';
				break;
		case 7:
					String_arr[3]='c';

				break;
		case 8:
					String_arr[3]='d';
				break;
		case 9:
					String_arr[3]='e';
				break;
		case 10:
					String_arr[3]='f';
				break;
		case 11:
					String_arr[3]='g';
				break;
		case 12:
					String_arr[3]='h';	
				break;
		case 13:			
					String_arr[3]='i';
				break;
		case 14:
					String_arr[3]='j';
				break;
		case 15:
					String_arr[3]='k';
				break;
		case 16:
					String_arr[3]='l';
				break;
//		//////////////////////
//		case 13:
//					String_arr[2]='n';
//				break;
//		case 14:		
//					String_arr[2]='m';
//				break;
//		case 15:
//				//	String_arr[2]='a';
//				break;
//		case 16:
//				//	String_arr[2]='a';
//				break;
	
	}
}
/*
*矩阵键盘第一行前三个分别选择A,B,C设备
*后面三行则为功能按键 既一个设备有九个功能按键
*/

/*

*/
void main()
{

	unsigned char key_dat;
		system_init();
	
//默认情况进入是空调
	WriteChinese16x16(2,16,"控制空调");
			while(1)
		{
			key_dat=Check_key_4x4();
			if(key_dat!=0)
			{
				if(key_dat==2)
				{
					String_arr[2]='B';
					String_arr[3]='0';//切换的时候清空命令
					WriteChinese16x16(2,16,"控制灯  ");
					goto B;
				}
				else if(key_dat==3)
				{
					String_arr[2]='C';
					String_arr[3]='0';//切换的时候清空命令
					WriteChinese16x16(2,16,"控制电视");
					goto C;
				}
				else if(key_dat==4)
				{
					String_arr[2]='D';
					String_arr[3]='0';//切换的时候清空命令
					WriteMixStrXx16(2,16," MP3       ");
					goto D;
				}
				else
				{
					
					Send_Fun(key_dat);
					SendString(String_arr,4);
					BLE_Init();
				}
			}
		}
		
	while(1)
{

		A:while(1)
		{
			key_dat=Check_key_4x4();
			if(key_dat!=0)
			{
				if(key_dat==2)
				{
					String_arr[2]='B';
					String_arr[3]='0';//切换的时候清空命令
					WriteChinese16x16(2,16,"控制灯  ");
					goto B;
				}
				else if(key_dat==3)
				{
					String_arr[2]='C';
					String_arr[3]='0';//切换的时候清空命令
					WriteChinese16x16(2,16,"控制电视");
					goto C;
				}
				else if(key_dat==4)
				{
					String_arr[2]='D';
					String_arr[3]='0';//切换的时候清空命令
					WriteMixStrXx16(2,16,"  MP3       ");
					goto D;
				}
				else
				{
					Send_Fun(key_dat);
					SendString(String_arr,4);
					BLE_Init();
				}
			}
		}
		
		B:while(1)
		{
			key_dat=Check_key_4x4();
			if(key_dat!=0)
			{
				if(key_dat==1)
				{
					String_arr[2]='A';
					String_arr[3]='0';//切换的时候清空命令
					WriteChinese16x16(2,16,"控制空调");
					goto A;
				}
				else if(key_dat==3)
				{
					String_arr[2]='C';
					String_arr[3]='0';//切换的时候清空命令
					WriteChinese16x16(2,16,"控制电视");
					goto C;
				}
				else if(key_dat==4)
				{
					String_arr[2]='D';
					String_arr[3]='0';//切换的时候清空命令
					WriteMixStrXx16(2,16,"  MP3       ");
					goto D;
				}
				else
				{
					Send_Fun(key_dat);
					SendString(String_arr,4);
					BLE_Init();
				}
			}
		}
		
		C:while(1)
		{
			key_dat=Check_key_4x4();
			if(key_dat!=0)
			{
				if(key_dat==1)
				{
					String_arr[2]='A';
					String_arr[3]='0';//切换的时候清空命令
					WriteChinese16x16(2,16,"控制空调");
					goto A;
				}
				else if(key_dat==2)
				{
					String_arr[2]='B';
					String_arr[3]='0';//切换的时候清空命令
					WriteChinese16x16(2,16,"控制灯  ");
					goto B;
				}			
					else if(key_dat==4)
				{
					String_arr[2]='D';
					String_arr[3]='0';//切换的时候清空命令
					WriteMixStrXx16(2,16,"  MP3       ");
					goto D;
				}
				else
				{
					Send_Fun(key_dat);
					SendString(String_arr,4);
					BLE_Init();
				}
			
			}
		}
		
		
		
		D:while(1)
		{
			key_dat=Check_key_4x4();
			if(key_dat!=0)
			{
				if(key_dat==1)
				{
					String_arr[2]='A';
					String_arr[3]='0';//切换的时候清空命令
					WriteChinese16x16(2,16,"控制空调");
					goto A;
				}
				else if(key_dat==2)
				{
					String_arr[2]='B';
					String_arr[3]='0';//切换的时候清空命令
					WriteChinese16x16(2,16,"控制灯  ");
					goto B;
				}
				else if(key_dat==3)
				{
					String_arr[2]='C';
					String_arr[3]='0';//切换的时候清空命令
					WriteChinese16x16(2,16,"控制电视");
					goto C;
				}
				else 
				{
					MP3_Send_Fun(key_dat);
					SendString(MP3_Send_arr,6);
					BLE_Init();
				}
			
			}
		}
			

}

}