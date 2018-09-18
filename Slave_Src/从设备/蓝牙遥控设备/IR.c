#include "IR.h"
 
sbit IR = P1^4;     // 红外发射 led 


void send0() 
{ 
	TR0 = 1;  
	ET0=1;
	TH1 = (65536-570)/256; 
	TL1 = (65536-570)%256; 
	TR1 = 1; 
	while(!TF1); 
	TF1 = 0; 
	TR1 = 0; 
	ET0=0;
	TR0 = 0; 
	IR = 1; 
	TH1 = (65536-570)/256; 
	TL1 = (65536-570)%256; 
	TR1 = 1; 
	while(!TF1); 
	TF1 = 0; 
	TR1 = 0; 
	IR = 1; 

} 
void send1() 
{ 
	TR0 = 1;   // 打开红外发射定时器 0  ，发射 38khz 红外
	ET0=1;
	//IR = 0;
	TH1 = (65536-570)/256; 
	TL1 = (65536-570)%256;  // 定时器 T1定时 1650us 
	TR1 = 1;   // 打开定时器 1 
	while(!TF1);   // 定时结束后
	TF1 = 0;   // 重置标志位
	TR1 = 0;   // 关闭定时器 1 
	TR0 = 0; 
		ET0=0;
	IR = 1; 
	TH1 = (65536-1670)/256; 
	TL1 = (65536-1670)%256; 
	TR1 = 1; 
	while(!TF1); 
	TF1 = 0; 
	TR1 = 0; 
	IR = 1; 
} 
void IRsend_byte(uchar num) 
{ 
	uchar i; 
	for(i=0;i<8;i++) 
	{ 
		if(num&0x80) 
			send1(); 
		else 
			send0(); 
		
			num<<=1; 
	} 
} 
/*

*/
void IRstart() 
{ 
	TR0 = 1; //打开定时器0
		ET0=1;
	//IR = 0;
	TH1 = (65536-4550)/256; 
	TL1 = (65536-4550)%256; 
	TR1 = 1; //打开定时器1 计时
	while(!TF1); 
	TF1 = 0; 
	TR1 = 0; 
	TR0 = 0;
	ET0=0;	
	IR = 1; 
	TH1 = (65536-4550)/256; 
	TL1 = (65536-4550)%256; 
	TR1 = 1; 
	while(!TF1); 
	TF1 = 0; 
	TR1 = 0; 
	IR = 1; 
} 

void Middle(unsigned int L, H) 
{ 
	TR0 = 1;  
//	IR = 0;
	TH1 = (65536-L)/256; 
	TL1 = (65536-L)%256; 
	TR1 = 1; 
	while(!TF1); 
	TF1 = 0; 
	TR1 = 0; 
	TR0 = 0; 
	ET0=0;
	IR = 1; 
	TH1 = (65536-H)/256; 
	TL1 = (65536-H)%256; 
	TR1 = 1; 
	while(!TF1); 
	TF1 = 0; 
	TR1 = 0; 
} 
void IRstop() 
{ 
	TR0 = 1; 
	ET0=1;
	//IR = 0; 
	TH1 = (65536-580)/256; 
	TL1 = (65536-580)%256; 
	TR1 = 1; 
	while(!TF1); 
	TF1 = 0; 
	TR1 = 0; 
	TR0 = 0; 
	IR = 1; 
	TH1 = (65536-5100)/256; 
	TL1 = (65536-5100)%256; 
	TR1 = 1; 
	while(!TF1); 
	TF1 = 0; 
	TR1 = 0; 
	IR = 0; 

} 


//void timer_init() 
//{ 
//	TMOD = 0x12; 
//	TH0  = 0xf6; 
//	TL0  = 0xf6; 
//	EA   = 1; 
//	ET0  = 1; 
//} 

//用来计数
void Timer1Init(void)		//0微秒@12.000MHz
{
	AUXR &= 0xBF;		//定时器时钟12T模式
	TMOD &= 0x0F;		//设置定时器模式
	TL1 = 0;		//设置定时初值
	TH1 = 0;		//设置定时初值
	TF1 = 0;		//清除TF1标志
	TR1 = 0;		//定时器1开始计时
}


//定时中断，38K Hz
void Timer0Init(void)		//13微秒@12.000MHz
{
	AUXR &= 0x7F;		//定时器时钟12T模式
	TMOD &= 0xF0;		//设置定时器模式
	TL0 = 0xF3;		//设置定时初值
	TH0 = 0xFF;		//设置定时初值
	TF0 = 0;		//清除TF0标志
	TR0 = 0;		//定时器0开始计时
	EA=1;
	ET0=1;
}



/*

*/
void IRsend_order(uchar a,uchar b,uchar c,uchar d,uchar e,uchar f) 
{ 
IRstart(); 
	IRsend_byte(a); 
	IRsend_byte(b); 
	IRsend_byte(c); 
	IRsend_byte(d); 
	IRsend_byte(e); 
	IRsend_byte(f); 
	IRstop(); 
} 
//void IRsend_order_no_S(uchar a,b,c,d,e,f) 
//{ 
//	IRsend_byte(a); 
//	IRsend_byte(b); 
//	IRsend_byte(c); 
//	IRsend_byte(d); 
//	IRsend_byte(e); 
//	IRsend_byte(f); 
//	IRstop(); 
//} 

/*
红外初始化函数
*/
 void  IR_init(void)
{
	Timer1Init();
	Timer0Init();
}

//开机关机数据
void IR_On_oFF() 
{ 
			ET0=1;
			IRsend_order(0xb2,0x4d,0x1f,0xe0,0x48,0xb7);
			IRsend_order(0xb2,0x4d,0x1f,0xe0,0x48,0xb7);
			ET0=0;
		  P14=0;
} 

//红外发送改变温度，模式数据
void IR_temper_change(unsigned char dat) 
{ 
			ET0=1;
			IRsend_order(0xb2,0x4d,0x1f,0xe0,dat,~dat);
			IRsend_order(0xb2,0x4d,0x1f,0xe0,dat,~dat);
			ET0=0;
		  P14=0;
}
void timer0()interrupt 1 
{ 
	IR = ~IR; 
} 