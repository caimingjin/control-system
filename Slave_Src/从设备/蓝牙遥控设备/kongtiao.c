#include "kongtiao.h"

unsigned char temper=0;//0-14 代表17-30 度
unsigned char model=0;//模式
//空调温度数组                     17℃                                                             30℃
unsigned char code AIR_temper_arr[14]={0x00,0x10,0x30,0x20,0x60,0x70,0x50,0x40,0xc0,0xd0,0x90,0x80,0xa0,0xb0};
//空调模式数组       
unsigned char code AIR_Model_arr[4]={0x08,0x00,0x04,0x0c};

void AIR_temper_add(void)
{
	unsigned char temp;

	if(temper>13)
	temper=13;
	else 
	temper++;
	
	temp=AIR_temper_arr[temper];
	temp|=model<<2;
	temp&=0xfc;
	IR_temper_change(temp);

}	

void AIR_temper_mul(void)
{
	unsigned char temp;

	if(temper==0)
	temper=0;
	else
	temper--;
	temp=AIR_temper_arr[temper];
	temp|=model<<2;
	temp&=0xfc;
	IR_temper_change(temp);
}
void AIR_Model(void)
{
unsigned char temp;
	model++;
	if(model>3)
	model=0;
	temp=AIR_temper_arr[temper];
	temp|=AIR_Model_arr[model];
	IR_temper_change(temp);
}