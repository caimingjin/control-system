#include "sys.h"
#include "gpio.h"
#include "Slave.h"
#include "delay.h"
#include "usart.h"
#include "UART3.h"
#include "SYN7318.h"
#include "gizwits_product.h"
#include "gizwits_protocol.h"
#include "bsp_GeneralTim.h"
#include "bsp_led.h"
#include "bsp_key.h"
#include "bsp_ili9341_lcd.h"
#define DICT_ 0x03  //识别的词典
#define DICT_Play 0x00  //播放的词典
//char D_MP3_1[] = {"D:\\Mp3\\Wifi_首次开机.mp3"};
char  D_MP3_2[] = {"D:\\Mp3\\Wifi_我在这.mp3"}; 
//char TF_MP3_1[] = {"E:\\1-歌曲\\一条大鱼不见了.mp3"}; 

void Gitwits_Init(void)
{
	
	GENERAL_TIM_Init(); //定时一毫秒
	uart1_init(9600);
	memset((uint8_t*)&currentDataPoint,0,sizeof(dataPoint_t));//设备状态结构体初始化
	gizwitsInit();
	userInit();
	userHandle();
 // gizwitsHandle((dataPoint_t*)&currentDataPoint);	//上报数据
}	
int main(void)
{ 

	//LED_GPIO_Config();
 // Key_GPIO_Config();
	ILI9341_Init ();
	//ILI9341_GramScan ( 6 );//液晶屏显示方向
	ILI9341_Clear(0,0,LCD_X_LENGTH,LCD_Y_LENGTH);	/* 清屏，显示全黑 */ 	
	LCD_SetColors(WHITE,BLACK);

	Gitwits_Init();
  delay_init();		  //初始化延时函数
  uart2_init(115200);  //各个从机串口以及从从机接收数据
  uart3_init(115200);//语音识别串口
  IDENTIFY_INIT();//设置传输距离以及识别等级
	SYN_BW_init();   //语音模块忙碌灯引脚
  BLE_Reset_Init();//蓝牙软件复位引脚
 // SYN_RCE_init();   //语音模块复位引脚
	LCD_Test();
	while(1)
	{
          

if(GPIO_ReadInputDataBit(SYN_BW_GPIO,SYN_BW_IO)==0 )//0空闲	 1灯亮是忙碌
{	
			Three_One(DICT_, 0x07, 0x02, D_MP3_2);//唤醒名为百灵管家,唤醒成功后播放“我在这”,之后可以识别0x00词典中的词条 
				//while(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_12)==1 );		
		if(SYN_Wake_Rsurt==1)
			{
					SYN_Wake_Rsurt=0;
					SYN_TTS("处理中");
					Execute_Result();//对从设备控制
			//		Dict_TTS_OK(DICT_Play);	//播放识别的结果				
				//	if()
						
//					else
//						SYN_TTS("错误的命令");
								//处理识别到的结果				
			}
	  else
			{
				Dict_TTS(DICT_Play); 
			}
				//结合Dict_TTS函数把识别结果播报出来
}

	 }
		
	
 }

//**************************************************************************************************


