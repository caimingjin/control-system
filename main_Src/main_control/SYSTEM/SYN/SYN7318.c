#include "SYN7318.h"
#include "string.h"
#include "Slave.h"
#include "gizwits_protocol.h"
#include "gizwits_product.h"
#include "bsp_key.h"
unsigned char Back[4] = {0};   //接收命令回传的数组
unsigned char ASR[6] = {0};    //接收识别结果回传的数组
unsigned char S[4] = {0};      //接收模块当前工作状态回传的数组
//设置语音识别的参数 识别距离  静止音上限，用户语音上限  识别等级
//FD 00 07 1E 02 0F A0 0F A0 03

extern u8 Slave_adrrD_MP3_BUFF[];
extern u8 SONG_NUMNER;
// 
char *MP3[]={
"E:\\Mp3\\平凡之路.mp3",
"E:\\Mp3\\世界第一等.mp3",
"E:\\Mp3\\OneRepublic.mp3",
"E:\\Mp3\\友情岁月.mp3",
"E:\\Mp3\\演员.mp3",
"E:\\Mp3\\也曾相识.mp3",
"E:\\Mp3\\忘情冷雨夜.mp3",
"E:\\Mp3\\1965.mp3",
};               	  
char *Dict0[] = {
//00  		   		 01       		 	02						03							04			
"空调已开",		"空调已关",			"温度已降",			"温度已升",				"模式已换",	
"制冷",				"制热",					"抽湿 ",			"自动",				  		"空白",  
"灯已开",			"灯已关",				"灯已暗",				"灯已亮",					"灯最亮",		
"灯适中",			"灯最暗",					"空白",					"空白",						"空白",		
"电视已开",		"电视已关",			"下个频道",			"频道已换",			"上个频道"
};//00词典
u8 BLE_SEND_DAT_FLAG=0;
char ** DICT[10] = {Dict0,MP3}; //可以添加其他词典

unsigned char Busy_Free=0;
u8 SYN_Wake_Rsurt=0;
extern void Delay(u32 count);

//识别参数初始化 识别距离，识别等级
void IDENTIFY_INIT(void)
{
unsigned char Command_Data[10]={0xfd,0x00,0x07,0x1e,IDENTIFY_DISTANCE,0x0f,0xa0,0x0f,0xa0,IDENTIFY_LEVEL_MIDDLE};

		USART_Put_String(USART3,Command_Data,10);
		USART_RX_BUF3_FULL=0;
		while(USART_RX_BUF3_FULL==0);//等待接收正确回传
		USART_RX_BUF3_FULL=0;
		Clear_USART_RX_BUF3();//清除BUF3
}
/****************************************************************************************
【函 数 名】：SYN_TTS----语音合成播放函数
【参数说明】：Pst：存放要合成播放的文本的数组名
【简    例】：SYN_TTS("北京龙邱"); //合成播放北京龙邱              
****************************************************************************************/
void SYN_TTS( char *Pst)
{
	uint16_t Length;
	char Frame[5];   //保存发送命令的数组
	
//if(Slave_adrrD_MP3_BUFF[0]==MP3_QUIT)
//{
	Length = strlen(Pst);
	Frame[0] = 0xFD;      //帧头
	Frame[1] = 0x00;
	Frame[2] = Length+2;
	Frame[3] = 0x01;      //语音合成播放命令
	Frame[4] = 0x00;      //播放编码格式为“GB2312”
	
  UART_Put_String(Frame,5);	
	UART_Put_String(Pst, Length);	
	delay_ms(140);  //保证语音模块播放就绪  不然引脚高低电平检测不到
	userHandle();//机智云上报
	while(GPIO_ReadInputDataBit(SYN_BW_GPIO,SYN_BW_IO)==1){
								
									userHandle();
}
 
}

//忙碌则一直等
	//if(USART_RX_BUF3_FULL)USART_RX_BUF3_FULL=0;
}


/*******************************************************************
【函 数 名】：SYN_MP3----MP3播放函数
【参数说明】：Pst：存放要播放的MP3的数组名
【简    例】：char D_MP3_2[] = {"D:\\Mp3\\Wifi_我在这.mp3"}; 
              SYN_MP3(D_MP3_2); //播放MP3
*******************************************************************/
void SYN_MP3(/*char *Pst,unsigned char control,*/unsigned char song_num)
{
	uint16_t Length;
	u8  sone_munber=SONG_NUMNER;
	char Frame[5];   //保存发送命令的数组

	Length = strlen(DICT[1][song_num]);
	
	Frame[0] = 0xFD;      //帧头
	Frame[1] = 0x00;
	Frame[2] = Length+2;
	Frame[3] = 0x61;      //MP3播放命令
	Frame[4] = 0x00;      //播放编码格式为“GB2312”
	
  UART_Put_String(Frame, 5);	
	UART_Put_String(DICT[1][song_num], Length);	
	//delay_ms(150);  //保证语音模块播放就绪  不然引脚高低电平检测不到
	//userHandle();//机智云上报
	while(  GPIO_ReadInputDataBit(SYN_BW_GPIO,SYN_BW_IO)==1 || Slave_adrrD_MP3_BUFF[0]!=MP3_QUIT){
								
				userHandle();
	//			Execute_Result();//对从设备控制
				Check_USART_RX_BUF2();//检测串口二接受到从设备上传的数组数据
				if(sone_munber!=SONG_NUMNER)//按键改变播放歌曲
				{    sone_munber=SONG_NUMNER;
							Length = strlen(DICT[1][sone_munber]);
							Frame[2] = Length+2;
						  UART_Put_String(Frame, 5);	
							UART_Put_String(DICT[1][sone_munber], Length);	
							USART_RX_BUF3_FULL=0;
							while(USART_RX_BUF3_FULL==0);
							USART_RX_BUF3_FULL=0;
				}

} //忙碌则一直等	


Slave_adrrD_MP3_BUFF[0]=MP3_QUIT;

}


/*********************************************************************
【函 数 名】：Start_ASR----开始语音识别函数
【参数说明】：Dict：词典编号
【简    例】：Start_ASR(0x00); //识别0x00词典中的词条
*********************************************************************/
void Start_ASR(char Dict)
{
	 char Frame[5];   //保存发送命令的数组
	
	Frame[0] = 0xFD;      //帧头
	Frame[1] = 0x00;
	Frame[2] = 0x02;   
	Frame[3] = 0x10;      //开始语音识别命令
	Frame[4] = Dict;      //词典编号  在这里修改想要识别的词典编号
	
	ASR[3] = 0;
	
  UART_Put_String(Frame, 5);	
	UART_Get_String(Back,4); 
	if(Back[3] == 0x41)
	{
		 UART_Get_String(Back,3);  //语音识别命令回传结果
		if(Back[0] == 0xfc) 
		{
			 UART_Get_String(ASR,Back[2]);
		}
	}
}
//三合一识别，得到正常结果
void Dict_TTS_OK(char Dict)
{
	if(USART_RX_BUF3[8]<COM_ID_NUM)
	SYN_TTS(DICT[Dict][USART_RX_BUF3[8]]);  //ASR[3]中存放词条ID
}
/***************************************************************************
【函 数 名】：Dict_TTS----识别词典结果播报函数
【参数说明】：Dict：词典编号
【简    例】：Start_ASR(0x00); //识别0x00词典中的词条
              Dict_TTS(0x00);  //如果识别成功,把识别到的词条播报出来 
***************************************************************************/
void Dict_TTS(char Dict)
{
	switch(USART_RX_BUF3[3])
	{
//		case 0x01:
//		case 0x02:
//		{
//			SYN_TTS(DICT[Dict][USART_RX_BUF3[8]]);  //ASR[3]中存放词条ID
//				
//		}break;
		case 0x03:
		{
			SYN_TTS("静音超时"); 
		}break;
		case 0x04:
		{
			SYN_TTS("语音超时");
		}break;
		case 0x05:
		case 0x07:
		{
			SYN_TTS("要说清楚点");
		}break;
		case 0x06:
		{
			SYN_TTS("识别内部错误");
		}break;
		
	}

		//ASR[3]=0xff;//清空

}

/*********************************************************************
【函 数 名】：Stop_ASR----停止语音识别函数
【参数说明】：无参
*********************************************************************/
void Stop_ASR(void)
{
 char Frame[4];   //保存发送命令的数组
	
	Frame[0] = 0xFD;      //帧头
	Frame[1] = 0x00;
	Frame[2] = 0x01;   
	Frame[3] = 0x11;      //停止语音识别命令
	
  UART_Put_String(Frame, 4);
}



/*********************************************************************************************************************************************************
【函 数 名】：Start_WakeUp----开始语音唤醒函数
【参数说明】：Wake_ID：唤醒名ID号----0x00：云宝  0x02：小播   0x04：百灵  0x06：叮当管家  0x07：百灵管家  0x08：小播管家  0x09：大管家  0x1F：自定义
【简    例】：Start_WakeUp(0x09);//唤醒名为大管家,唤醒成功之后播报“我在这”
**********************************************************************************************************************************************************/
void Start_WakeUp(char Wake_ID)
{
	 char Frame[5];   //保存发送命令的数组
	
	Frame[0] = 0xFD;      //帧头
	Frame[1] = 0x00;
	Frame[2] = 0x02;   
	Frame[3] = 0x51;      //开始语音唤醒命令
	Frame[4] = Wake_ID;   
	
  UART_Put_String(Frame, 5);	
	UART_Get_String(Back,4); 
	if(Back[3] == 0x41)  
	{
		 UART_Get_String(Back,4);  //语音识别命令回传结果
		if(Back[3] == 0x21)
		{
			// SYN_MP3("D:\\Mp3\\Wifi_我在这.mp3"); 
			//Delay(50000000);
			SYN_Wake_Rsurt=1;
		}
		else
		{
			//SYN_Wake_Rsurt=0;
			//SYN_TTS("你好"); 
			//Delay(50000000);
		}
	}
	else
	{
		//SYN_Wake_Rsurt=0;
			//SYN_TTS("错误"); 
			//Delay(50000000);
	}
}


/*********************************************************************************************************************************************************
【函 数 名】：Three_One----三合一函数：开启语音唤醒，唤醒后播放提示音，播完后开启语音识别
【参数说明】：Dict：词典编号
              Wake_ID：唤醒名ID号----0x00：云宝  0x02：小播   0x04：百灵  0x06：叮当管家  0x07：百灵管家  0x08：小播管家  0x09：大管家  0x1F：自定义
              Sound：提示音类型  0x00：无提示音  0x01：文本类型  0x02：内置MP3类型
              Pst：提示音的内容
【简    例】：Three_One(0x00, 0x00, 0x02, D_MP3_2);//唤醒名为云宝,唤醒成功后播放“我在这”,之后可以识别0x00词典中的词条 
              Dict_TTS(0x00); //结合Dict_TTS函数把识别结果播报出来
**********************************************************************************************************************************************************/
void Three_One(char Dict, char Wake_ID, char Sound,  char *Pst)
{
	 char Frame[100];   //保存发送命令的数组
	uint16_t Length,i;
	
	Frame[0] = 0xFD;      //帧头
//	Frame[1] = 0x00;
//	Frame[2] = 0x07;    //由于不确定数据区的长度，所以选择暂时屏蔽
	Frame[3] = 0x15;      //三合一识别命令
	Frame[4] = Dict;      //词典编号
	Frame[5] = Wake_ID;   //唤醒名ID号
	Frame[6] = Sound;     //提示音类型
	if(Sound == 0x00)     //无提示音
	{
		Frame[1] = 0x00;
		Frame[2] = 0x04;
	  UART_Put_String(Frame, 7);	
	}
	else                 //有提示音
	{
		Frame[7] = 0x01;      //文本或内置MP3提示音编码格式：0x00：GB2312编码  0x01：GBK编码  0x02：BIG5编码  0x03：Unicode小头  0x04nicode大头
		Length = strlen(Pst); //文本或内置MP3提示音的长度
		Frame[1] = 0x00;
		Frame[2] = Length + 5;
		for(i = 0; i < Length; i++)
		{
			Frame[8 + i] = Pst[i];
		}
		Clear_USART_RX_BUF3();//清除BUF3
	  UART_Put_String(Frame, Length + 8);	
	}
	//UART_Get_String(Back,4);
	while(USART_RX_BUF3_FULL==0);
		USART_RX_BUF3_FULL=0;
	if(USART_RX_BUF3[3] == 0x41)
	{
	//	UART_Get_String(Back,3);  //语音识别命令回传结果
				Clear_USART_RX_BUF3();//清除BUF3
		while(USART_RX_BUF3_FULL==0)
				{
						//添加其他代码和程序，此处为等待唤醒

					Check_USART_RX_BUF2();//检测串口二接受到从设备上传的数组数据
					userHandle();

				//	gizwitsHandle((dataPoint_t*)&currentDataPoint);
//					if(BLE_SEND_DAT_FLAG)
//					{
//							BLE_SEND_DAT_FLAG=0;
//							
//					}
			  // 
					
				}
			USART_RX_BUF3_FULL=0;
			//ASR[0]=0x00;//清空
		if(USART_RX_BUF3[0] ==0xfc && USART_RX_BUF3[3] == 0x01)
		{
				// UART_Get_String(ASR,Back[2]);
					SYN_Wake_Rsurt=1;	
		}
	}
	
}


/***************************************************************************
【函 数 名】：Status_Query----模块状态查询函数
【参数说明】：无参
【简    例】：Status_Query(); 
***************************************************************************/
void Status_Query(void)
{
	 char Frame[4];   //保存发送命令的数组
	
	Frame[0] = 0xFD;    //帧头
	Frame[1] = 0x00;
	Frame[2] = 0x01;   
	Frame[3] = 0x21;   //状态查询命令
	
  UART_Put_String(Frame, 4);	
	UART_Get_String(Back,4); 
	if(Back[3] == 0x41)  
	{
		 UART_Get_String(S,4);  //模块当前工作状态的回传结果
	}
}


void Clear_USART_RX_BUF3(void)
{
	u8 i=0;
	for(i=0;i<USART3_REC_LEN;i++)
	{
		USART_RX_BUF3[i]=0x00;
	}
}
