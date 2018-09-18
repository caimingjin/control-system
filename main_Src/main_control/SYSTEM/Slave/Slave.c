#include "Slave.h"
#define Slave_adrrA_AIR 0x41 
#define Slave_adrrB_Light 0x42
#define Slave_adrrC_TV 0x43
#define Slave_adrrD_MP3   0x44   
//mp3控制命令
char QUIT[4]={ 0xFD,0x00,0x01,0x02 };
char STOP[4]={ 0xFD, 0x00,0x01,0x03 };
char CONTINUE[4]={0xFD,0x00,0x01,0x04 };
unsigned char SONG_NUMNER=0;//MP3播放音乐的歌曲数量

//extern  currentDataPoint;
//定义从设备数组，存储上一次从机状态，以两次状态不同，让语音模块发声
u8 Slave_adrrA_AIR_BUFF[3]={0x00,0x00,0x00};   //分别是 开关机 模式（0-3） 温度（0-13）+17
u8 Slave_adrrB_Light_BUFF[2]={0x00,0x00};  //开关机  灯等级（0-7）
u8 Slave_adrrC_TV_BUFF[2]={0x00,0x00};    //开关机   频道 0-99
u8 Slave_adrrD_MP3_BUFF[2]={0x00,0x00};//存放需要播放音乐的值,播放状态，播放到哪个歌曲
char  *Light_Level[9]={"关闭","一个","二个","三个","四个","五个","六个","七个","八个"};
char  *AIR_Model[4]={"自动","制冷","抽湿","制热"};

char *AIR_Temper[14]={"十七度","十八度","十九度","二十度","二十一度","二十二度","二十三度",\
									"二十四度","二十五度","二十六度","二十七度","二十八度","二十九度","三十度"};
//   播放控制符[n2]：数字作为数值：0x5b 0x6e 0x32  0x5d
char TV_Channel[6]={0x5b,0x6e,0x32,0x5d,0x30,0x30};
u8 WIFI_Send_flag=0;
/*
* Send2Slave[0]，Send2Slave[1]蓝牙发射标准格式：： 
*Send2Slave[2] ：A B C 从设备选择
*Send2Slave[3] ：a b c 从设备命令
*/
unsigned char Send2Slave[4]={':',':',0,0};

extern void LCD_Test(void);
void Check_USART_RX_BUF2(void)
{
		u8 i;
		if(USART_RX_BUF2_FULL)
	{			WIFI_Send_flag=1;
			USART_RX_BUF2_FULL=0;
		switch(USART_RX_BUF2[1])
		{
					//空调============================================================================
		case Slave_adrrA_AIR:
		//执行从设备A相应代码，显示，上传机智云,或者播放音频提示操作完成
		//比如;按下遥控，从机接受并执行后会给主机回传数据，主机接收到后进行播报，遥控按下电视开机，主机播报电视已开
						if(USART_RX_BUF2[2]!=Slave_adrrA_AIR_BUFF[0])//接到的和上次存储的不一样
							{
									
									if(USART_RX_BUF2[2]==POWER_ON)
									{
										Slave_adrrA_AIR_BUFF[0]=USART_RX_BUF2[2];
										SYN_TTS("空调设备开"); //语音播报里面包含机智云上传代码
									
									}
									else
									{
										Slave_adrrA_AIR_BUFF[0]=USART_RX_BUF2[2];
										SYN_TTS("空调设备关");
									}		
								
							}

						  else if(USART_RX_BUF2[3]!=Slave_adrrA_AIR_BUFF[1] && USART_RX_BUF2[2]==POWER_ON)//接到的和上次存储的不一样,代表模式改变了，应该播报出来
							{
											Slave_adrrA_AIR_BUFF[1]=USART_RX_BUF2[3];
//									if(USART_RX_BUF2[3]>Slave_adrrB_Light_BUFF[1]) //
//										{
														SYN_TTS("当前模式为"); 
														SYN_TTS(AIR_Model[(Slave_adrrA_AIR_BUFF[1])]); 
//										}

							}
						 else if(USART_RX_BUF2[4]!=Slave_adrrA_AIR_BUFF[2]  && USART_RX_BUF2[2]==POWER_ON)//接到的和上次存储的不一样,代表模式改变了，应该播报
							{
								Slave_adrrA_AIR_BUFF[2]=USART_RX_BUF2[4];
//									if(USART_RX_BUF2[3]>Slave_adrrB_Light_BUFF[1]) //
//										{
														SYN_TTS("当前温度"); 
														SYN_TTS(AIR_Temper[Slave_adrrA_AIR_BUFF[2]]); 
//										}

							}
					
					
					
				
				break;
				//灯======================================================================================
		case Slave_adrrB_Light: 
			if(USART_RX_BUF2[2]!=Slave_adrrB_Light_BUFF[0])//接到的和上次存储的不一样
				{
						Slave_adrrB_Light_BUFF[0]=USART_RX_BUF2[2];
						if(USART_RX_BUF2[2]==POWER_ON)
						SYN_TTS("灯设备开"); 
						else
						SYN_TTS("灯设备关"); 
				}
		else if(USART_RX_BUF2[3]!=Slave_adrrB_Light_BUFF[1]  && USART_RX_BUF2[2]==POWER_ON)//接到的和上次存储的不一样
				{
						Slave_adrrB_Light_BUFF[1]=USART_RX_BUF2[3];
//									if(USART_RX_BUF2[3]>Slave_adrrB_Light_BUFF[1]) //这次灯亮数量多
//										{
											SYN_TTS("亮灯个数"); 
											SYN_TTS(Light_Level[Slave_adrrB_Light_BUFF[1]]); 
//										}
				}
		
					
//执行从设备C相应代码，显示，或者上传机智云
				break;
							//电视=================================================================================
	case Slave_adrrC_TV:
//执行从设备C相应代码，显示，或者上传机智云
			if(USART_RX_BUF2[2]!=Slave_adrrC_TV_BUFF[0])//接到的和上次存储的不一样
				{
					Slave_adrrC_TV_BUFF[0]=USART_RX_BUF2[2];
						if(USART_RX_BUF2[2]==POWER_ON)
						SYN_TTS("电视开机"); 
						else
						SYN_TTS("电视关机"); 
				}
			 else if(USART_RX_BUF2[3]!=Slave_adrrC_TV_BUFF[1]  && USART_RX_BUF2[2]==POWER_ON )//接到的和上次存储的不一样
				{
						Slave_adrrC_TV_BUFF[1]=USART_RX_BUF2[3];      //(TV_Channel[4]*10 +TV_Channel[5]);
						TV_Channel[4]=USART_RX_BUF2[3]/10 + 0x30;//转换为ASCII
						TV_Channel[5]=USART_RX_BUF2[3]%10 + 0x30;
//									if(USART_RX_BUF2[3]>Slave_adrrB_Light_BUFF[1]) //这次灯亮数量多
//										{
										SYN_TTS("当前频道"); 
										SYN_TTS(TV_Channel); 
//										}
				}

	
				break;



	case Slave_adrrD_MP3:

			switch(USART_RX_BUF2[2])
			{
				case MP3_START:
					if(Slave_adrrD_MP3_BUFF[0]==MP3_QUIT  )//存放上次的状态没有播放
							SYN_MP3(SONG_NUMNER);
				break;

				case MP3_QUIT:
				if(Slave_adrrD_MP3_BUFF[0]!=MP3_QUIT)
				{
					UART_Put_String(QUIT, 4);
						Slave_adrrD_MP3_BUFF[0]=MP3_QUIT;
					while(USART_RX_BUF3_FULL==0);
					USART_RX_BUF3_FULL=0;
				}
				break;

				case MP3_STOP:
				//	if(Slave_adrrD_MP3_BUFF[0]==MP3_START)//正在进行音乐播放
					//{
							UART_Put_String(STOP, 4);	
							while(USART_RX_BUF3_FULL==0);
							USART_RX_BUF3_FULL=0;
				//	}
				break;

				case MP3_CONTINUE:
				//	if(Slave_adrrD_MP3_BUFF[0]==MP3_STOP)//
				//	{
							//继续播放播放音乐
							UART_Put_String(CONTINUE, 4);
							while(USART_RX_BUF3_FULL==0);
							USART_RX_BUF3_FULL=0;
				//	}
				break;

				case MP3_NEXT:
					if(Slave_adrrD_MP3_BUFF[0]!=MP3_QUIT)//
					{
							if(SONG_NUMNER<MP3_SONG_MAX)
									SONG_NUMNER++;
							else	SONG_NUMNER=0;
					}
				break;

				case MP3_LAST:
					if(Slave_adrrD_MP3_BUFF[0]!=MP3_QUIT)//
					{
							if(SONG_NUMNER>0)SONG_NUMNER--;
							else SONG_NUMNER=MP3_SONG_MAX;
					}
				break;

			}

	//	
		Slave_adrrD_MP3_BUFF[0]=USART_RX_BUF2[2];
		//Slave_adrrD_MP3_BUFF[1]=SONG_NUMNER;
	//	Slave_adrrD_MP3_BUFF[1]=USART_RX_BUF2[3];

	
				break;
		}
		//清空数组
			for(i=0;i<USART2_REC_LEN;i++)
			{
					USART_RX_BUF2[i]=0x00;
			}
	//	userHandle();

		 LCD_Test();

	}

}
/*
10  42 67   B   16
14  43 61   C   20
05  41 66   A   5
10  42 67   B  16
14  43 61   C  20
21  30 64   0  _3
03  41 64   A  _3   
08  41 69   A  18
13  42 6A   B  19
*/
void BLE_Send(void)
{
	USART_Put_String(BLE_USART,Send2Slave,/*sizeof(Send2Slave)/sizeof(Send2Slave[0])*/ 4);
	delay_ms(80);
Send2Slave[2]='0';
Send2Slave[3]='0';
//	delay_ms(80);
//	/*蓝牙复位*/
GPIO_ResetBits(BLE_RST_GPIO,BLE_RST_IO);
	delay_ms(5);
GPIO_SetBits(BLE_RST_GPIO,BLE_RST_IO);

}
//语音识别结构后，执行相应程序
u8 Execute_Result(void)
{
//	u8 temp=99;
//USART_RX_BUF3[8];为词典命令ID命令低八位，十六进制表示

//	temp=((USART_RX_BUF3[8]/10)*16 )+  (USART_RX_BUF3[8]%10);//转换成十进制标志
//		USART_Put_String(USART2,&temp,1);
//if(temp<= Command_NUM)
if(1)
{
/*
设备A对应000-009   temp/10==0
设备B对应010-019   temp/10==1
设备C对应020-029   temp/10==2
*/

//if(temp/10==0)			Send2Slave[2]='A';
//if(temp/10==1)			Send2Slave[2]='B';
//if(temp/10==2)			Send2Slave[2]='C';
	switch(USART_RX_BUF3[8]/10)
	{
//设备A
		case 0:
			Send2Slave[2]='A';
		break;
//设备B
		case 1:
			Send2Slave[2]='B';
		break;
//设备C
		case 2:
			Send2Slave[2]='C';
		break;	
		default :;
			Send2Slave[2]='0';
	}

	switch(USART_RX_BUF3[8]%10)
{
	case 0:
			Send2Slave[3]='a';
	break;
	case 1:
			Send2Slave[3]='b';
	break;
	case 2:
			Send2Slave[3]='c';
	break;
	case 3:
			Send2Slave[3]='d';
	break;
	case 4:
			Send2Slave[3]='e';
	break;
	case 5:
			Send2Slave[3]='f';
	break;
	case 6:
			Send2Slave[3]='g';
	break;
	case 7:
			Send2Slave[3]='h';
	break;
	case 8:
			Send2Slave[3]='i';
	break;
	case 9:
			Send2Slave[3]='j';
	break;
	default :;
	Send2Slave[3]='0';
}
 BLE_Send();
////////////////
}
else
	return 0;

return 1;


}
/*
*开空调 000    空调关 001     冷一点 002    热一点 003      换个模式 004
*空调开 000    关空调 001     太热了 002    太冷了 003      制冷 005
*打开空调 000  关闭空调 001   温度低一点 002 升高温度 003    制冷模式 005
*空调打开 000  空调关闭 001   降低温度 002   温度高一点 003  制热 006
* 
*制热模式  006   打开灯 010    关闭灯 011    暗一点 012    调亮一点 013
*抽湿 007        开灯 010      关掉灯 011    调暗一点 012  灯最亮 014
*抽湿模式  007   把灯打开 010   把灯关掉 011  太暗了 013    灯适中 015 
*自动模式 008    关灯 011      太亮了 012    亮一点 013    灯最暗 016
*
*电视开 020    关闭电视 021    
*打开电视 020   关掉电视 021    换个频道 023 
*开电视 020    下个频道 022    上个频道 024
*电视关 021    下一个台 022    上一个台 024
*/
