#include "stc15.h"
#include "usart.h"
#include "OLED.h"
#define Slave_adrr 0x41 //从设备识别地址A 0x41  B 0x42  C 0x43 D 0x44

/*********************************************************************
【函 数 名】：UART1_Get_char----串口1接收一个字节函数
【参数说明】；无参
【返 回 值】：接收到的字节
【简    例】：uchar d;
              d = UART1_Get_char();
              UART1_Put_Char(d); -----输出接收到的字节
*********************************************************************/
unsigned char UART1_Get_char() 
{
	while(RI == 0);  //RI = 1;接收结束
	RI = 0;
	return(SBUF);	
}
/*********************************************************************
【函 数 名】：UART1_Get_String----串口1接收字符串函数
【参数说明】；Pst：存放接收到的字符串的数组名
              Length：字符串长度
【简    例】：uchar d[4] = {0};
              UART1_Get_String(d,4);
              UART1_Put_String(d,4);----输出接收到的字符串 
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
* ChanmelFunction：频道和功能数据
*最大255 足够用
*/
//unsigned char  ChanmelFunction;

/*
* BLEReceive：蓝牙接受数据
*前八位地址识别，后八位数据位
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
	P44=0;//复位按键
	Delay100ms();
	P44=1;//复位按键
	P15=1;
	P14=0;
	OLED_Init();
	Usart_init();
	WriteChinese16x16(0,32,"模拟电视");
}

void main()
{
	system_init();
	while(1)
	{
		if(BLEReceive[0]==Slave_adrr)//判断接收到数据前八位是否为从机地址
		{
			BLEReceive[0]=0x00;//清除
			switch(BLEReceive[1])//判断后八位对应功能
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

