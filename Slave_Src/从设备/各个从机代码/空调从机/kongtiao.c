#include "kongtiao.h"
#include "IR.h"
#include "Usart.h"
#include "OLED.h"
#define TEMPER_ADD17  17
// Slave_adrr   0x41
#define END_dat 0xaa
unsigned char temper=0;//0-14 代表17-30 度
unsigned char model=0;//模式
unsigned char poewr=0;//模式0 关机，255 开机
//空调温度数组                          17℃                                                             30℃
unsigned char code AIR_temper_arr[14]={0x00,0x10,0x30,0x20,0x60,0x70,0x50,0x40,0xc0,0xd0,0x90,0x80,0xa0,0xb0};
//空调模式数组       
unsigned char code AIR_Model_arr[4]={0x08,0x00,0x04,0x0c};
//                                    自动  制冷 抽湿 制热
// 发送给主机空调数据数组,S为空调识别码   温度  模式  开关机
unsigned char  send_data[6]={':',':','S',17,0,0};
extern unsigned char BLE_RST_Flag;
extern bit BLE_RST_Flag_bit;
/*        
INFO[0] : ':' BLE固定前缀
INFO[1] : ':' BLE固定前缀
INFO[2] : 'S' 信息开始
INFO[3] : 识别号 ('B'==0x42)
INFO[4] ：开关机状态
INFO[5] ：模式
INFO[6] ：温度
INFO[7] ：END_dat
INFO[8] ：不用
INFO[9] ：不用
*/
unsigned char INFO[]={':',':','S',0x41,0x00,0xff,0xff,END_dat};
extern void Delay1ms();
void Delay50ms()		//@12.000MHz
{
	unsigned char i, j, k;

	_nop_();
	_nop_();
	i = 3;
	j = 85;
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
	//	Delay50ms();//保证数据至少发出一帧
		BLE=0;  //至少拉低200us后拉高，或悬空
		Delay10ms();
		BLE=1;
}
//BLE向主机BLE发送信息
void Send_info(void)
{
	//Delay25ms();//
	INFO[4]=poewr;
	INFO[5]=model;
	INFO[6]=temper;
	SendString_Size(INFO,8);
//	BLE_reset();
	BLE_RST_Flag=0;//标志开始
	BLE_RST_Flag_bit=1;
}


void Display_Init(void)
{
		//	OLED_Init();//初始化显示
		WriteChinese16x16(0,0,"模拟空调");
		WriteStr_8x16(1,0," ");
		WriteChinese16x16(1,20,"℃");	
		//	PrintNum_8x16(1,0,(temper+17)/10);
		//	PrintNum_8x16(1,8,(temper+17)%10);
		//	WriteChinese16x16(2,48,"自动");
			WriteChinese16x16(0,0,"模式：");
	 WriteChinese16x16(2,50,"       ");

			WriteStr_8x16(0,80,"     ");
	
}
void AIR_On_on(void) 
{ 
			Timer1Init_IR();
			ET0=1;
			IRsend_order(0xb2,0x4d,0x1f,0xe0,0x08,0xf7);
			IRsend_order(0xb2,0x4d,0x1f,0xe0,0x08,0xf7);
//			IRsend_order(0xb2,0x4d,0x1f,0xe0,0x48,0xf7);
//			IRsend_order(0xb2,0x4d,0x1f,0xe0,0x48,0xf7);
			ET0=0;
		  P14=0;
			poewr=0xff;
	if(poewr)
	{
				WriteStr_8x16(0,80,"OPEN ");
				PrintNum_8x16(1,34,(temper+TEMPER_ADD17)/10);
			PrintNum_8x16(1,43,(temper+TEMPER_ADD17)%10);
			switch(model)
		{
			case 0: WriteChinese16x16(2,52,"自动");
				break;
			case 1:	WriteChinese16x16(2,52,"制冷");
				break;
			case 2: WriteChinese16x16(2,52,"抽湿");
				break;
			case 3: WriteChinese16x16(2,52,"制热");
				break;
			
		}
	}
	else
	{
		//OLED_Init();
	//	Delay10ms();
	WriteChinese16x16(0,0,"模拟空调");
	WriteStr_8x16(0,80,"CLOSE");
	WriteChinese16x16(1,0,"℃");	
	WriteStr_8x16(1,18,":");
	WriteChinese16x16(2,0,"模式 ：");
	WriteStr_8x16(2,34,":");
	WriteChinese16x16(1,27,"   ");	
	WriteChinese16x16(2,52,"  ");

	}
		Timer1Init_main();
		Send_info();
		

} 
void AIR_On_oFF(void) 
{ 
			Timer1Init_IR();
			ET0=1;
			IRsend_order(0xb2,0x4d,0x7b,0x84,0xe0,0x1f);
			IRsend_order(0xb2,0x4d,0x7b,0x84,0xe0,0x1f);
			ET0=0;
		  P14=0;
			poewr=0x00;
	if(poewr)
	{
				WriteStr_8x16(0,80,"OPEN ");
				PrintNum_8x16(1,34,(temper+TEMPER_ADD17)/10);
			PrintNum_8x16(1,43,(temper+TEMPER_ADD17)%10);
			switch(model)
		{
			case 0: WriteChinese16x16(2,52,"自动");
				break;
			case 1:	WriteChinese16x16(2,52,"制冷");
				break;
			case 2: WriteChinese16x16(2,52,"抽湿");
				break;
			case 3: WriteChinese16x16(2,52,"制热");
				break;
			
		}
	}
	else
	{
		//OLED_Init();
	//	Delay10ms();
	WriteChinese16x16(0,0,"模拟空调");
	WriteStr_8x16(0,80,"CLOSE");
	WriteChinese16x16(1,0,"℃");	
	WriteStr_8x16(1,18,":");
	WriteChinese16x16(2,0,"模式 ：");
	WriteStr_8x16(2,34,":");
	WriteChinese16x16(1,27,"   ");	
	WriteChinese16x16(2,52,"  ");

	}
		Timer1Init_main();
		Send_info();
	

} 
void AIR_temper_add(void)
{
	unsigned char temp;
	if(poewr)
	{
			Timer1Init_IR();
	if(temper>=13)
	temper=13;
	else 
	temper=temper+1;
	
	temp=AIR_temper_arr[temper];
	temp|=AIR_Model_arr[model];
	IR_temper_change(temp);
		Timer1Init_main();
	
		PrintNum_8x16(1,34,(temper+TEMPER_ADD17)/10);
		PrintNum_8x16(1,43,(temper+TEMPER_ADD17)%10);
	//	WriteChinese16x16(1,20,"℃");
		Send_info();
	
	}
}	

void AIR_temper_mul(void)
{
	unsigned char temp;
	if(poewr)
	{
		Timer1Init_IR();  // 定时器打开
		if(temper==0)
		temper=0;
		else
			temper=temper-1;
		temp=AIR_temper_arr[temper];//温度赋值
		temp|=AIR_Model_arr[model];//模式赋值
		IR_temper_change(temp);//红外发射
		Timer1Init_main();// 定时器打开 分时复用
		
		PrintNum_8x16(1,34,(temper+TEMPER_ADD17)/10);
		PrintNum_8x16(1,43,(temper+TEMPER_ADD17)%10);
			//WriteChinese16x16(1,20,"℃");
		
		Send_info();
	
		
	}
	
	

//		else
//	{
//		//Display_Init();
//			
//	}

}

void AIR_temper_Set(unsigned char dat )
{
	unsigned char temp;
	if(poewr)
	{
		Timer1Init_IR();  // 定时器打开
		temper=dat;
		temp=AIR_temper_arr[temper];//温度赋值
		temp|=AIR_Model_arr[model];//模式赋值
		
		IR_temper_change(temp);//红外发射
		Timer1Init_main();// 定时器打开 分时复用
		
		PrintNum_8x16(1,34,(temper+TEMPER_ADD17)/10);
		PrintNum_8x16(1,43,(temper+TEMPER_ADD17)%10);
			//WriteChinese16x16(1,20,"℃");
		
		Send_info();

		
	}
}
void AIR_Model(void)
{
unsigned char temp;
	if(poewr)
	{
		Timer1Init_IR();
		model=model+1;
		if(model>3)
		model=0;
		temp=AIR_temper_arr[temper];
		temp|=AIR_Model_arr[model];
		IR_temper_change(temp);
		Timer1Init_main();
		//WriteStr_8x16(2,0,"MODE:");
		switch(model)
		{
			case 0: WriteChinese16x16(2,52,"自动");
				break;
			case 1:	WriteChinese16x16(2,52,"制冷");
				break;
			case 2: WriteChinese16x16(2,52,"抽湿");
				break;
			case 3: WriteChinese16x16(2,52,"制热");
				break;
			
		}
			Send_info();

	
	}
//	else
//	{
//		//Display_Init();
//			
//	}		
}


void Model_Change(unsigned char temp)
{
		if(poewr)
	{
		Timer1Init_IR();
		model=temp;
		temp=AIR_temper_arr[temper];
		temp|=AIR_Model_arr[model];
		IR_temper_change(temp);
		Timer1Init_main();
		//WriteStr_8x16(2,0,"MODE:");
		switch(model)
		{
			case 0: WriteChinese16x16(2,52,"自动");
				break;
			case 1:	WriteChinese16x16(2,52,"制冷");
				break;
			case 2: WriteChinese16x16(2,52,"抽湿");
				break;
			case 3: WriteChinese16x16(2,52,"制热");
				break;
			
		}
			Send_info();

	
	}
}

