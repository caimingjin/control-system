#include "kongtiao.h"
#include "IR.h"
#include "Usart.h"
unsigned char temper=0;//0-14 ����17-30 ��
unsigned char model=0;//ģʽ
unsigned char poewr=0;//ģʽ0 �ػ���255 ����
//�յ��¶�����                           17��                                                             30��
unsigned char code AIR_temper_arr[14]={0x00,0x10,0x30,0x20,0x60,0x70,0x50,0x40,0xc0,0xd0,0x90,0x80,0xa0,0xb0};
//�յ�ģʽ����       
unsigned char code AIR_Model_arr[4]={0x08,0x00,0x04,0x0c};
// ���͸������յ���������,SΪ�յ�ʶ����   �¶�  ģʽ  ���ػ�
unsigned char  send_data[6]={':',':','S',17,0,0};
extern void Delay1ms();
void BLE_reset(void)
{
		Delay25ms();//��֤�������ٷ���һ֡
		BLE=0;  //��������200us�����ߣ�������
		Delay1ms();
		BLE=1;
}
void AIR_On_oFF(void) 
{ 
			Timer1Init_IR();
			ET0=1;
			IRsend_order(0xb2,0x4d,0x1f,0xe0,0x48,0xb7);
			IRsend_order(0xb2,0x4d,0x1f,0xe0,0x48,0xb7);
			ET0=0;
		  P14=0;
			poewr=~poewr;
		send_data[5]=poewr;
		Timer1Init_main();
		SendString(send_data);
		BLE_reset();

} 
void AIR_temper_add(void)
{
	unsigned char temp;
		Timer1Init_IR();
	if(temper==13)
	temper=13;
	else 
	temper=temper+1;
	
	temp=AIR_temper_arr[temper];
	temp|=AIR_Model_arr[model];
	IR_temper_change(temp);
		send_data[3]=temp+17;
Timer1Init_main();
		SendString(send_data);
		BLE_reset();
}	

void AIR_temper_mul(void)
{
	unsigned char temp;
		Timer1Init_IR();  // ��ʱ����
	if(temper==0)
	temper=0;
	else
		temper=temper-1;
	temp=AIR_temper_arr[temper];//�¶ȸ�ֵ
	temp|=AIR_Model_arr[model];//ģʽ��ֵ
	IR_temper_change(temp);//���ⷢ��
			send_data[3]=temp+17;
	Timer1Init_main();// ��ʱ���� ��ʱ����
		SendString(send_data);
		BLE_reset();

}
void AIR_Model(void)
{
unsigned char temp;
			Timer1Init_IR();
	model=model+1;
	if(model>3)
	model=0;
	temp=AIR_temper_arr[temper];
	temp|=AIR_Model_arr[model];
	IR_temper_change(temp);
			send_data[4]=temp+'0';
	Timer1Init_main();
		SendString(send_data);
		BLE_reset();
}