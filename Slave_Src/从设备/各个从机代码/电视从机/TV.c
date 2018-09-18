#include "TV.h"
#include "OLED.h"
#include "IR.h"
#include "usart.h"
unsigned char TV_sta=0 ;//开关机状态
// Slave_adrr 0x43
unsigned char chnel=0;//频道
#define END_dat 0xaa
extern unsigned char BLE_RST_Flag;
extern bit BLE_RST_Flag_bit;
extern void Delay25ms();	//@12.000MHz
extern void Delay1ms();	//@12.000MHz
/*  
INFO[0] : ':' BLE固定前缀
INFO[1] : ':' BLE固定前缀
INFO[2] : 'S' 信息开始
INFO[3] : 识别号 ('B'==0x43)
INFO[4] ：开关机状态
INFO[5] ：频道
INFO[6] ：END_dat
INFO[7] ：不用
INFO[8] ：不用
INFO[9] ：不用
*/

unsigned char INFO[]={':',':','S',0x43,0x00,0x00,END_dat};
void Delay50ms()		//@12.000MHz
{
	unsigned char i, j, k;

	_nop_();
	_nop_();
	i = 3;
	j = 72;
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

void BLE_reset(void)
{
		//Delay50ms();//保证数据至少发出一帧
		BLE=0;  //至少拉低200us后拉高，或悬空
		Delay10ms();
		BLE=1;
}
void Send_info(void)
{
	//Delay25ms();//
	INFO[4]=TV_sta;
	INFO[5]=chnel;
	SendString_Size(INFO,7);
	//BLE_reset();
	BLE_RST_Flag=0;//标志开始
	BLE_RST_Flag_bit=1;
}

void open_open(void)
{
		
		TV_sta=0xff;
			if(TV_sta)
				WriteStr_8x16(0,80,"OPEN ");
			else
				WriteStr_8x16(0,80,"ClOSE");
					WriteStr_8x16(1,79,"   ");
			Timer1Init_IR();  // 定时器打开
			IRsend_order(0x55,~0x55,0x67,~0x67,0x44,~0x44);
				Timer1Init_main();// 定时器打开 分时复用
			Send_info();

}
void open_close(void)
{
		
		TV_sta=0x00;
			if(TV_sta)
				WriteStr_8x16(0,80,"OPEN ");
			else
				WriteStr_8x16(0,80,"ClOSE");
				WriteStr_8x16(1,79,"   ");
			Timer1Init_IR();  // 定时器打开
			IRsend_order(0x55,~0x55,0x67,~0x67,0x44,~0x44);
				Timer1Init_main();// 定时器打开 分时复用
			Send_info();
			
}

void chnel_add(void)
{
		if(TV_sta)
		{
			if(chnel<99)
				chnel++;
			else
				chnel=0;
		
			PrintNum_8x16(1,80,chnel/10);
			PrintNum_8x16(1,89,chnel%10);
			Timer1Init_IR();  // 定时器打开					

			IRsend_order(0x55,~0x56,0x67,~0x6f,0x48,~0x48);
			Timer1Init_main();// 定时器打开 分时复用
			Send_info();
		
}
}

void chnel_mul(void)
{
	if(TV_sta)
	{
		if(chnel==0)
			chnel=0;
		else
			chnel--;
		PrintNum_8x16(1,80,chnel/10);
		PrintNum_8x16(1,89,chnel%10);

		Timer1Init_IR();  // 定时器打开
		IRsend_order(0x55,~0x55,0x67,~0x67,0x4f,~0x4f);
		Timer1Init_main();// 定时器打开 分时复用
		Send_info();
	
	}
}

void chnel_Change(void)
{

	Timer1Init_IR();  // 定时器打开
	PrintNum_8x16(1,80,chnel/10);
	PrintNum_8x16(1,89,chnel%10);
	IRsend_order(0x55,~0x55,0x67,~0x67,0x4f,~0x4f);
		Timer1Init_main();// 定时器打开 分时复用
			Send_info();
//			BLE_reset();
}

