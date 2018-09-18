#ifndef __SLAVE_H
#define __SLAVE_H
#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "SYN7318.h"
#include "string.h"
#include "gpio.h"
#include "gizwits_product.h"
#include "gizwits_protocol.h"
//定义播放音乐数值
#define MP3_START 0xff
#define MP3_QUIT  0x00
#define MP3_STOP  0xf0
#define MP3_CONTINUE  0x0f
#define MP3_NEXT  0xee
#define MP3_LAST  0xa0

#define MP3_SONG_MAX  8
//MP3控制命令



#define  BLE_USART  USART2

extern unsigned char Send2Slave[4];
extern unsigned char Slave_adrrA_AIR_BUFF[3];   //分别是 开关机 模式（0-3） 温度（0-13）+17
extern unsigned char Slave_adrrB_Light_BUFF[2];  //开关机  灯等级（0-7）
extern unsigned char Slave_adrrC_TV_BUFF[2];    //开关机   频道 0-99



void BLE_Send(void);
//从设备中默认255 ，后0表示关机，1表示开机
#define POWER_ON  0xff
#define POWER_OFF 0

#define Command_NUM  22  //定义，命令ID总数
//#define TV_OFF 0

//#define AIR_ON 1
//#define AIR_OFF 0
void Check_USART_RX_BUF2(void);  
u8 Execute_Result(void);
#endif

