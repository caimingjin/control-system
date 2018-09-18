#include "kongtiao.h"
#include "IR.h"
#include "Usart.h"
#include "OLED.h"
#define TEMPER_ADD17  17
// Slave_adrr   0x41
#define END_dat 0xaa
unsigned char temper=0;//0-14 ����17-30 ��
unsigned char model=0;//ģʽ
unsigned char poewr=0;//ģʽ0 �ػ���255 ����
//�յ��¶�����                          17��                                                             30��
unsigned char code AIR_temper_arr[14]={0x00,0x10,0x30,0x20,0x60,0x70,0x50,0x40,0xc0,0xd0,0x90,0x80,0xa0,0xb0};
//�յ�ģʽ����       
unsigned char code AIR_Model_arr[4]={0x08,0x00,0x04,0x0c};
//                                    �Զ�  ���� ��ʪ ����
// ���͸������յ���������,SΪ�յ�ʶ����   �¶�  ģʽ  ���ػ�
unsigned char  send_data[6]={':',':','S',17,0,0};
extern unsigned char BLE_RST_Flag;
extern bit BLE_RST_Flag_bit;
/*        
INFO[0] : ':' BLE�̶�ǰ׺
INFO[1] : ':' BLE�̶�ǰ׺
INFO[2] : 'S' ��Ϣ��ʼ
INFO[3] : ʶ��� ('B'==0x42)
INFO[4] �����ػ�״̬
INFO[5] ��ģʽ
INFO[6] ���¶�
INFO[7] ��END_dat
INFO[8] ������
INFO[9] ������
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
	//	Delay50ms();//��֤�������ٷ���һ֡
		BLE=0;  //��������200us�����ߣ�������
		Delay10ms();
		BLE=1;
}
//BLE������BLE������Ϣ
void Send_info(void)
{
	//Delay25ms();//
	INFO[4]=poewr;
	INFO[5]=model;
	INFO[6]=temper;
	SendString_Size(INFO,8);
//	BLE_reset();
	BLE_RST_Flag=0;//��־��ʼ
	BLE_RST_Flag_bit=1;
}


void Display_Init(void)
{
		//	OLED_Init();//��ʼ����ʾ
		WriteChinese16x16(0,0,"ģ��յ�");
		WriteStr_8x16(1,0," ");
		WriteChinese16x16(1,20,"��");	
		//	PrintNum_8x16(1,0,(temper+17)/10);
		//	PrintNum_8x16(1,8,(temper+17)%10);
		//	WriteChinese16x16(2,48,"�Զ�");
			WriteChinese16x16(0,0,"ģʽ��");
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
			case 0: WriteChinese16x16(2,52,"�Զ�");
				break;
			case 1:	WriteChinese16x16(2,52,"����");
				break;
			case 2: WriteChinese16x16(2,52,"��ʪ");
				break;
			case 3: WriteChinese16x16(2,52,"����");
				break;
			
		}
	}
	else
	{
		//OLED_Init();
	//	Delay10ms();
	WriteChinese16x16(0,0,"ģ��յ�");
	WriteStr_8x16(0,80,"CLOSE");
	WriteChinese16x16(1,0,"��");	
	WriteStr_8x16(1,18,":");
	WriteChinese16x16(2,0,"ģʽ ��");
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
			case 0: WriteChinese16x16(2,52,"�Զ�");
				break;
			case 1:	WriteChinese16x16(2,52,"����");
				break;
			case 2: WriteChinese16x16(2,52,"��ʪ");
				break;
			case 3: WriteChinese16x16(2,52,"����");
				break;
			
		}
	}
	else
	{
		//OLED_Init();
	//	Delay10ms();
	WriteChinese16x16(0,0,"ģ��յ�");
	WriteStr_8x16(0,80,"CLOSE");
	WriteChinese16x16(1,0,"��");	
	WriteStr_8x16(1,18,":");
	WriteChinese16x16(2,0,"ģʽ ��");
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
	//	WriteChinese16x16(1,20,"��");
		Send_info();
	
	}
}	

void AIR_temper_mul(void)
{
	unsigned char temp;
	if(poewr)
	{
		Timer1Init_IR();  // ��ʱ����
		if(temper==0)
		temper=0;
		else
			temper=temper-1;
		temp=AIR_temper_arr[temper];//�¶ȸ�ֵ
		temp|=AIR_Model_arr[model];//ģʽ��ֵ
		IR_temper_change(temp);//���ⷢ��
		Timer1Init_main();// ��ʱ���� ��ʱ����
		
		PrintNum_8x16(1,34,(temper+TEMPER_ADD17)/10);
		PrintNum_8x16(1,43,(temper+TEMPER_ADD17)%10);
			//WriteChinese16x16(1,20,"��");
		
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
		Timer1Init_IR();  // ��ʱ����
		temper=dat;
		temp=AIR_temper_arr[temper];//�¶ȸ�ֵ
		temp|=AIR_Model_arr[model];//ģʽ��ֵ
		
		IR_temper_change(temp);//���ⷢ��
		Timer1Init_main();// ��ʱ���� ��ʱ����
		
		PrintNum_8x16(1,34,(temper+TEMPER_ADD17)/10);
		PrintNum_8x16(1,43,(temper+TEMPER_ADD17)%10);
			//WriteChinese16x16(1,20,"��");
		
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
			case 0: WriteChinese16x16(2,52,"�Զ�");
				break;
			case 1:	WriteChinese16x16(2,52,"����");
				break;
			case 2: WriteChinese16x16(2,52,"��ʪ");
				break;
			case 3: WriteChinese16x16(2,52,"����");
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
			case 0: WriteChinese16x16(2,52,"�Զ�");
				break;
			case 1:	WriteChinese16x16(2,52,"����");
				break;
			case 2: WriteChinese16x16(2,52,"��ʪ");
				break;
			case 3: WriteChinese16x16(2,52,"����");
				break;
			
		}
			Send_info();

	
	}
}

