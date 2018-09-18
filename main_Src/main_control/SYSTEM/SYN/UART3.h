#include "stm32f10x.h"
#ifndef _UART_H_
#define _UART_H_

void UARTInit3(uint32_t bound);
void UART_Put_Char(uint16_t Data);
uint16_t UART_Get_char(void);
void UART_Put_String(char *Pst,uint16_t Length);
void UART_Get_String(unsigned char *Pst,uint16_t Length); 

#endif

