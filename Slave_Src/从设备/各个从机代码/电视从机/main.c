#include "stc15.h"
#include "usart.h"
#include "OLED.h"
#include "IR.h"
#include "TV.h"
#define Slave_adrr 0x43 //从设备识别地址A 0x41  B 0x42  C 0x43 D 0x44
#define BLE_RST_WAIT_TIME 4  //3*20 60ms 
extern unsigned char temper;//0-14 代表17-30 度
extern unsigned char model;//模式
extern unsigned char flag;
unsigned char  BLEReceive[2];
extern unsigned char BLEReceive_buff[2];
extern unsigned char  send_data[6];
extern unsigned char INFO[];
extern unsigned char TV_sta;//开关机状态
// Slave_adrr 0x43
extern unsigned char chnel;//频道
extern bit full;
unsigned char time_flag=0;
bit read_buff=0;
extern unsigned char chnel;//频道
unsigned char time_start=0;
bit  sure=0;//确定按键
unsigned char TV_chnel_arr[2]={'-','-'};//遥控控制的频道
unsigned char TV_chnel_flag=0;
bit BLE_RST_Flag_bit=0;
unsigned char BLE_RST_Flag=1;//蓝牙复位，避免使用延时，当为零时复位开始，
void Delay25ms()		//@12.000MHz
{
	unsigned char i, j, k;

	_nop_();
	_nop_();
	i = 2;
	j = 50;
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
	WriteChinese16x16(0,0,"模拟电视");
	WriteChinese16x16(1,0,"当前频道");
	WriteChinese16x16(2,0,"选择频道");
	WriteStr_8x16(1,64,":");  
	WriteStr_8x16(0,64,":");
	WriteStr_8x16(2,64,":");  
	
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

void fun_chnel(unsigned char temp)
{
//	unsigned char dat=0;
//	dat=TV_chnel_flag*8;
	if(TV_sta)//开机有效
	{
		//开启计时器 
		time_start=1;
			if(TV_chnel_flag>1)
			{
				TV_chnel_flag=0;
				TV_chnel_arr[0]='-';
				TV_chnel_arr[1]='-';
				WriteStr_8x16(2,72,"   ");
			}

				TV_chnel_arr[TV_chnel_flag]=temp-'d';//
			PrintNum_8x16(2,(TV_chnel_flag+8)*9,TV_chnel_arr[TV_chnel_flag]);	
				TV_chnel_flag++;
	

	}
}
void sure_(void)
{
	unsigned char dat=0;
				if(TV_chnel_arr[0]!='-' && TV_chnel_arr[1]!='-')
					dat=TV_chnel_arr[0]*10+TV_chnel_arr[1];
			  if(TV_chnel_arr[0]!='-' && TV_chnel_arr[1]=='-' )
					dat=TV_chnel_arr[0];
	
			
			if(dat>99)dat=99;
			INFO[4]=TV_sta;
			INFO[5]=dat;
			chnel=dat;
			

			
			chnel_Change();//显示和红外发射
			SendString_Size(INFO,7);
			
			BLE_reset();
		WriteStr_8x16(2,72,"   ");
		TV_chnel_arr[0]='-';
		TV_chnel_arr[1]='-';
		TV_chnel_flag=0;
}
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
					open_open();
						break;
				case 'b':					
					open_close();
				break;
				
				case 'c':
					chnel_add();
				break;
				
				case 'd':
				chnel_mul();
					
//					WriteStr_8x16(2,0,"MODE:");
//					PrintNum_8x16(2,45,model);				
				break;
				
				case 'e':   //1
					fun_chnel('e');
				break;
				
				case 'f':   //2
					fun_chnel('f');
				break;	
				
				case 'g':   //3
						fun_chnel('g');
				break;
				
				case 'h':   //4
						fun_chnel('h');
				break;
				
				case 'i':   //5
						fun_chnel('i');
				break;
				
				case 'j':    //6
					fun_chnel('j');
				break;
				
				case 'k':   //7
						fun_chnel('k');
				break;
				
				case 'l':   //8
					//	fun_chnel('l');
						sure=1;
				break;
				default:
					if(BLEReceive[1]>'m')
					{
						TV_chnel_arr[0]=(BLEReceive[1]-'m')/10;
						TV_chnel_arr[1]=(BLEReceive[1]-'m')%10;
						sure_();
					}
							
			}
			BLEReceive[1]=0x00;

		}//
		
		
		if(sure==1)// 选择频道后 按下确认按键
		{
			sure=0;
			sure_();
			
		}
		if(time_start>50)//时间到了没有按下确定，取消
		{
				time_start=0;
				TV_chnel_flag=0;
				TV_chnel_arr[0]='-';
				TV_chnel_arr[1]='-';
			WriteStr_8x16(2,72,"   ");
		}
		PrintNum_8x16(3,0,sum);
			if(time_flag==1)
			{
				time_flag=0;
					sum++;
					Send_info();
					
				if(sum>9)sum=0;
			//	Send_info();
				
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
	if(mun%5==0)  //100MS
	{
	
		read_buff=1;
		if(time_start)
		{
			time_start++;
		}
		
	}
	if(mun==70)
	{
		mun=0;
		time_flag=1;
	}
}

