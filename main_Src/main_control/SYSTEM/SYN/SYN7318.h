#include "stm32f10x.h"
#include "delay.h"
#include "UART3.h"
#include "usart.h"
#include "Slave.h"
#ifndef _SYN7318_H_
#define _SYN7318_H_
//识别参数 距离
#define  COM_ID_NUM  26
#define IDENTIFY_DISTANCE  0x02  
//拒识等级
#define IDENTIFY_LEVEL_SO_LOW  0x05 // 超低 不建议
#define IDENTIFY_LEVEL__LOW  0x04  //低
#define IDENTIFY_LEVEL_MIDDLE  0x03   //中
#define IDENTIFY_LEVEL_HIGHT  0x02   //高
#define IDENTIFY_LEVEL_SO_HIGHT  0x01 //  超高 不建议

extern unsigned char Back[4];
extern unsigned char ASR[6];
extern unsigned char S[4];
extern unsigned char Busy_Free;	
extern u8 SYN_Wake_Rsurt;
void IDENTIFY_INIT(void);
void SYN_TTS( char *Pst); //语音合成播放函数
void SYN_MP3(/*char *Pst,unsigned */unsigned char control);//播放MP3
void Start_ASR(char Dict); //开始识别函数
void Dict_TTS(char Dict); //词条索引函数
void Stop_ASR(void); //停止语音识别函数
void Start_WakeUp(char Wake_ID); //语音唤醒函数
void Three_One(char Dict, char Wake_ID, char Sound, char *Pst); //三合一函数
void Status_Query(void); //模块状态查询函数
void Clear_USART_RX_BUF3(void);
void Dict_TTS_OK(char Dict);
#endif
