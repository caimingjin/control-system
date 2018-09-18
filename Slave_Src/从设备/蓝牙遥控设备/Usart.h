#ifndef _USART_H_
#define _USART_H_
#include "header.h"
typedef unsigned char BYTE;
typedef unsigned int WORD;
#include "stc15.h"
#include "intrins.h"
extern  BYTE  BLEReceive[2];
void Usart_init(void);
void SendData(BYTE dat);
void SendString(char *s,unsigned char Size);
#endif
