#ifndef _KONGTIAO_H_
#define _KONGTIAO_H_
#include "stc15.h"
#define BLE P44
#define Auto 0
#define Hot 3
#define COOl 1
#define WETTED 2
void AIR_temper_add(void);//�¶ȼ�
void AIR_temper_mul(void);//�¶ȼ���
void AIR_Model(void);//ģʽ����
void AIR_On_oFF(void);//�ػ�
void AIR_On_on(void);//����
void BLE_reset(void);
extern void Timer1Init_main(void);
extern void Delay1ms(void);
extern void Delay25ms(void);
void Model_Change(unsigned char temp);
void BLE_reset(void);
void Send_info(void);
void AIR_temper_Set(unsigned char dat );
#endif
