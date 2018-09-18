#include "kongtiao.h"
#include "IR.h"
#include "Usart.h"
unsigned char temper=0;//0-14 代表17-30 度
unsigned char model=0;//模式
unsigned char poewr=0;//模式0 关机，255 开机
//空调温度数组                           17℃                                                             30℃
unsigned char code AIR_temper_arr[14]={0x00,0x10,0x30,0x20,0x60,0x70,0x50,0x40,0xc0,0xd0,0x90,0x80,0xa0,0xb0};
//空调模式数组       
unsigned char code AIR_Model_arr[4]={0x08,0x00,0x04,0x0c};
// 发送给主机空调数据数组,S为空调识别码   温度  模式  开关机
unsigned char  send_data[6]={':',':','S',17,0,0};
extern void Delay1ms();
void BLE_reset(void)
{
		Delay25ms();//保证数据至少发出一帧
		BLE=0;  //至少拉低200us后拉高，或悬空
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
		Timer1Init_IR();  // 定时器打开
	if(temper==0)
	temper=0;
	else
		temper=temper-1;
	temp=AIR_temper_arr[temper];//温度赋值
	temp|=AIR_Model_arr[model];//模式赋值
	IR_temper_change(temp);//红外发射
			send_data[3]=temp+17;
	Timer1Init_main();// 定时器打开 分时复用
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