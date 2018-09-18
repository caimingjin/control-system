#include "stc15.h"
#include "usart.h"
#include "OLED.h"
#include "kongtiao.h"
#include "IR.h"
#define BLE_RST_WAIT_TIME 4 //4*20 80ms 
#define Slave_adrr 0x41 //从设备识别地址A 0x41  B 0x42  C 0x43 D 0x44
extern unsigned char temper;//0-14 代表17-30 度
extern unsigned char model;//模式
extern unsigned char flag;
unsigned char  BLEReceive[2];
extern unsigned char BLEReceive_buff[2];
extern unsigned char  send_data[6];
extern bit full;
unsigned char time_flag=0;
bit read_buff=0;
bit BLE_RST_Flag_bit=0;
unsigned char BLE_RST_Flag=1;//蓝牙复位，避免使用延时，当为零时复位开始，
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


void Timer1Init_main(void)		//20毫秒@12.000MHz
{
	AUXR &= 0xBF;		//定时器时钟12T模式
	TMOD &= 0x0F;		//设置定时器模式
	TL1 = 0xE0;		//设置定时初值
	TH1 = 0xB1;		//设置定时初值
	TF1 = 0;		//清除TF1标志
	TR1 = 1;		//定时器1开始计时
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
	BLE=0;//复位按键
	Delay1ms();
	BLE=1;//复位按键
	P15=1;
	P14=0;
	OLED_Init();
	Usart_init();
	Timer1Init_main();
	IR_init();
	WriteChinese16x16(0,0,"模拟空调");
	WriteStr_8x16(0,80,"CLOSE");
	WriteChinese16x16(1,0,"℃");	
	WriteStr_8x16(1,18,":");
	WriteChinese16x16(2,0,"模式 ：");
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
开空调 000
空调开 000
打开空调 000    Aa
空调打开 000

空调关 001
关空调 001
关闭空调 001    Ab
空调关闭 001

冷一点 002
太热了 002
温度低一点 002   Ac
降低温度 002

热一点 003
太冷了 003       Ad
升高温度 003
温度高一点 003

换个模式 004    Ae

制冷 005      Af
制冷模式 005

制热 006        Ag
制热模式  006

抽湿 007
抽湿模式  007   Ah

自动模式 008    Ai
*/
void main()
{
	unsigned char sum=0;
	system_init();
	while(1)
	{
		if(BLEReceive[0]==Slave_adrr   )//判断接收到数据前八位是否为从机地址
		{
			BLEReceive[0]=0x00;//清除
			switch(BLEReceive[1])//判断后八位对应功能
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
				
					//换个模式 004    Ae
				case 'e':
					AIR_Model();
				break;
					
			//制冷 005      Af  自动0,"制冷1","抽湿2","制热3"  
				case 'f':   
					Model_Change(COOl);
				break;	
			//	制热 006        Ag
				case 'g':
					Model_Change(Hot);	
				break;	
			//	抽湿模式  007   Ah
				case 'h':
				Model_Change(WETTED);		
				break;	
			//	自动模式 008    Ai
				case 'i':
				Model_Change(Auto);			
				break;			
				case 'j':
				break;	

				default :
			if(BLEReceive[1]>'m')
				{
					AIR_temper_Set(BLEReceive[1]-'m');//差值0-13
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
			//标志成立进行蓝牙复位，避免适应延时降低响应
			if(BLE_RST_Flag>=BLE_RST_WAIT_TIME &&BLE_RST_Flag_bit)
			{		
				BLE_reset();
				BLE_RST_Flag=0;
				BLE_RST_Flag_bit=0;
			}
			

	}
}
//20ms 中断
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

