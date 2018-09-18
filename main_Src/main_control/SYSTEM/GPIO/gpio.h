#ifndef __GPIO_H
#define __GPIO_H
#include "sys.h"
#define SYN_BW_IO   GPIO_Pin_5
#define SYN_BW_GPIO   GPIOA

#define BLE_RST_IO  GPIO_Pin_1
#define BLE_RST_GPIO  GPIOB
void SYN_BW_init(void);
void SYN_RCE_init(void);
void BLE_Reset_Init(void);
#endif
