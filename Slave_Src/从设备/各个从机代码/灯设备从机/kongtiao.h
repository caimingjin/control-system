#ifndef _KONGTIAO_H_
#define _KONGTIAO_H_
#include "stc15.h"
#define BLE P44
void AIR_temper_add(void);//温度加
void AIR_temper_mul(void);//温度减少
void AIR_Model(void);//模式设置
void AIR_On_oFF(void);//开机关机
extern void Timer1Init_main(void);
extern void Delay1ms(void);
extern void Delay25ms(void);
void BLE_reset(void);
#endif
