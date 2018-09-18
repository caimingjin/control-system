#ifndef _LIGHT_H_
#define _LIGHT_H_
#include "stc15.h"
#include "usart.h"
#include "OLED.h"
#define Light_MAX  3
#define Light_MID  2
#define Light_MIN  1
#define BLE P44
void Light_Off(void);
void Light_ON(void);
void Light_More(void);
void Light_Less(void);
void Send_info(void);
void BLE_reset(void);
void Light_Change(unsigned char temp);
#endif
