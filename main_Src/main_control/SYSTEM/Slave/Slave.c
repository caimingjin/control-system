#include "Slave.h"
#define Slave_adrrA_AIR 0x41 
#define Slave_adrrB_Light 0x42
#define Slave_adrrC_TV 0x43
#define Slave_adrrD_MP3   0x44   
//mp3��������
char QUIT[4]={ 0xFD,0x00,0x01,0x02 };
char STOP[4]={ 0xFD, 0x00,0x01,0x03 };
char CONTINUE[4]={0xFD,0x00,0x01,0x04 };
unsigned char SONG_NUMNER=0;//MP3�������ֵĸ�������

//extern  currentDataPoint;
//������豸���飬�洢��һ�δӻ�״̬��������״̬��ͬ��������ģ�鷢��
u8 Slave_adrrA_AIR_BUFF[3]={0x00,0x00,0x00};   //�ֱ��� ���ػ� ģʽ��0-3�� �¶ȣ�0-13��+17
u8 Slave_adrrB_Light_BUFF[2]={0x00,0x00};  //���ػ�  �Ƶȼ���0-7��
u8 Slave_adrrC_TV_BUFF[2]={0x00,0x00};    //���ػ�   Ƶ�� 0-99
u8 Slave_adrrD_MP3_BUFF[2]={0x00,0x00};//�����Ҫ�������ֵ�ֵ,����״̬�����ŵ��ĸ�����
char  *Light_Level[9]={"�ر�","һ��","����","����","�ĸ�","���","����","�߸�","�˸�"};
char  *AIR_Model[4]={"�Զ�","����","��ʪ","����"};

char *AIR_Temper[14]={"ʮ�߶�","ʮ�˶�","ʮ�Ŷ�","��ʮ��","��ʮһ��","��ʮ����","��ʮ����",\
									"��ʮ�Ķ�","��ʮ���","��ʮ����","��ʮ�߶�","��ʮ�˶�","��ʮ�Ŷ�","��ʮ��"};
//   ���ſ��Ʒ�[n2]��������Ϊ��ֵ��0x5b 0x6e 0x32  0x5d
char TV_Channel[6]={0x5b,0x6e,0x32,0x5d,0x30,0x30};
u8 WIFI_Send_flag=0;
/*
* Send2Slave[0]��Send2Slave[1]���������׼��ʽ���� 
*Send2Slave[2] ��A B C ���豸ѡ��
*Send2Slave[3] ��a b c ���豸����
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
					//�յ�============================================================================
		case Slave_adrrA_AIR:
		//ִ�д��豸A��Ӧ���룬��ʾ���ϴ�������,���߲�����Ƶ��ʾ�������
		//����;����ң�أ��ӻ����ܲ�ִ�к��������ش����ݣ��������յ�����в�����ң�ذ��µ��ӿ������������������ѿ�
						if(USART_RX_BUF2[2]!=Slave_adrrA_AIR_BUFF[0])//�ӵ��ĺ��ϴδ洢�Ĳ�һ��
							{
									
									if(USART_RX_BUF2[2]==POWER_ON)
									{
										Slave_adrrA_AIR_BUFF[0]=USART_RX_BUF2[2];
										SYN_TTS("�յ��豸��"); //����������������������ϴ�����
									
									}
									else
									{
										Slave_adrrA_AIR_BUFF[0]=USART_RX_BUF2[2];
										SYN_TTS("�յ��豸��");
									}		
								
							}

						  else if(USART_RX_BUF2[3]!=Slave_adrrA_AIR_BUFF[1] && USART_RX_BUF2[2]==POWER_ON)//�ӵ��ĺ��ϴδ洢�Ĳ�һ��,����ģʽ�ı��ˣ�Ӧ�ò�������
							{
											Slave_adrrA_AIR_BUFF[1]=USART_RX_BUF2[3];
//									if(USART_RX_BUF2[3]>Slave_adrrB_Light_BUFF[1]) //
//										{
														SYN_TTS("��ǰģʽΪ"); 
														SYN_TTS(AIR_Model[(Slave_adrrA_AIR_BUFF[1])]); 
//										}

							}
						 else if(USART_RX_BUF2[4]!=Slave_adrrA_AIR_BUFF[2]  && USART_RX_BUF2[2]==POWER_ON)//�ӵ��ĺ��ϴδ洢�Ĳ�һ��,����ģʽ�ı��ˣ�Ӧ�ò���
							{
								Slave_adrrA_AIR_BUFF[2]=USART_RX_BUF2[4];
//									if(USART_RX_BUF2[3]>Slave_adrrB_Light_BUFF[1]) //
//										{
														SYN_TTS("��ǰ�¶�"); 
														SYN_TTS(AIR_Temper[Slave_adrrA_AIR_BUFF[2]]); 
//										}

							}
					
					
					
				
				break;
				//��======================================================================================
		case Slave_adrrB_Light: 
			if(USART_RX_BUF2[2]!=Slave_adrrB_Light_BUFF[0])//�ӵ��ĺ��ϴδ洢�Ĳ�һ��
				{
						Slave_adrrB_Light_BUFF[0]=USART_RX_BUF2[2];
						if(USART_RX_BUF2[2]==POWER_ON)
						SYN_TTS("���豸��"); 
						else
						SYN_TTS("���豸��"); 
				}
		else if(USART_RX_BUF2[3]!=Slave_adrrB_Light_BUFF[1]  && USART_RX_BUF2[2]==POWER_ON)//�ӵ��ĺ��ϴδ洢�Ĳ�һ��
				{
						Slave_adrrB_Light_BUFF[1]=USART_RX_BUF2[3];
//									if(USART_RX_BUF2[3]>Slave_adrrB_Light_BUFF[1]) //��ε���������
//										{
											SYN_TTS("���Ƹ���"); 
											SYN_TTS(Light_Level[Slave_adrrB_Light_BUFF[1]]); 
//										}
				}
		
					
//ִ�д��豸C��Ӧ���룬��ʾ�������ϴ�������
				break;
							//����=================================================================================
	case Slave_adrrC_TV:
//ִ�д��豸C��Ӧ���룬��ʾ�������ϴ�������
			if(USART_RX_BUF2[2]!=Slave_adrrC_TV_BUFF[0])//�ӵ��ĺ��ϴδ洢�Ĳ�һ��
				{
					Slave_adrrC_TV_BUFF[0]=USART_RX_BUF2[2];
						if(USART_RX_BUF2[2]==POWER_ON)
						SYN_TTS("���ӿ���"); 
						else
						SYN_TTS("���ӹػ�"); 
				}
			 else if(USART_RX_BUF2[3]!=Slave_adrrC_TV_BUFF[1]  && USART_RX_BUF2[2]==POWER_ON )//�ӵ��ĺ��ϴδ洢�Ĳ�һ��
				{
						Slave_adrrC_TV_BUFF[1]=USART_RX_BUF2[3];      //(TV_Channel[4]*10 +TV_Channel[5]);
						TV_Channel[4]=USART_RX_BUF2[3]/10 + 0x30;//ת��ΪASCII
						TV_Channel[5]=USART_RX_BUF2[3]%10 + 0x30;
//									if(USART_RX_BUF2[3]>Slave_adrrB_Light_BUFF[1]) //��ε���������
//										{
										SYN_TTS("��ǰƵ��"); 
										SYN_TTS(TV_Channel); 
//										}
				}

	
				break;



	case Slave_adrrD_MP3:

			switch(USART_RX_BUF2[2])
			{
				case MP3_START:
					if(Slave_adrrD_MP3_BUFF[0]==MP3_QUIT  )//����ϴε�״̬û�в���
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
				//	if(Slave_adrrD_MP3_BUFF[0]==MP3_START)//���ڽ������ֲ���
					//{
							UART_Put_String(STOP, 4);	
							while(USART_RX_BUF3_FULL==0);
							USART_RX_BUF3_FULL=0;
				//	}
				break;

				case MP3_CONTINUE:
				//	if(Slave_adrrD_MP3_BUFF[0]==MP3_STOP)//
				//	{
							//�������Ų�������
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
		//�������
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
//	/*������λ*/
GPIO_ResetBits(BLE_RST_GPIO,BLE_RST_IO);
	delay_ms(5);
GPIO_SetBits(BLE_RST_GPIO,BLE_RST_IO);

}
//����ʶ��ṹ��ִ����Ӧ����
u8 Execute_Result(void)
{
//	u8 temp=99;
//USART_RX_BUF3[8];Ϊ�ʵ�����ID����Ͱ�λ��ʮ�����Ʊ�ʾ

//	temp=((USART_RX_BUF3[8]/10)*16 )+  (USART_RX_BUF3[8]%10);//ת����ʮ���Ʊ�־
//		USART_Put_String(USART2,&temp,1);
//if(temp<= Command_NUM)
if(1)
{
/*
�豸A��Ӧ000-009   temp/10==0
�豸B��Ӧ010-019   temp/10==1
�豸C��Ӧ020-029   temp/10==2
*/

//if(temp/10==0)			Send2Slave[2]='A';
//if(temp/10==1)			Send2Slave[2]='B';
//if(temp/10==2)			Send2Slave[2]='C';
	switch(USART_RX_BUF3[8]/10)
	{
//�豸A
		case 0:
			Send2Slave[2]='A';
		break;
//�豸B
		case 1:
			Send2Slave[2]='B';
		break;
//�豸C
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
*���յ� 000    �յ��� 001     ��һ�� 002    ��һ�� 003      ����ģʽ 004
*�յ��� 000    �ؿյ� 001     ̫���� 002    ̫���� 003      ���� 005
*�򿪿յ� 000  �رտյ� 001   �¶ȵ�һ�� 002 �����¶� 003    ����ģʽ 005
*�յ��� 000  �յ��ر� 001   �����¶� 002   �¶ȸ�һ�� 003  ���� 006
* 
*����ģʽ  006   �򿪵� 010    �رյ� 011    ��һ�� 012    ����һ�� 013
*��ʪ 007        ���� 010      �ص��� 011    ����һ�� 012  ������ 014
*��ʪģʽ  007   �ѵƴ� 010   �ѵƹص� 011  ̫���� 013    ������ 015 
*�Զ�ģʽ 008    �ص� 011      ̫���� 012    ��һ�� 013    ��� 016
*
*���ӿ� 020    �رյ��� 021    
*�򿪵��� 020   �ص����� 021    ����Ƶ�� 023 
*������ 020    �¸�Ƶ�� 022    �ϸ�Ƶ�� 024
*���ӹ� 021    ��һ��̨ 022    ��һ��̨ 024
*/
