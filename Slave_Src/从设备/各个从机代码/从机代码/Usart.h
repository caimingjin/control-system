#ifndef _USART_H_
#define _USART_H_
typedef unsigned char BYTE;
typedef unsigned int WORD;
#define Slave_adrr 0x41 //从设备识别地址
#include "stc15.h"
#include "intrins.h"
extern unsigned char  BLEReceive[2];
void Usart_init(void);
void SendData(BYTE dat);
void SendString(char *s);
#endif
