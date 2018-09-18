#include "IR.h"
 
sbit IR = P1^4;     // ���ⷢ�� led 


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
	TR0 = 1;   // �򿪺��ⷢ�䶨ʱ�� 0  ������ 38khz ����
	ET0=1;
	//IR = 0;
	TH1 = (65536-570)/256; 
	TL1 = (65536-570)%256;  // ��ʱ�� T1��ʱ 1650us 
	TR1 = 1;   // �򿪶�ʱ�� 1 
	while(!TF1);   // ��ʱ������
	TF1 = 0;   // ���ñ�־λ
	TR1 = 0;   // �رն�ʱ�� 1 
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
	TR0 = 1; //�򿪶�ʱ��0
		ET0=1;
	//IR = 0;
	TH1 = (65536-4550)/256; 
	TL1 = (65536-4550)%256; 
	TR1 = 1; //�򿪶�ʱ��1 ��ʱ
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

//��������
void Timer1Init(void)		//0΢��@12.000MHz
{
	AUXR &= 0xBF;		//��ʱ��ʱ��12Tģʽ
	TMOD &= 0x0F;		//���ö�ʱ��ģʽ
	TL1 = 0;		//���ö�ʱ��ֵ
	TH1 = 0;		//���ö�ʱ��ֵ
	TF1 = 0;		//���TF1��־
	TR1 = 0;		//��ʱ��1��ʼ��ʱ
}


//��ʱ�жϣ�38K Hz
void Timer0Init(void)		//13΢��@12.000MHz
{
	AUXR &= 0x7F;		//��ʱ��ʱ��12Tģʽ
	TMOD &= 0xF0;		//���ö�ʱ��ģʽ
	TL0 = 0xF3;		//���ö�ʱ��ֵ
	TH0 = 0xFF;		//���ö�ʱ��ֵ
	TF0 = 0;		//���TF0��־
	TR0 = 0;		//��ʱ��0��ʼ��ʱ
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
�����ʼ������
*/
 void  IR_init(void)
{
	Timer1Init();
	Timer0Init();
}

//�����ػ�����
void IR_On_oFF() 
{ 
			ET0=1;
			IRsend_order(0xb2,0x4d,0x1f,0xe0,0x48,0xb7);
			IRsend_order(0xb2,0x4d,0x1f,0xe0,0x48,0xb7);
			ET0=0;
		  P14=0;
} 

//���ⷢ�͸ı��¶ȣ�ģʽ����
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