#ifndef _KONGTIAO_H_
#define _KONGTIAO_H_
#include "stc15.h"
#define BLE P44
void AIR_temper_add(void);//�¶ȼ�
void AIR_temper_mul(void);//�¶ȼ���
void AIR_Model(void);//ģʽ����
void AIR_On_oFF(void);//�����ػ�
extern void Timer1Init_main(void);
extern void Delay1ms(void);
extern void Delay25ms(void);
void BLE_reset(void);
#endif
