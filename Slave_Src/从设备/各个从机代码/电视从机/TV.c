#include "TV.h"
#include "OLED.h"
#include "IR.h"
#include "usart.h"
unsigned char TV_sta=0 ;//���ػ�״̬
// Slave_adrr 0x43
unsigned char chnel=0;//Ƶ��
#define END_dat 0xaa
extern unsigned char BLE_RST_Flag;
extern bit BLE_RST_Flag_bit;
extern void Delay25ms();	//@12.000MHz
extern void Delay1ms();	//@12.000MHz
/*  
INFO[0] : ':' BLE�̶�ǰ׺
INFO[1] : ':' BLE�̶�ǰ׺
INFO[2] : 'S' ��Ϣ��ʼ
INFO[3] : ʶ��� ('B'==0x43)
INFO[4] �����ػ�״̬
INFO[5] ��Ƶ��
INFO[6] ��END_dat
INFO[7] ������
INFO[8] ������
INFO[9] ������
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
		//Delay50ms();//��֤�������ٷ���һ֡
		BLE=0;  //��������200us�����ߣ�������
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
	BLE_RST_Flag=0;//��־��ʼ
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
			Timer1Init_IR();  // ��ʱ����
			IRsend_order(0x55,~0x55,0x67,~0x67,0x44,~0x44);
				Timer1Init_main();// ��ʱ���� ��ʱ����
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
			Timer1Init_IR();  // ��ʱ����
			IRsend_order(0x55,~0x55,0x67,~0x67,0x44,~0x44);
				Timer1Init_main();// ��ʱ���� ��ʱ����
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
			Timer1Init_IR();  // ��ʱ����					

			IRsend_order(0x55,~0x56,0x67,~0x6f,0x48,~0x48);
			Timer1Init_main();// ��ʱ���� ��ʱ����
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

		Timer1Init_IR();  // ��ʱ����
		IRsend_order(0x55,~0x55,0x67,~0x67,0x4f,~0x4f);
		Timer1Init_main();// ��ʱ���� ��ʱ����
		Send_info();
	
	}
}

void chnel_Change(void)
{

	Timer1Init_IR();  // ��ʱ����
	PrintNum_8x16(1,80,chnel/10);
	PrintNum_8x16(1,89,chnel%10);
	IRsend_order(0x55,~0x55,0x67,~0x67,0x4f,~0x4f);
		Timer1Init_main();// ��ʱ���� ��ʱ����
			Send_info();
//			BLE_reset();
}

