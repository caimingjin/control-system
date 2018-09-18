#include<reg52.h> 
typedef unsigned intuint ; 
typedef unsigned char uchar ; 
sbit IR = P2^4;     // ���ⷢ�� led 
sbit S2 = P3^0;     // ��������
sbit S3 = P3^1;     // ��������
voidDelay_ms(uint i) 
{ 
	uchar j; 
	for(;i>0;i--) 
	for(j=114;j>0;j--);  
} 
void send0() 
{ 
	TR0 = 1; 
	TH1 = (65536-516)/256; 
	TL1 = (65536-516)%256; 
	TR1 = 1; 
	while(!TF1); 
	TF1 = 0; 
	TR1 = 0; 
	TR0 = 0; 
	IR = 1; 
	TH1 = (65536-516)/256; 
	TL1 = (65536-516)%256; 
	TR1 = 1; 
	while(!TF1); 
	TF1 = 0; 
	TR1 = 0; 
} 
void send1() 
{ 
	TR0 = 1;   // �򿪺��ⷢ�䶨ʱ�� 0  ������ 38khz ����
	TH1 = (65536-1552)/256; 
	TL1 = (65536-1552)%256;  // ��ʱ�� T1��ʱ 1650us 
	TR1 = 1;   // �򿪶�ʱ�� 1 
	while(!TF1);   // ��ʱ������
	TF1 = 0;   // ���ñ�־λ
	TR1 = 0;   // �رն�ʱ�� 1 
	TR0 = 0; 
	IR = 1; 
	TH1 = (65536-516)/256; 
	TL1 = (65536-516)%256; 
	TR1 = 1; 
	while(!TF1); 
	TF1 = 0; 
	TR1 = 0; 
	IR = 1; 
} 
voidIRsend_byte(ucharnum) 
{ 
	uchar i; 
	for(i=0;i<8;i++) 
	{ 
		if(num&0x01) 
			send1(); 
		else 
			send0(); 
			num>>=1; 
	} 
} 
voidIRstart() 
{ 
	TR0 = 1; 
	TH1 = (65536-8294)/256; 
	TL1 = (65536-8294)%256; 
	TR1 = 1; 
	while(!TF1); 
	TF1 = 0; 
	TR1 = 0; 
	TR0 = 0; 
	IR = 1; 
	TH1 = (65536-4147)/256; 
	TL1 = (65536-4147)%256; 
	TR1 = 1; 
	while(!TF1); 
	TF1 = 0; 
	TR1 = 0; 
	IR = 1; 
} 
voidIRstop() 
{ 
	TR0 = 1; 
	TH1 = (65536-500)/256; 
	TL1 = (65536-500)%256; 
	TR1 = 1; 
	while(!TF1); 
	TF1 = 0; 
	TR1 = 0; 
	TR0 = 0; 
	IR = 1; 
	TH1 = (65536-500)/256; 
	TL1 = (65536-500)%256; 
	TR1 = 1; 
	while(!TF1); 
	TF1 = 0; 
	TR1 = 0; 
	IR = 1; 
} 
voidtimer_init() 
{ 
	TMOD = 0x12; 
	TH0  = 0xf6; 
	TL0  = 0xf6; 
	EA   = 1; 
	ET0  = 1; 
} 
voidIRsend_order(uchara,b,c,d) 
{ 
	IRstart(); 
	IRsend_byte(a); 
	IRsend_byte(b); 
	IRsend_byte(c); 
	IRsend_byte(d); 
	IRstop(); 
} 
void main() 
{ 
	timer_init(); 
while(1) 
{ 
	if(!S2) 
	{ 
		Delay_ms(5); 
		if(!S2) 
		{ 
			while(!S2); 
			IRsend_order(0x10,0xe7,0x00,0xff); 
		} 
	} 
	else if(!S3) 
	{ 
		Delay_ms(5); 
		if(!S3) 
		{ 
			while(!S2); 
			IRsend_order(0x10,0xe7,0x01,0xfe); 
		} 
	} 
} 
} 
void timer0()interrupt 1 
{ 
	IR = ~IR; 
} 