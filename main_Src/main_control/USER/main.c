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
#define DICT_ 0x03  //ʶ��Ĵʵ�
#define DICT_Play 0x00  //���ŵĴʵ�
//char D_MP3_1[] = {"D:\\Mp3\\Wifi_�״ο���.mp3"};
char  D_MP3_2[] = {"D:\\Mp3\\Wifi_������.mp3"}; 
//char TF_MP3_1[] = {"E:\\1-����\\һ�����㲻����.mp3"}; 

void Gitwits_Init(void)
{
	
	GENERAL_TIM_Init(); //��ʱһ����
	uart1_init(9600);
	memset((uint8_t*)&currentDataPoint,0,sizeof(dataPoint_t));//�豸״̬�ṹ���ʼ��
	gizwitsInit();
	userInit();
	userHandle();
 // gizwitsHandle((dataPoint_t*)&currentDataPoint);	//�ϱ�����
}	
int main(void)
{ 

	//LED_GPIO_Config();
 // Key_GPIO_Config();
	ILI9341_Init ();
	//ILI9341_GramScan ( 6 );//Һ������ʾ����
	ILI9341_Clear(0,0,LCD_X_LENGTH,LCD_Y_LENGTH);	/* ��������ʾȫ�� */ 	
	LCD_SetColors(WHITE,BLACK);

	Gitwits_Init();
  delay_init();		  //��ʼ����ʱ����
  uart2_init(115200);  //�����ӻ������Լ��Ӵӻ���������
  uart3_init(115200);//����ʶ�𴮿�
  IDENTIFY_INIT();//���ô�������Լ�ʶ��ȼ�
	SYN_BW_init();   //����ģ��æµ������
  BLE_Reset_Init();//���������λ����
 // SYN_RCE_init();   //����ģ�鸴λ����
	LCD_Test();
	while(1)
	{
          

if(GPIO_ReadInputDataBit(SYN_BW_GPIO,SYN_BW_IO)==0 )//0����	 1������æµ
{	
			Three_One(DICT_, 0x07, 0x02, D_MP3_2);//������Ϊ����ܼ�,���ѳɹ��󲥷š������⡱,֮�����ʶ��0x00�ʵ��еĴ��� 
				//while(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_12)==1 );		
		if(SYN_Wake_Rsurt==1)
			{
					SYN_Wake_Rsurt=0;
					SYN_TTS("������");
					Execute_Result();//�Դ��豸����
			//		Dict_TTS_OK(DICT_Play);	//����ʶ��Ľ��				
				//	if()
						
//					else
//						SYN_TTS("���������");
								//����ʶ�𵽵Ľ��				
			}
	  else
			{
				Dict_TTS(DICT_Play); 
			}
				//���Dict_TTS������ʶ������������
}

	 }
		
	
 }

//**************************************************************************************************


