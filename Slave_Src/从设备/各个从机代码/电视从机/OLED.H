/**************************************************

	文件名： OLED.H
	作  用： 源文件函数和变量的外部声明，及16X16格式字体及编码的定义
	版  本： V1.0
	作  者： FG
	时  间： 2015-2-7 23:55:37
***************************************************/

#ifndef  _OLED_H_
#define  _OLED_H_

//源文件函数与变量的外部声明
 void WriteDat(unsigned char Dat);
 void WriteCmd(unsigned char Cmd);
 void SetPos(unsigned char Page, unsigned char Column);
 void FillScreen(unsigned char Dat);
 void SetAddressMode(unsigned char  AddMode);	                 //取值为0~2
 void SetStartPage(unsigned char StartPage);	                 //用于页地址模式
 void SetStartColumn(unsigned char StartColumn);		         //用于页地址模式
 void SetPageAddress(unsigned char Start, unsigned char End);	 //注：该函数仅用于水平地址或垂直地址模式
 void SetColumnAddress(unsigned char Start, unsigned char End);//注：该函数仅用于水平地址或垂直地址模式
 void SetStartLine(unsigned char StartLine);	                 //取值：0~63
 void SetContrast(unsigned char ContrastValue);	             //取值范围为0~255共256级对比度
 void SetChargePump(unsigned char  on);	                     //0：关闭电荷泵；1：开启电荷泵
 void SetCommonRemap(unsigned char Dat);                       //设置COM输出的扫描方向
 void SetSegmentRemap(unsigned char Dat);	                     //设置segment的重映射
 void SetEntireDisplay(unsigned char OnMode);	                 // 0: 正常显示
 void SetInverseDisplay(unsigned char InverseValue);	         // 反白显示开/关
 void SetMultiplexRatio(unsigned char Ratio);	                 //取值：0~63
 void SetDisplayOnOff(unsigned char on);	                     // 设置显示开/关
 void SetDisplayOffset(unsigned char OffsetValue);
 void SetDisplayClock(unsigned char Value);
 void SetPrechargePeriod(unsigned char Value);                 // 设置预充电周期，缺省为0x22
extern void SetCommonConfig(unsigned char Dat);	                     // 缺省 => 0x12 (0x10)
 void SetVCOMH(unsigned char Dat);                             //Dat格式： 0xxx_0000
 void SetNop(void);
 void OLED_Init(void);                                         //oled初始化 
// void WriteChar_6x8(unsigned char Page,unsigned char Column, unsigned char Dat);
 void WriteStr_8x16(unsigned char Page, unsigned char Column, unsigned char Str[]);
 void WriteChinese16x16(unsigned char Page,unsigned char Column,unsigned char Str[]);
 void WriteMixStrXx16(unsigned char Page, unsigned char Column, unsigned char Str[]);
 void PrintChar(unsigned char Page, unsigned char Column, char Dat);
 void PrintShort(unsigned char Page, unsigned char Column, int Dat);
 void PrintImage(unsigned char ImageTable[], unsigned char RowNum, unsigned char ColumnNum);
 void ConHorScroll(unsigned char StartPage,unsigned char EndPage,bit Direction,unsigned char speed);
 void ConVerHorScroll(unsigned char StartPage,unsigned char EndPage,bit Direction,unsigned char speed,unsigned char VerOffset);
 void SetVerScrollArea(unsigned char FixRowNo,unsigned char ScrollRowNo);
 void PrintNum_6x8(unsigned char Page,unsigned char Column,unsigned char num);	  //num取值为0~9
 void PrintNum_8x16(unsigned char Page,unsigned char Column,unsigned char num);	  //num取值为0~9


//源文件变量的外部声明
 const unsigned char code F6x8[][6];
 const unsigned char code F8X16[][16];
 const unsigned char code font16x16[];
 const unsigned char code font16x16_code[];

#endif