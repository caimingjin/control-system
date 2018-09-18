/**************************************************

	文件名： OLED.H
	作  用： 源文件函数和变量的外部声明，及16X16格式字体及编码的定义
	版  本： V1.0
	作  者： FG
	时  间： 2015-2-7 23:55:37
***************************************************/

#ifndef  _OLED_H_
#define  _OLED_H_
#include "header.h"
//源文件函数与变量的外部声明
extern void WriteDat(unsigned char Dat);
extern void WriteCmd(unsigned char Cmd);
extern void SetPos(unsigned char Page, unsigned char Column);
extern void FillScreen(unsigned char Dat);
extern void SetAddressMode(unsigned char  AddMode);	                 //取值为0~2
extern void SetStartPage(unsigned char StartPage);	                 //用于页地址模式
extern void SetStartColumn(unsigned char StartColumn);		         //用于页地址模式
extern void SetPageAddress(unsigned char Start, unsigned char End);	 //注：该函数仅用于水平地址或垂直地址模式
extern void SetColumnAddress(unsigned char Start, unsigned char End);//注：该函数仅用于水平地址或垂直地址模式
extern void SetStartLine(unsigned char StartLine);	                 //取值：0~63
extern void SetContrast(unsigned char ContrastValue);	             //取值范围为0~255共256级对比度
extern void SetChargePump(unsigned char  on);	                     //0：关闭电荷泵；1：开启电荷泵
extern void SetCommonRemap(unsigned char Dat);                       //设置COM输出的扫描方向
extern void SetSegmentRemap(unsigned char Dat);	                     //设置segment的重映射
extern void SetEntireDisplay(unsigned char OnMode);	                 // 0: 正常显示
extern void SetInverseDisplay(unsigned char InverseValue);	         // 反白显示开/关
extern void SetMultiplexRatio(unsigned char Ratio);	                 //取值：0~63
extern void SetDisplayOnOff(unsigned char on);	                     // 设置显示开/关
extern void SetDisplayOffset(unsigned char OffsetValue);
extern void SetDisplayClock(unsigned char Value);
extern void SetPrechargePeriod(unsigned char Value);                 // 设置预充电周期，缺省为0x22
extern void SetCommonConfig(unsigned char Dat);	                     // 缺省 => 0x12 (0x10)
extern void SetVCOMH(unsigned char Dat);                             //Dat格式： 0xxx_0000
extern void SetNop(void);
extern void OLED_Init(void);                                         //oled初始化 
extern void WriteChar_6x8(unsigned char Page,unsigned char Column, unsigned char Dat);
extern void WriteStr_8x16(unsigned char Page, unsigned char Column, unsigned char Str[]);
extern void WriteChinese16x16(unsigned char Page,unsigned char Column,unsigned char Str[]);
extern void WriteMixStrXx16(unsigned char Page, unsigned char Column, unsigned char Str[]);
extern void PrintChar(unsigned char Page, unsigned char Column, char Dat);
extern void PrintShort(unsigned char Page, unsigned char Column, int Dat);
extern void PrintImage(unsigned char ImageTable[], unsigned char RowNum, unsigned char ColumnNum);
extern void ConHorScroll(unsigned char StartPage,unsigned char EndPage,bit Direction,unsigned char speed);
extern void ConVerHorScroll(unsigned char StartPage,unsigned char EndPage,bit Direction,unsigned char speed,unsigned char VerOffset);
extern void SetVerScrollArea(unsigned char FixRowNo,unsigned char ScrollRowNo);
extern void PrintNum_6x8(unsigned char Page,unsigned char Column,unsigned char num);	  //num取值为0~9
extern void PrintNum_8x16(unsigned char Page,unsigned char Column,unsigned char num);	  //num取值为0~9


//源文件变量的外部声明
extern const unsigned char code F6x8[][6];
extern const unsigned char code F8X16[][16];
extern const unsigned char code font16x16[];
extern const unsigned char code font16x16_code[];

#endif