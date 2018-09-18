#include "SYN7318.h"
#include "string.h"
#include "Slave.h"
#include "gizwits_protocol.h"
#include "gizwits_product.h"
#include "bsp_key.h"
unsigned char Back[4] = {0};   //��������ش�������
unsigned char ASR[6] = {0};    //����ʶ�����ش�������
unsigned char S[4] = {0};      //����ģ�鵱ǰ����״̬�ش�������
//��������ʶ��Ĳ��� ʶ�����  ��ֹ�����ޣ��û���������  ʶ��ȼ�
//FD 00 07 1E 02 0F A0 0F A0 03

extern u8 Slave_adrrD_MP3_BUFF[];
extern u8 SONG_NUMNER;
// 
char *MP3[]={
"E:\\Mp3\\ƽ��֮·.mp3",
"E:\\Mp3\\�����һ��.mp3",
"E:\\Mp3\\OneRepublic.mp3",
"E:\\Mp3\\��������.mp3",
"E:\\Mp3\\��Ա.mp3",
"E:\\Mp3\\Ҳ����ʶ.mp3",
"E:\\Mp3\\��������ҹ.mp3",
"E:\\Mp3\\1965.mp3",
};               	  
char *Dict0[] = {
//00  		   		 01       		 	02						03							04			
"�յ��ѿ�",		"�յ��ѹ�",			"�¶��ѽ�",			"�¶�����",				"ģʽ�ѻ�",	
"����",				"����",					"��ʪ ",			"�Զ�",				  		"�հ�",  
"���ѿ�",			"���ѹ�",				"���Ѱ�",				"������",					"������",		
"������",			"���",					"�հ�",					"�հ�",						"�հ�",		
"�����ѿ�",		"�����ѹ�",			"�¸�Ƶ��",			"Ƶ���ѻ�",			"�ϸ�Ƶ��"
};//00�ʵ�
u8 BLE_SEND_DAT_FLAG=0;
char ** DICT[10] = {Dict0,MP3}; //������������ʵ�

unsigned char Busy_Free=0;
u8 SYN_Wake_Rsurt=0;
extern void Delay(u32 count);

//ʶ�������ʼ�� ʶ����룬ʶ��ȼ�
void IDENTIFY_INIT(void)
{
unsigned char Command_Data[10]={0xfd,0x00,0x07,0x1e,IDENTIFY_DISTANCE,0x0f,0xa0,0x0f,0xa0,IDENTIFY_LEVEL_MIDDLE};

		USART_Put_String(USART3,Command_Data,10);
		USART_RX_BUF3_FULL=0;
		while(USART_RX_BUF3_FULL==0);//�ȴ�������ȷ�ش�
		USART_RX_BUF3_FULL=0;
		Clear_USART_RX_BUF3();//���BUF3
}
/****************************************************************************************
���� �� ������SYN_TTS----�����ϳɲ��ź���
������˵������Pst�����Ҫ�ϳɲ��ŵ��ı���������
����    ������SYN_TTS("��������"); //�ϳɲ��ű�������              
****************************************************************************************/
void SYN_TTS( char *Pst)
{
	uint16_t Length;
	char Frame[5];   //���淢�����������
	
//if(Slave_adrrD_MP3_BUFF[0]==MP3_QUIT)
//{
	Length = strlen(Pst);
	Frame[0] = 0xFD;      //֡ͷ
	Frame[1] = 0x00;
	Frame[2] = Length+2;
	Frame[3] = 0x01;      //�����ϳɲ�������
	Frame[4] = 0x00;      //���ű����ʽΪ��GB2312��
	
  UART_Put_String(Frame,5);	
	UART_Put_String(Pst, Length);	
	delay_ms(140);  //��֤����ģ�鲥�ž���  ��Ȼ���Ÿߵ͵�ƽ��ⲻ��
	userHandle();//�������ϱ�
	while(GPIO_ReadInputDataBit(SYN_BW_GPIO,SYN_BW_IO)==1){
								
									userHandle();
}
 
}

//æµ��һֱ��
	//if(USART_RX_BUF3_FULL)USART_RX_BUF3_FULL=0;
}


/*******************************************************************
���� �� ������SYN_MP3----MP3���ź���
������˵������Pst�����Ҫ���ŵ�MP3��������
����    ������char D_MP3_2[] = {"D:\\Mp3\\Wifi_������.mp3"}; 
              SYN_MP3(D_MP3_2); //����MP3
*******************************************************************/
void SYN_MP3(/*char *Pst,unsigned char control,*/unsigned char song_num)
{
	uint16_t Length;
	u8  sone_munber=SONG_NUMNER;
	char Frame[5];   //���淢�����������

	Length = strlen(DICT[1][song_num]);
	
	Frame[0] = 0xFD;      //֡ͷ
	Frame[1] = 0x00;
	Frame[2] = Length+2;
	Frame[3] = 0x61;      //MP3��������
	Frame[4] = 0x00;      //���ű����ʽΪ��GB2312��
	
  UART_Put_String(Frame, 5);	
	UART_Put_String(DICT[1][song_num], Length);	
	//delay_ms(150);  //��֤����ģ�鲥�ž���  ��Ȼ���Ÿߵ͵�ƽ��ⲻ��
	//userHandle();//�������ϱ�
	while(  GPIO_ReadInputDataBit(SYN_BW_GPIO,SYN_BW_IO)==1 || Slave_adrrD_MP3_BUFF[0]!=MP3_QUIT){
								
				userHandle();
	//			Execute_Result();//�Դ��豸����
				Check_USART_RX_BUF2();//��⴮�ڶ����ܵ����豸�ϴ�����������
				if(sone_munber!=SONG_NUMNER)//�����ı䲥�Ÿ���
				{    sone_munber=SONG_NUMNER;
							Length = strlen(DICT[1][sone_munber]);
							Frame[2] = Length+2;
						  UART_Put_String(Frame, 5);	
							UART_Put_String(DICT[1][sone_munber], Length);	
							USART_RX_BUF3_FULL=0;
							while(USART_RX_BUF3_FULL==0);
							USART_RX_BUF3_FULL=0;
				}

} //æµ��һֱ��	


Slave_adrrD_MP3_BUFF[0]=MP3_QUIT;

}


/*********************************************************************
���� �� ������Start_ASR----��ʼ����ʶ����
������˵������Dict���ʵ���
����    ������Start_ASR(0x00); //ʶ��0x00�ʵ��еĴ���
*********************************************************************/
void Start_ASR(char Dict)
{
	 char Frame[5];   //���淢�����������
	
	Frame[0] = 0xFD;      //֡ͷ
	Frame[1] = 0x00;
	Frame[2] = 0x02;   
	Frame[3] = 0x10;      //��ʼ����ʶ������
	Frame[4] = Dict;      //�ʵ���  �������޸���Ҫʶ��Ĵʵ���
	
	ASR[3] = 0;
	
  UART_Put_String(Frame, 5);	
	UART_Get_String(Back,4); 
	if(Back[3] == 0x41)
	{
		 UART_Get_String(Back,3);  //����ʶ������ش����
		if(Back[0] == 0xfc) 
		{
			 UART_Get_String(ASR,Back[2]);
		}
	}
}
//����һʶ�𣬵õ��������
void Dict_TTS_OK(char Dict)
{
	if(USART_RX_BUF3[8]<COM_ID_NUM)
	SYN_TTS(DICT[Dict][USART_RX_BUF3[8]]);  //ASR[3]�д�Ŵ���ID
}
/***************************************************************************
���� �� ������Dict_TTS----ʶ��ʵ�����������
������˵������Dict���ʵ���
����    ������Start_ASR(0x00); //ʶ��0x00�ʵ��еĴ���
              Dict_TTS(0x00);  //���ʶ��ɹ�,��ʶ�𵽵Ĵ����������� 
***************************************************************************/
void Dict_TTS(char Dict)
{
	switch(USART_RX_BUF3[3])
	{
//		case 0x01:
//		case 0x02:
//		{
//			SYN_TTS(DICT[Dict][USART_RX_BUF3[8]]);  //ASR[3]�д�Ŵ���ID
//				
//		}break;
		case 0x03:
		{
			SYN_TTS("������ʱ"); 
		}break;
		case 0x04:
		{
			SYN_TTS("������ʱ");
		}break;
		case 0x05:
		case 0x07:
		{
			SYN_TTS("Ҫ˵�����");
		}break;
		case 0x06:
		{
			SYN_TTS("ʶ���ڲ�����");
		}break;
		
	}

		//ASR[3]=0xff;//���

}

/*********************************************************************
���� �� ������Stop_ASR----ֹͣ����ʶ����
������˵�������޲�
*********************************************************************/
void Stop_ASR(void)
{
 char Frame[4];   //���淢�����������
	
	Frame[0] = 0xFD;      //֡ͷ
	Frame[1] = 0x00;
	Frame[2] = 0x01;   
	Frame[3] = 0x11;      //ֹͣ����ʶ������
	
  UART_Put_String(Frame, 4);
}



/*********************************************************************************************************************************************************
���� �� ������Start_WakeUp----��ʼ�������Ѻ���
������˵������Wake_ID��������ID��----0x00���Ʊ�  0x02��С��   0x04������  0x06�������ܼ�  0x07������ܼ�  0x08��С���ܼ�  0x09����ܼ�  0x1F���Զ���
����    ������Start_WakeUp(0x09);//������Ϊ��ܼ�,���ѳɹ�֮�󲥱��������⡱
**********************************************************************************************************************************************************/
void Start_WakeUp(char Wake_ID)
{
	 char Frame[5];   //���淢�����������
	
	Frame[0] = 0xFD;      //֡ͷ
	Frame[1] = 0x00;
	Frame[2] = 0x02;   
	Frame[3] = 0x51;      //��ʼ������������
	Frame[4] = Wake_ID;   
	
  UART_Put_String(Frame, 5);	
	UART_Get_String(Back,4); 
	if(Back[3] == 0x41)  
	{
		 UART_Get_String(Back,4);  //����ʶ������ش����
		if(Back[3] == 0x21)
		{
			// SYN_MP3("D:\\Mp3\\Wifi_������.mp3"); 
			//Delay(50000000);
			SYN_Wake_Rsurt=1;
		}
		else
		{
			//SYN_Wake_Rsurt=0;
			//SYN_TTS("���"); 
			//Delay(50000000);
		}
	}
	else
	{
		//SYN_Wake_Rsurt=0;
			//SYN_TTS("����"); 
			//Delay(50000000);
	}
}


/*********************************************************************************************************************************************************
���� �� ������Three_One----����һ�����������������ѣ����Ѻ󲥷���ʾ���������������ʶ��
������˵������Dict���ʵ���
              Wake_ID��������ID��----0x00���Ʊ�  0x02��С��   0x04������  0x06�������ܼ�  0x07������ܼ�  0x08��С���ܼ�  0x09����ܼ�  0x1F���Զ���
              Sound����ʾ������  0x00������ʾ��  0x01���ı�����  0x02������MP3����
              Pst����ʾ��������
����    ������Three_One(0x00, 0x00, 0x02, D_MP3_2);//������Ϊ�Ʊ�,���ѳɹ��󲥷š������⡱,֮�����ʶ��0x00�ʵ��еĴ��� 
              Dict_TTS(0x00); //���Dict_TTS������ʶ������������
**********************************************************************************************************************************************************/
void Three_One(char Dict, char Wake_ID, char Sound,  char *Pst)
{
	 char Frame[100];   //���淢�����������
	uint16_t Length,i;
	
	Frame[0] = 0xFD;      //֡ͷ
//	Frame[1] = 0x00;
//	Frame[2] = 0x07;    //���ڲ�ȷ���������ĳ��ȣ�����ѡ����ʱ����
	Frame[3] = 0x15;      //����һʶ������
	Frame[4] = Dict;      //�ʵ���
	Frame[5] = Wake_ID;   //������ID��
	Frame[6] = Sound;     //��ʾ������
	if(Sound == 0x00)     //����ʾ��
	{
		Frame[1] = 0x00;
		Frame[2] = 0x04;
	  UART_Put_String(Frame, 7);	
	}
	else                 //����ʾ��
	{
		Frame[7] = 0x01;      //�ı�������MP3��ʾ�������ʽ��0x00��GB2312����  0x01��GBK����  0x02��BIG5����  0x03��UnicodeСͷ  0x04�Unicode��ͷ
		Length = strlen(Pst); //�ı�������MP3��ʾ���ĳ���
		Frame[1] = 0x00;
		Frame[2] = Length + 5;
		for(i = 0; i < Length; i++)
		{
			Frame[8 + i] = Pst[i];
		}
		Clear_USART_RX_BUF3();//���BUF3
	  UART_Put_String(Frame, Length + 8);	
	}
	//UART_Get_String(Back,4);
	while(USART_RX_BUF3_FULL==0);
		USART_RX_BUF3_FULL=0;
	if(USART_RX_BUF3[3] == 0x41)
	{
	//	UART_Get_String(Back,3);  //����ʶ������ش����
				Clear_USART_RX_BUF3();//���BUF3
		while(USART_RX_BUF3_FULL==0)
				{
						//�����������ͳ��򣬴˴�Ϊ�ȴ�����

					Check_USART_RX_BUF2();//��⴮�ڶ����ܵ����豸�ϴ�����������
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
			//ASR[0]=0x00;//���
		if(USART_RX_BUF3[0] ==0xfc && USART_RX_BUF3[3] == 0x01)
		{
				// UART_Get_String(ASR,Back[2]);
					SYN_Wake_Rsurt=1;	
		}
	}
	
}


/***************************************************************************
���� �� ������Status_Query----ģ��״̬��ѯ����
������˵�������޲�
����    ������Status_Query(); 
***************************************************************************/
void Status_Query(void)
{
	 char Frame[4];   //���淢�����������
	
	Frame[0] = 0xFD;    //֡ͷ
	Frame[1] = 0x00;
	Frame[2] = 0x01;   
	Frame[3] = 0x21;   //״̬��ѯ����
	
  UART_Put_String(Frame, 4);	
	UART_Get_String(Back,4); 
	if(Back[3] == 0x41)  
	{
		 UART_Get_String(S,4);  //ģ�鵱ǰ����״̬�Ļش����
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
