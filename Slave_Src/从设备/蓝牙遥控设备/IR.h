#ifndef _IR_H_
#define _IR_H_
#include "stc15.h"
#include "header.h"
#define IR_Send_1 516
#define uchar unsigned char


void IR_text() ;
 void  IR_init(void);
void IRsend_order(uchar a,uchar b,uchar c,uchar d,uchar e,uchar f);

void IR_temper_change(unsigned char dat);
void Middle(unsigned int L, unsigned int H) ;
#endif
