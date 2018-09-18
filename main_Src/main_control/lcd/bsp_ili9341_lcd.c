/**
  ******************************************************************************
  * @file    bsp_ili9341_lcd.c
  * @version V1.0
  * @date    2013-xx-xx
  * @brief   ili9341液晶屏驱动
  ******************************************************************************
  * @attention
  *
  * 实验平台:秉火 F103-指南者 STM32 开发板 
  * 论坛    :http://www.firebbs.cn
  * 淘宝    :https://fire-stm32.taobao.com
  *
  ******************************************************************************
  */ 

#include "bsp_ili9341_lcd.h"
#include "fonts.h"	
#include "Slave.h"
#include <stdio.h>
#include <string.h>
//根据液晶扫描方向而变化的XY像素宽度
//调用ILI9341_GramScan函数设置方向时会自动更改
uint16_t LCD_X_LENGTH = ILI9341_LESS_PIXEL;
uint16_t LCD_Y_LENGTH = ILI9341_MORE_PIXEL;

//液晶屏扫描模式，本变量主要用于方便选择触摸屏的计算参数
//参数可选值为0-7
//调用ILI9341_GramScan函数设置方向时会自动更改
//LCD刚初始化完成时会使用本默认值
uint8_t LCD_SCAN_MODE = 6;


static sFONT *LCD_Currentfonts = &Font8x16;  //英文字体
static uint16_t CurrentTextColor   = BLACK;//前景色
static uint16_t CurrentBackColor   = WHITE;//背景色

__inline void                 ILI9341_Write_Cmd           ( uint16_t usCmd );
__inline void                 ILI9341_Write_Data          ( uint16_t usData );
__inline uint16_t             ILI9341_Read_Data           ( void );
static void                   ILI9341_Delay               ( __IO uint32_t nCount );
static void                   ILI9341_GPIO_Config         ( void );
static void                   ILI9341_FSMC_Config         ( void );
static void                   ILI9341_REG_Config          ( void );
static void                   ILI9341_SetCursor           ( uint16_t usX, uint16_t usY );
static __inline void          ILI9341_FillColor           ( uint32_t ulAmout_Point, uint16_t usColor );
static uint16_t               ILI9341_Read_PixelData      ( void );



#define AIR_Y_START  20
#define AIR_X_START  10

#define LIGHT_Y_START  120
#define LIGHT_X_START  10

#define TV_Y_START  200
#define TV_X_START  10

#define LINE_SPACE  20

unsigned char  font16x16[] = {"模式空调电视灯开关机个数状态当前频道制冷热抽湿自动温度℃："}; //这里只对汉字进行编码，请不要包含字符的编码
unsigned char font16x16_Library[]={
0x11,0x10,0x11,0x10,0x17,0xFC,0x11,0x10,0xFC,0x00,0x13,0xF8,0x32,0x08,0x3B,0xF8,
0x56,0x08,0x53,0xF8,0x90,0x40,0x17,0xFC,0x10,0xA0,0x11,0x10,0x12,0x08,0x14,0x06,//"模",0
0x00,0x48,0x00,0x44,0x00,0x44,0x00,0x40,0xFF,0xFE,0x00,0x40,0x00,0x40,0x3E,0x40,
0x08,0x40,0x08,0x40,0x08,0x20,0x08,0x22,0x0F,0x12,0x78,0x0A,0x20,0x06,0x00,0x02,//"式",1
0x02,0x00,0x01,0x00,0x7F,0xFE,0x40,0x02,0x88,0x24,0x10,0x10,0x20,0x08,0x00,0x00,
0x1F,0xF0,0x01,0x00,0x01,0x00,0x01,0x00,0x01,0x00,0x01,0x00,0x7F,0xFC,0x00,0x00,//"空",0
0x00,0x00,0x47,0xFC,0x24,0x44,0x24,0x44,0x05,0xF4,0x04,0x44,0xE4,0x44,0x27,0xFC,
0x24,0x04,0x25,0xF4,0x25,0x14,0x2D,0x14,0x35,0xF4,0x24,0x04,0x04,0x14,0x08,0x08,//"调",1
0x01,0x00,0x01,0x00,0x01,0x00,0x3F,0xF8,0x21,0x08,0x21,0x08,0x21,0x08,0x3F,0xF8,
0x21,0x08,0x21,0x08,0x21,0x08,0x3F,0xF8,0x21,0x0A,0x01,0x02,0x01,0x02,0x00,0xFE,//"电",2
0x20,0x00,0x11,0xFC,0x11,0x04,0xF9,0x04,0x09,0x24,0x11,0x24,0x11,0x24,0x39,0x24,
0x55,0x24,0x95,0x54,0x10,0x50,0x10,0x90,0x10,0x90,0x11,0x12,0x12,0x12,0x14,0x0E,//"视",3
0x10,0x00,0x10,0x00,0x13,0xFE,0x10,0x20,0x54,0x20,0x58,0x20,0x50,0x20,0x90,0x20,
0x10,0x20,0x10,0x20,0x10,0x20,0x28,0x20,0x24,0x20,0x44,0x20,0x40,0xA0,0x80,0x40,//"灯",4
0x00,0x00,0x7F,0xFC,0x08,0x20,0x08,0x20,0x08,0x20,0x08,0x20,0x08,0x20,0xFF,0xFE,
0x08,0x20,0x08,0x20,0x08,0x20,0x08,0x20,0x10,0x20,0x10,0x20,0x20,0x20,0x40,0x20,//"开",5
0x10,0x10,0x08,0x10,0x08,0x20,0x00,0x00,0x3F,0xF8,0x01,0x00,0x01,0x00,0x01,0x00,
0xFF,0xFE,0x01,0x00,0x02,0x80,0x02,0x80,0x04,0x40,0x08,0x20,0x30,0x18,0xC0,0x06,//"关",6
0x10,0x00,0x11,0xF0,0x11,0x10,0x11,0x10,0xFD,0x10,0x11,0x10,0x31,0x10,0x39,0x10,
0x55,0x10,0x55,0x10,0x91,0x10,0x11,0x12,0x11,0x12,0x12,0x12,0x12,0x0E,0x14,0x00,//"机",7
0x01,0x00,0x01,0x00,0x02,0x80,0x04,0x40,0x08,0x20,0x10,0x10,0x21,0x08,0xC1,0x06,
0x01,0x00,0x01,0x00,0x01,0x00,0x01,0x00,0x01,0x00,0x01,0x00,0x01,0x00,0x01,0x00,//"个",8
0x08,0x20,0x49,0x20,0x2A,0x20,0x08,0x3E,0xFF,0x44,0x2A,0x44,0x49,0x44,0x88,0xA4,
0x10,0x28,0xFE,0x28,0x22,0x10,0x42,0x10,0x64,0x28,0x18,0x28,0x34,0x44,0xC2,0x82,//"数",9
0x08,0x40,0x08,0x48,0x08,0x44,0x48,0x44,0x28,0x40,0x2F,0xFE,0x08,0x40,0x08,0x40,
0x18,0x40,0x28,0xA0,0xC8,0xA0,0x08,0x90,0x09,0x10,0x09,0x08,0x0A,0x04,0x0C,0x02,//"状",10
0x01,0x00,0x01,0x00,0x7F,0xFC,0x01,0x00,0x02,0x80,0x04,0x40,0x0A,0x20,0x31,0x18,
0xC0,0x06,0x01,0x00,0x08,0x88,0x48,0x84,0x48,0x12,0x48,0x12,0x87,0xF0,0x00,0x00,//"态",11
0x01,0x00,0x21,0x08,0x11,0x08,0x09,0x10,0x09,0x20,0x01,0x00,0x7F,0xF8,0x00,0x08,
0x00,0x08,0x00,0x08,0x3F,0xF8,0x00,0x08,0x00,0x08,0x00,0x08,0x7F,0xF8,0x00,0x08,//"当",12
0x10,0x10,0x08,0x10,0x08,0x20,0xFF,0xFE,0x00,0x00,0x3E,0x08,0x22,0x48,0x22,0x48,
0x3E,0x48,0x22,0x48,0x22,0x48,0x3E,0x48,0x22,0x08,0x22,0x08,0x2A,0x28,0x24,0x10,//"前",13
0x10,0x00,0x11,0xFE,0x50,0x20,0x5C,0x40,0x51,0xFC,0x51,0x04,0xFF,0x24,0x01,0x24,
0x11,0x24,0x55,0x24,0x55,0x24,0x55,0x44,0x84,0x50,0x08,0x88,0x31,0x04,0xC2,0x02,//"频",14
0x02,0x08,0x21,0x10,0x10,0x00,0x17,0xFC,0x00,0x80,0x03,0xF8,0xF2,0x08,0x13,0xF8,
0x12,0x08,0x13,0xF8,0x12,0x08,0x13,0xF8,0x12,0x08,0x28,0x00,0x47,0xFE,0x00,0x00,//"道",15
0x04,0x04,0x24,0x04,0x24,0x04,0x3F,0xA4,0x44,0x24,0x04,0x24,0xFF,0xE4,0x04,0x24,
0x04,0x24,0x3F,0xA4,0x24,0xA4,0x24,0xA4,0x26,0x84,0x25,0x04,0x04,0x14,0x04,0x08,//"制",16
0x00,0x40,0x40,0x40,0x20,0xA0,0x20,0xA0,0x01,0x10,0x02,0x48,0x14,0x26,0x10,0x20,
0x23,0xF8,0xE0,0x08,0x20,0x10,0x21,0x10,0x20,0xA0,0x20,0x40,0x20,0x20,0x00,0x20,//"冷",17
0x10,0x40,0x10,0x40,0x10,0x40,0xFD,0xF8,0x10,0x48,0x10,0x48,0x1C,0xC8,0x30,0x48,
0xD0,0xAA,0x10,0xAA,0x51,0x06,0x22,0x02,0x00,0x00,0x48,0x88,0x44,0x44,0x84,0x44,//"热",18
0x10,0x20,0x10,0x20,0x10,0x20,0x10,0x20,0xFD,0xFC,0x11,0x24,0x11,0x24,0x15,0x24,
0x19,0x24,0x31,0xFC,0xD1,0x24,0x11,0x24,0x11,0x24,0x11,0x24,0x51,0xFC,0x21,0x04,//"抽",19
0x00,0x00,0x27,0xF8,0x14,0x08,0x14,0x08,0x87,0xF8,0x44,0x08,0x44,0x08,0x17,0xF8,
0x11,0x20,0x21,0x20,0xE9,0x24,0x25,0x28,0x23,0x30,0x21,0x20,0x2F,0xFE,0x00,0x00,//"湿",20
0x01,0x00,0x02,0x00,0x04,0x00,0x1F,0xF0,0x10,0x10,0x10,0x10,0x10,0x10,0x1F,0xF0,
0x10,0x10,0x10,0x10,0x1F,0xF0,0x10,0x10,0x10,0x10,0x10,0x10,0x1F,0xF0,0x10,0x10,//"自",21
0x00,0x40,0x00,0x40,0x7C,0x40,0x00,0x40,0x01,0xFC,0x00,0x44,0xFE,0x44,0x20,0x44,
0x20,0x44,0x20,0x84,0x48,0x84,0x44,0x84,0xFD,0x04,0x45,0x04,0x02,0x28,0x04,0x10,//"动",22
0x00,0x00,0x23,0xF8,0x12,0x08,0x12,0x08,0x83,0xF8,0x42,0x08,0x42,0x08,0x13,0xF8,
0x10,0x00,0x27,0xFC,0xE4,0xA4,0x24,0xA4,0x24,0xA4,0x24,0xA4,0x2F,0xFE,0x00,0x00,//"温",23
0x01,0x00,0x00,0x80,0x3F,0xFE,0x22,0x20,0x22,0x20,0x3F,0xFC,0x22,0x20,0x22,0x20,
0x23,0xE0,0x20,0x00,0x2F,0xF0,0x24,0x10,0x42,0x20,0x41,0xC0,0x86,0x30,0x38,0x0E,//"度",24
0x60,0x00,0x91,0xF4,0x96,0x0C,0x6C,0x04,0x08,0x04,0x18,0x00,0x18,0x00,0x18,0x00,
0x18,0x00,0x18,0x00,0x18,0x00,0x08,0x00,0x0C,0x04,0x06,0x08,0x01,0xF0,0x00,0x00,//"℃",25

0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x30,0x00,0x30,0x00,0x00,0x00,0x30,0x00,0x30,0x00,0x00,0x00,0x00,0x00,//"：",0
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,//" ",1
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,//" ",2


};


/**
  * @brief  向ILI9341写入命令
  * @param  usCmd :要写入的命令（表寄存器地址）
  * @retval 无
  */	
__inline void ILI9341_Write_Cmd ( uint16_t usCmd )
{
	* ( __IO uint16_t * ) ( FSMC_Addr_ILI9341_CMD ) = usCmd;
	
}


/**
  * @brief  向ILI9341写入数据
  * @param  usData :要写入的数据
  * @retval 无
  */	
__inline void ILI9341_Write_Data ( uint16_t usData )
{
	* ( __IO uint16_t * ) ( FSMC_Addr_ILI9341_DATA ) = usData;
	
}


/**
  * @brief  从ILI9341读取数据
  * @param  无
  * @retval 读取到的数据
  */	
__inline uint16_t ILI9341_Read_Data ( void )
{
	return ( * ( __IO uint16_t * ) ( FSMC_Addr_ILI9341_DATA ) );
	
}


/**
  * @brief  用于 ILI9341 简单延时函数
  * @param  nCount ：延时计数值
  * @retval 无
  */	
static void ILI9341_Delay ( __IO uint32_t nCount )
{
  for ( ; nCount != 0; nCount -- );
	
}


/**
  * @brief  初始化ILI9341的IO引脚
  * @param  无
  * @retval 无
  */
static void ILI9341_GPIO_Config ( void )
{
	GPIO_InitTypeDef GPIO_InitStructure;

	/* 使能FSMC对应相应管脚时钟*/
	RCC_APB2PeriphClockCmd ( 	
													/*控制信号*/
													ILI9341_CS_CLK|ILI9341_DC_CLK|ILI9341_WR_CLK|
													ILI9341_RD_CLK	|ILI9341_BK_CLK|ILI9341_RST_CLK|
													/*数据信号*/
													ILI9341_D0_CLK|ILI9341_D1_CLK|	ILI9341_D2_CLK | 
													ILI9341_D3_CLK | ILI9341_D4_CLK|ILI9341_D5_CLK|
													ILI9341_D6_CLK | ILI9341_D7_CLK|ILI9341_D8_CLK|
													ILI9341_D9_CLK | ILI9341_D10_CLK|ILI9341_D11_CLK|
													ILI9341_D12_CLK | ILI9341_D13_CLK|ILI9341_D14_CLK|
													ILI9341_D15_CLK	, ENABLE );
		
	
	/* 配置FSMC相对应的数据线,FSMC-D0~D15 */	
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode =  GPIO_Mode_AF_PP;
	
	GPIO_InitStructure.GPIO_Pin = ILI9341_D0_PIN;
	GPIO_Init ( ILI9341_D0_PORT, & GPIO_InitStructure );

	GPIO_InitStructure.GPIO_Pin = ILI9341_D1_PIN;
	GPIO_Init ( ILI9341_D1_PORT, & GPIO_InitStructure );
	
	GPIO_InitStructure.GPIO_Pin = ILI9341_D2_PIN;
	GPIO_Init ( ILI9341_D2_PORT, & GPIO_InitStructure );
	
	GPIO_InitStructure.GPIO_Pin = ILI9341_D3_PIN;
	GPIO_Init ( ILI9341_D3_PORT, & GPIO_InitStructure );
	
	GPIO_InitStructure.GPIO_Pin = ILI9341_D4_PIN;
	GPIO_Init ( ILI9341_D4_PORT, & GPIO_InitStructure );
	
	GPIO_InitStructure.GPIO_Pin = ILI9341_D5_PIN;
	GPIO_Init ( ILI9341_D5_PORT, & GPIO_InitStructure );
	
	GPIO_InitStructure.GPIO_Pin = ILI9341_D6_PIN;
	GPIO_Init ( ILI9341_D6_PORT, & GPIO_InitStructure );
	
	GPIO_InitStructure.GPIO_Pin = ILI9341_D7_PIN;
	GPIO_Init ( ILI9341_D7_PORT, & GPIO_InitStructure );
	
	GPIO_InitStructure.GPIO_Pin = ILI9341_D8_PIN;
	GPIO_Init ( ILI9341_D8_PORT, & GPIO_InitStructure );
	
	GPIO_InitStructure.GPIO_Pin = ILI9341_D9_PIN;
	GPIO_Init ( ILI9341_D9_PORT, & GPIO_InitStructure );
	
	GPIO_InitStructure.GPIO_Pin = ILI9341_D10_PIN;
	GPIO_Init ( ILI9341_D10_PORT, & GPIO_InitStructure );
	
	GPIO_InitStructure.GPIO_Pin = ILI9341_D11_PIN;
	GPIO_Init ( ILI9341_D11_PORT, & GPIO_InitStructure );

	GPIO_InitStructure.GPIO_Pin = ILI9341_D12_PIN;
	GPIO_Init ( ILI9341_D12_PORT, & GPIO_InitStructure );	
	
	GPIO_InitStructure.GPIO_Pin = ILI9341_D13_PIN;
	GPIO_Init ( ILI9341_D13_PORT, & GPIO_InitStructure );
	
	GPIO_InitStructure.GPIO_Pin = ILI9341_D14_PIN;
	GPIO_Init ( ILI9341_D14_PORT, & GPIO_InitStructure );
	
	GPIO_InitStructure.GPIO_Pin = ILI9341_D15_PIN;
	GPIO_Init ( ILI9341_D15_PORT, & GPIO_InitStructure );
	

	
	/* 配置FSMC相对应的控制线
	 * FSMC_NOE   :LCD-RD
	 * FSMC_NWE   :LCD-WR
	 * FSMC_NE1   :LCD-CS
	 * FSMC_A16  	:LCD-DC
	 */
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode =  GPIO_Mode_AF_PP;
	
	GPIO_InitStructure.GPIO_Pin = ILI9341_RD_PIN; 
	GPIO_Init (ILI9341_RD_PORT, & GPIO_InitStructure );
	
	GPIO_InitStructure.GPIO_Pin = ILI9341_WR_PIN; 
	GPIO_Init (ILI9341_WR_PORT, & GPIO_InitStructure );
	
	GPIO_InitStructure.GPIO_Pin = ILI9341_CS_PIN; 
	GPIO_Init ( ILI9341_CS_PORT, & GPIO_InitStructure );  
	
	GPIO_InitStructure.GPIO_Pin = ILI9341_DC_PIN; 
	GPIO_Init ( ILI9341_DC_PORT, & GPIO_InitStructure );
	

  /* 配置LCD复位RST控制管脚*/
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_InitStructure.GPIO_Pin = ILI9341_RST_PIN; 
	GPIO_Init ( ILI9341_RST_PORT, & GPIO_InitStructure );
	
	
	/* 配置LCD背光控制管脚BK*/
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;  
	
	GPIO_InitStructure.GPIO_Pin = ILI9341_BK_PIN; 
	GPIO_Init ( ILI9341_BK_PORT, & GPIO_InitStructure );
}


 /**
  * @brief  LCD  FSMC 模式配置
  * @param  无
  * @retval 无
  */
static void ILI9341_FSMC_Config ( void )
{
	FSMC_NORSRAMInitTypeDef  FSMC_NORSRAMInitStructure;
	FSMC_NORSRAMTimingInitTypeDef  readWriteTiming; 	
	
	/* 使能FSMC时钟*/
	RCC_AHBPeriphClockCmd ( RCC_AHBPeriph_FSMC, ENABLE );

	//地址建立时间（ADDSET）为1个HCLK 2/72M=28ns
	readWriteTiming.FSMC_AddressSetupTime      = 0x01;	 //地址建立时间
	//数据保持时间（DATAST）+ 1个HCLK = 5/72M=70ns	
	readWriteTiming.FSMC_DataSetupTime         = 0x04;	 //数据建立时间
	//选择控制的模式
	//模式B,异步NOR FLASH模式，与ILI9341的8080时序匹配
	readWriteTiming.FSMC_AccessMode            = FSMC_AccessMode_B;	
	
	/*以下配置与模式B无关*/
	//地址保持时间（ADDHLD）模式A未用到
	readWriteTiming.FSMC_AddressHoldTime       = 0x00;	 //地址保持时间
	//设置总线转换周期，仅用于复用模式的NOR操作
	readWriteTiming.FSMC_BusTurnAroundDuration = 0x00;
	//设置时钟分频，仅用于同步类型的存储器
	readWriteTiming.FSMC_CLKDivision           = 0x00;
	//数据保持时间，仅用于同步型的NOR	
	readWriteTiming.FSMC_DataLatency           = 0x00;	

	
	FSMC_NORSRAMInitStructure.FSMC_Bank                  = FSMC_Bank1_NORSRAMx;
	FSMC_NORSRAMInitStructure.FSMC_DataAddressMux        = FSMC_DataAddressMux_Disable;
	FSMC_NORSRAMInitStructure.FSMC_MemoryType            = FSMC_MemoryType_NOR;
	FSMC_NORSRAMInitStructure.FSMC_MemoryDataWidth       = FSMC_MemoryDataWidth_16b;
	FSMC_NORSRAMInitStructure.FSMC_BurstAccessMode       = FSMC_BurstAccessMode_Disable;
	FSMC_NORSRAMInitStructure.FSMC_WaitSignalPolarity    = FSMC_WaitSignalPolarity_Low;
	FSMC_NORSRAMInitStructure.FSMC_WrapMode              = FSMC_WrapMode_Disable;
	FSMC_NORSRAMInitStructure.FSMC_WaitSignalActive      = FSMC_WaitSignalActive_BeforeWaitState;
	FSMC_NORSRAMInitStructure.FSMC_WriteOperation        = FSMC_WriteOperation_Enable;
	FSMC_NORSRAMInitStructure.FSMC_WaitSignal            = FSMC_WaitSignal_Disable;
	FSMC_NORSRAMInitStructure.FSMC_ExtendedMode          = FSMC_ExtendedMode_Disable;
	FSMC_NORSRAMInitStructure.FSMC_WriteBurst            = FSMC_WriteBurst_Disable;
	FSMC_NORSRAMInitStructure.FSMC_ReadWriteTimingStruct = &readWriteTiming;
	FSMC_NORSRAMInitStructure.FSMC_WriteTimingStruct     = &readWriteTiming;  
	
	FSMC_NORSRAMInit ( & FSMC_NORSRAMInitStructure ); 
	
	
	/* 使能 FSMC_Bank1_NORSRAM4 */
	FSMC_NORSRAMCmd ( FSMC_Bank1_NORSRAMx, ENABLE );  
		
		
}


/**
 * @brief  初始化ILI9341寄存器
 * @param  无
 * @retval 无
 */
static void ILI9341_REG_Config ( void )
{
	/*  Power control B (CFh)  */
	DEBUG_DELAY  ();
	ILI9341_Write_Cmd ( 0xCF  );
	ILI9341_Write_Data ( 0x00  );
	ILI9341_Write_Data ( 0x81  );
	ILI9341_Write_Data ( 0x30  );
	
	/*  Power on sequence control (EDh) */
	DEBUG_DELAY ();
	ILI9341_Write_Cmd ( 0xED );
	ILI9341_Write_Data ( 0x64 );
	ILI9341_Write_Data ( 0x03 );
	ILI9341_Write_Data ( 0x12 );
	ILI9341_Write_Data ( 0x81 );
	
	/*  Driver timing control A (E8h) */
	DEBUG_DELAY ();
	ILI9341_Write_Cmd ( 0xE8 );
	ILI9341_Write_Data ( 0x85 );
	ILI9341_Write_Data ( 0x10 );
	ILI9341_Write_Data ( 0x78 );
	
	/*  Power control A (CBh) */
	DEBUG_DELAY ();
	ILI9341_Write_Cmd ( 0xCB );
	ILI9341_Write_Data ( 0x39 );
	ILI9341_Write_Data ( 0x2C );
	ILI9341_Write_Data ( 0x00 );
	ILI9341_Write_Data ( 0x34 );
	ILI9341_Write_Data ( 0x02 );
	
	/* Pump ratio control (F7h) */
	DEBUG_DELAY ();
	ILI9341_Write_Cmd ( 0xF7 );
	ILI9341_Write_Data ( 0x20 );
	
	/* Driver timing control B */
	DEBUG_DELAY ();
	ILI9341_Write_Cmd ( 0xEA );
	ILI9341_Write_Data ( 0x00 );
	ILI9341_Write_Data ( 0x00 );
	
	/* Frame Rate Control (In Normal Mode/Full Colors) (B1h) */
	DEBUG_DELAY ();
	ILI9341_Write_Cmd ( 0xB1 );
	ILI9341_Write_Data ( 0x00 );
	ILI9341_Write_Data ( 0x1B );
	
	/*  Display Function Control (B6h) */
	DEBUG_DELAY ();
	ILI9341_Write_Cmd ( 0xB6 );
	ILI9341_Write_Data ( 0x0A );
	ILI9341_Write_Data ( 0xA2 );
	
	/* Power Control 1 (C0h) */
	DEBUG_DELAY ();
	ILI9341_Write_Cmd ( 0xC0 );
	ILI9341_Write_Data ( 0x35 );
	
	/* Power Control 2 (C1h) */
	DEBUG_DELAY ();
	ILI9341_Write_Cmd ( 0xC1 );
	ILI9341_Write_Data ( 0x11 );
	
	/* VCOM Control 1 (C5h) */
	ILI9341_Write_Cmd ( 0xC5 );
	ILI9341_Write_Data ( 0x45 );
	ILI9341_Write_Data ( 0x45 );
	
	/*  VCOM Control 2 (C7h)  */
	ILI9341_Write_Cmd ( 0xC7 );
	ILI9341_Write_Data ( 0xA2 );
	
	/* Enable 3G (F2h) */
	ILI9341_Write_Cmd ( 0xF2 );
	ILI9341_Write_Data ( 0x00 );
	
	/* Gamma Set (26h) */
	ILI9341_Write_Cmd ( 0x26 );
	ILI9341_Write_Data ( 0x01 );
	DEBUG_DELAY ();
	
	/* Positive Gamma Correction */
	ILI9341_Write_Cmd ( 0xE0 ); //Set Gamma
	ILI9341_Write_Data ( 0x0F );
	ILI9341_Write_Data ( 0x26 );
	ILI9341_Write_Data ( 0x24 );
	ILI9341_Write_Data ( 0x0B );
	ILI9341_Write_Data ( 0x0E );
	ILI9341_Write_Data ( 0x09 );
	ILI9341_Write_Data ( 0x54 );
	ILI9341_Write_Data ( 0xA8 );
	ILI9341_Write_Data ( 0x46 );
	ILI9341_Write_Data ( 0x0C );
	ILI9341_Write_Data ( 0x17 );
	ILI9341_Write_Data ( 0x09 );
	ILI9341_Write_Data ( 0x0F );
	ILI9341_Write_Data ( 0x07 );
	ILI9341_Write_Data ( 0x00 );
	
	/* Negative Gamma Correction (E1h) */
	ILI9341_Write_Cmd ( 0XE1 ); //Set Gamma
	ILI9341_Write_Data ( 0x00 );
	ILI9341_Write_Data ( 0x19 );
	ILI9341_Write_Data ( 0x1B );
	ILI9341_Write_Data ( 0x04 );
	ILI9341_Write_Data ( 0x10 );
	ILI9341_Write_Data ( 0x07 );
	ILI9341_Write_Data ( 0x2A );
	ILI9341_Write_Data ( 0x47 );
	ILI9341_Write_Data ( 0x39 );
	ILI9341_Write_Data ( 0x03 );
	ILI9341_Write_Data ( 0x06 );
	ILI9341_Write_Data ( 0x06 );
	ILI9341_Write_Data ( 0x30 );
	ILI9341_Write_Data ( 0x38 );
	ILI9341_Write_Data ( 0x0F );
	
	/* memory access control set */
	DEBUG_DELAY ();
	ILI9341_Write_Cmd ( 0x36 ); 	
	ILI9341_Write_Data ( 0xC8 );    /*竖屏  左上角到 (起点)到右下角 (终点)扫描方式*/
	DEBUG_DELAY ();
	
	/* column address control set */
	ILI9341_Write_Cmd ( CMD_SetCoordinateX ); 
	ILI9341_Write_Data ( 0x00 );
	ILI9341_Write_Data ( 0x00 );
	ILI9341_Write_Data ( 0x00 );
	ILI9341_Write_Data ( 0xEF );
	
	/* page address control set */
	DEBUG_DELAY ();
	ILI9341_Write_Cmd ( CMD_SetCoordinateY ); 
	ILI9341_Write_Data ( 0x00 );
	ILI9341_Write_Data ( 0x00 );
	ILI9341_Write_Data ( 0x01 );
	ILI9341_Write_Data ( 0x3F );
	
	/*  Pixel Format Set (3Ah)  */
	DEBUG_DELAY ();
	ILI9341_Write_Cmd ( 0x3a ); 
	ILI9341_Write_Data ( 0x55 );
	
	/* Sleep Out (11h)  */
	ILI9341_Write_Cmd ( 0x11 );	
	ILI9341_Delay ( 0xAFFf<<2 );
	DEBUG_DELAY ();
	
	/* Display ON (29h) */
	ILI9341_Write_Cmd ( 0x29 ); 
	
	
}


/**
 * @brief  ILI9341初始化函数，如果要用到lcd，一定要调用这个函数
 * @param  无
 * @retval 无
 */
void ILI9341_Init ( void )
{
	ILI9341_GPIO_Config ();
	ILI9341_FSMC_Config ();
	
	ILI9341_BackLed_Control ( ENABLE );      //点亮LCD背光灯
	ILI9341_Rst ();
	ILI9341_REG_Config ();
	
	//设置默认扫描方向，其中 6 模式为大部分液晶例程的默认显示方向  
	ILI9341_GramScan(LCD_SCAN_MODE);
}


/**
 * @brief  ILI9341背光LED控制
 * @param  enumState ：决定是否使能背光LED
  *   该参数为以下值之一：
  *     @arg ENABLE :使能背光LED
  *     @arg DISABLE :禁用背光LED
 * @retval 无
 */
void ILI9341_BackLed_Control ( FunctionalState enumState )
{
	if ( enumState )
		GPIO_ResetBits ( ILI9341_BK_PORT, ILI9341_BK_PIN );	
	else
		GPIO_SetBits ( ILI9341_BK_PORT, ILI9341_BK_PIN );
		
}



/**
 * @brief  ILI9341 软件复位
 * @param  无
 * @retval 无
 */
void ILI9341_Rst ( void )
{			
	GPIO_ResetBits ( ILI9341_RST_PORT, ILI9341_RST_PIN );	 //低电平复位

	ILI9341_Delay ( 0xAFF ); 					   

	GPIO_SetBits ( ILI9341_RST_PORT, ILI9341_RST_PIN );		 	 

	ILI9341_Delay ( 0xAFF ); 	
	
}




/**
 * @brief  设置ILI9341的GRAM的扫描方向 
 * @param  ucOption ：选择GRAM的扫描方向 
 *     @arg 0-7 :参数可选值为0-7这八个方向
 *
 *	！！！其中0、3、5、6 模式适合从左至右显示文字，
 *				不推荐使用其它模式显示文字	其它模式显示文字会有镜像效果			
 *		
 *	其中0、2、4、6 模式的X方向像素为240，Y方向像素为320
 *	其中1、3、5、7 模式下X方向像素为320，Y方向像素为240
 *
 *	其中 6 模式为大部分液晶例程的默认显示方向
 *	其中 3 模式为摄像头例程使用的方向
 *	其中 0 模式为BMP图片显示例程使用的方向
 *
 * @retval 无
 * @note  坐标图例：A表示向上，V表示向下，<表示向左，>表示向右
					X表示X轴，Y表示Y轴

------------------------------------------------------------
模式0：				.		模式1：		.	模式2：			.	模式3：					
					A		.					A		.		A					.		A									
					|		.					|		.		|					.		|							
					Y		.					X		.		Y					.		X					
					0		.					1		.		2					.		3					
	<--- X0 o		.	<----Y1	o		.		o 2X--->  .		o 3Y--->	
------------------------------------------------------------	
模式4：				.	模式5：			.	模式6：			.	模式7：					
	<--- X4 o		.	<--- Y5 o		.		o 6X--->  .		o 7Y--->	
					4		.					5		.		6					.		7	
					Y		.					X		.		Y					.		X						
					|		.					|		.		|					.		|							
					V		.					V		.		V					.		V		
---------------------------------------------------------				
											 LCD屏示例
								|-----------------|
								|			秉火Logo		|
								|									|
								|									|
								|									|
								|									|
								|									|
								|									|
								|									|
								|									|
								|-----------------|
								屏幕正面（宽240，高320）

 *******************************************************/
void ILI9341_GramScan ( uint8_t ucOption )
{	
	//参数检查，只可输入0-7
	if(ucOption >7 )
		return;
	
	//根据模式更新LCD_SCAN_MODE的值，主要用于触摸屏选择计算参数
	LCD_SCAN_MODE = ucOption;
	
	//根据模式更新XY方向的像素宽度
	if(ucOption%2 == 0)	
	{
		//0 2 4 6模式下X方向像素宽度为240，Y方向为320
		LCD_X_LENGTH = ILI9341_LESS_PIXEL;
		LCD_Y_LENGTH =	ILI9341_MORE_PIXEL;
	}
	else				
	{
		//1 3 5 7模式下X方向像素宽度为320，Y方向为240
		LCD_X_LENGTH = ILI9341_MORE_PIXEL;
		LCD_Y_LENGTH =	ILI9341_LESS_PIXEL; 
	}

	//0x36命令参数的高3位可用于设置GRAM扫描方向	
	ILI9341_Write_Cmd ( 0x36 ); 
	ILI9341_Write_Data ( 0x08 |(ucOption<<5));//根据ucOption的值设置LCD参数，共0-7种模式
	ILI9341_Write_Cmd ( CMD_SetCoordinateX ); 
	ILI9341_Write_Data ( 0x00 );		/* x 起始坐标高8位 */
	ILI9341_Write_Data ( 0x00 );		/* x 起始坐标低8位 */
	ILI9341_Write_Data ( ((LCD_X_LENGTH-1)>>8)&0xFF ); /* x 结束坐标高8位 */	
	ILI9341_Write_Data ( (LCD_X_LENGTH-1)&0xFF );				/* x 结束坐标低8位 */

	ILI9341_Write_Cmd ( CMD_SetCoordinateY ); 
	ILI9341_Write_Data ( 0x00 );		/* y 起始坐标高8位 */
	ILI9341_Write_Data ( 0x00 );		/* y 起始坐标低8位 */
	ILI9341_Write_Data ( ((LCD_Y_LENGTH-1)>>8)&0xFF );	/* y 结束坐标高8位 */	 
	ILI9341_Write_Data ( (LCD_Y_LENGTH-1)&0xFF );				/* y 结束坐标低8位 */

	/* write gram start */
	ILI9341_Write_Cmd ( CMD_SetPixel );	
}



/**
 * @brief  在ILI9341显示器上开辟一个窗口
 * @param  usX ：在特定扫描方向下窗口的起点X坐标
 * @param  usY ：在特定扫描方向下窗口的起点Y坐标
 * @param  usWidth ：窗口的宽度
 * @param  usHeight ：窗口的高度
 * @retval 无
 */
void ILI9341_OpenWindow ( uint16_t usX, uint16_t usY, uint16_t usWidth, uint16_t usHeight )
{	
	ILI9341_Write_Cmd ( CMD_SetCoordinateX ); 				 /* 设置X坐标 */
	ILI9341_Write_Data ( usX >> 8  );	 /* 先高8位，然后低8位 */
	ILI9341_Write_Data ( usX & 0xff  );	 /* 设置起始点和结束点*/
	ILI9341_Write_Data ( ( usX + usWidth - 1 ) >> 8  );
	ILI9341_Write_Data ( ( usX + usWidth - 1 ) & 0xff  );

	ILI9341_Write_Cmd ( CMD_SetCoordinateY ); 			     /* 设置Y坐标*/
	ILI9341_Write_Data ( usY >> 8  );
	ILI9341_Write_Data ( usY & 0xff  );
	ILI9341_Write_Data ( ( usY + usHeight - 1 ) >> 8 );
	ILI9341_Write_Data ( ( usY + usHeight - 1) & 0xff );
	
}


/**
 * @brief  设定ILI9341的光标坐标
 * @param  usX ：在特定扫描方向下光标的X坐标
 * @param  usY ：在特定扫描方向下光标的Y坐标
 * @retval 无
 */
static void ILI9341_SetCursor ( uint16_t usX, uint16_t usY )	
{
	ILI9341_OpenWindow ( usX, usY, 1, 1 );
}


/**
 * @brief  在ILI9341显示器上以某一颜色填充像素点
 * @param  ulAmout_Point ：要填充颜色的像素点的总数目
 * @param  usColor ：颜色
 * @retval 无
 */
static __inline void ILI9341_FillColor ( uint32_t ulAmout_Point, uint16_t usColor )
{
	uint32_t i = 0;
	
	
	/* memory write */
	ILI9341_Write_Cmd ( CMD_SetPixel );	
		
	for ( i = 0; i < ulAmout_Point; i ++ )
		ILI9341_Write_Data ( usColor );
		
	
}


/**
 * @brief  对ILI9341显示器的某一窗口以某种颜色进行清屏
 * @param  usX ：在特定扫描方向下窗口的起点X坐标
 * @param  usY ：在特定扫描方向下窗口的起点Y坐标
 * @param  usWidth ：窗口的宽度
 * @param  usHeight ：窗口的高度
 * @note 可使用LCD_SetBackColor、LCD_SetTextColor、LCD_SetColors函数设置颜色
 * @retval 无
 */
void ILI9341_Clear ( uint16_t usX, uint16_t usY, uint16_t usWidth, uint16_t usHeight )
{
	ILI9341_OpenWindow ( usX, usY, usWidth, usHeight );

	ILI9341_FillColor ( usWidth * usHeight, CurrentTextColor );		
	
}


/**
 * @brief  对ILI9341显示器的某一点以某种颜色进行填充
 * @param  usX ：在特定扫描方向下该点的X坐标
 * @param  usY ：在特定扫描方向下该点的Y坐标
 * @note 可使用LCD_SetBackColor、LCD_SetTextColor、LCD_SetColors函数设置颜色
 * @retval 无
 */
void ILI9341_SetPointPixel ( uint16_t usX, uint16_t usY )	
{	
	if ( ( usX < LCD_X_LENGTH ) && ( usY < LCD_Y_LENGTH ) )
  {
		ILI9341_SetCursor ( usX, usY );
		
		ILI9341_FillColor ( 1, CurrentTextColor );
	}
	
}


/**
 * @brief  读取ILI9341 GRAN 的一个像素数据
 * @param  无
 * @retval 像素数据
 */
static uint16_t ILI9341_Read_PixelData ( void )	
{	
	uint16_t usR=0, usG=0, usB=0 ;

	
	ILI9341_Write_Cmd ( 0x2E );   /* 读数据 */
	
	usR = ILI9341_Read_Data (); 	/*FIRST READ OUT DUMMY DATA*/
	
	usR = ILI9341_Read_Data ();  	/*READ OUT RED DATA  */
	usB = ILI9341_Read_Data ();  	/*READ OUT BLUE DATA*/
	usG = ILI9341_Read_Data ();  	/*READ OUT GREEN DATA*/	
	
  return ( ( ( usR >> 11 ) << 11 ) | ( ( usG >> 10 ) << 5 ) | ( usB >> 11 ) );
	
}


/**
 * @brief  获取 ILI9341 显示器上某一个坐标点的像素数据
 * @param  usX ：在特定扫描方向下该点的X坐标
 * @param  usY ：在特定扫描方向下该点的Y坐标
 * @retval 像素数据
 */
uint16_t ILI9341_GetPointPixel ( uint16_t usX, uint16_t usY )
{ 
	uint16_t usPixelData;

	
	ILI9341_SetCursor ( usX, usY );
	
	usPixelData = ILI9341_Read_PixelData ();
	
	return usPixelData;
	
}


/**
 * @brief  在 ILI9341 显示器上使用 Bresenham 算法画线段 
 * @param  usX1 ：在特定扫描方向下线段的一个端点X坐标
 * @param  usY1 ：在特定扫描方向下线段的一个端点Y坐标
 * @param  usX2 ：在特定扫描方向下线段的另一个端点X坐标
 * @param  usY2 ：在特定扫描方向下线段的另一个端点Y坐标
 * @note 可使用LCD_SetBackColor、LCD_SetTextColor、LCD_SetColors函数设置颜色
 * @retval 无
 */
void ILI9341_DrawLine ( uint16_t usX1, uint16_t usY1, uint16_t usX2, uint16_t usY2 )
{
	uint16_t us; 
	uint16_t usX_Current, usY_Current;
	
	int32_t lError_X = 0, lError_Y = 0, lDelta_X, lDelta_Y, lDistance; 
	int32_t lIncrease_X, lIncrease_Y; 	
	
	
	lDelta_X = usX2 - usX1; //计算坐标增量 
	lDelta_Y = usY2 - usY1; 
	
	usX_Current = usX1; 
	usY_Current = usY1; 
	
	
	if ( lDelta_X > 0 ) 
		lIncrease_X = 1; //设置单步方向 
	
	else if ( lDelta_X == 0 ) 
		lIncrease_X = 0;//垂直线 
	
	else 
  { 
    lIncrease_X = -1;
    lDelta_X = - lDelta_X;
  } 

	
	if ( lDelta_Y > 0 )
		lIncrease_Y = 1; 
	
	else if ( lDelta_Y == 0 )
		lIncrease_Y = 0;//水平线 
	
	else 
  {
    lIncrease_Y = -1;
    lDelta_Y = - lDelta_Y;
  } 

	
	if (  lDelta_X > lDelta_Y )
		lDistance = lDelta_X; //选取基本增量坐标轴 
	
	else 
		lDistance = lDelta_Y; 

	
	for ( us = 0; us <= lDistance + 1; us ++ )//画线输出 
	{  
		ILI9341_SetPointPixel ( usX_Current, usY_Current );//画点 
		
		lError_X += lDelta_X ; 
		lError_Y += lDelta_Y ; 
		
		if ( lError_X > lDistance ) 
		{ 
			lError_X -= lDistance; 
			usX_Current += lIncrease_X; 
		}  
		
		if ( lError_Y > lDistance ) 
		{ 
			lError_Y -= lDistance; 
			usY_Current += lIncrease_Y; 
		} 
		
	}  
	
	
}   


/**
 * @brief  在 ILI9341 显示器上画一个矩形
 * @param  usX_Start ：在特定扫描方向下矩形的起始点X坐标
 * @param  usY_Start ：在特定扫描方向下矩形的起始点Y坐标
 * @param  usWidth：矩形的宽度（单位：像素）
 * @param  usHeight：矩形的高度（单位：像素）
 * @param  ucFilled ：选择是否填充该矩形
  *   该参数为以下值之一：
  *     @arg 0 :空心矩形
  *     @arg 1 :实心矩形 
 * @note 可使用LCD_SetBackColor、LCD_SetTextColor、LCD_SetColors函数设置颜色
 * @retval 无
 */
void ILI9341_DrawRectangle ( uint16_t usX_Start, uint16_t usY_Start, uint16_t usWidth, uint16_t usHeight, uint8_t ucFilled )
{
	if ( ucFilled )
	{
		ILI9341_OpenWindow ( usX_Start, usY_Start, usWidth, usHeight );
		ILI9341_FillColor ( usWidth * usHeight ,CurrentTextColor);	
	}
	else
	{
		ILI9341_DrawLine ( usX_Start, usY_Start, usX_Start + usWidth - 1, usY_Start );
		ILI9341_DrawLine ( usX_Start, usY_Start + usHeight - 1, usX_Start + usWidth - 1, usY_Start + usHeight - 1 );
		ILI9341_DrawLine ( usX_Start, usY_Start, usX_Start, usY_Start + usHeight - 1 );
		ILI9341_DrawLine ( usX_Start + usWidth - 1, usY_Start, usX_Start + usWidth - 1, usY_Start + usHeight - 1 );		
	}

}


/**
 * @brief  在 ILI9341 显示器上使用 Bresenham 算法画圆
 * @param  usX_Center ：在特定扫描方向下圆心的X坐标
 * @param  usY_Center ：在特定扫描方向下圆心的Y坐标
 * @param  usRadius：圆的半径（单位：像素）
 * @param  ucFilled ：选择是否填充该圆
  *   该参数为以下值之一：
  *     @arg 0 :空心圆
  *     @arg 1 :实心圆
 * @note 可使用LCD_SetBackColor、LCD_SetTextColor、LCD_SetColors函数设置颜色
 * @retval 无
 */
void ILI9341_DrawCircle ( uint16_t usX_Center, uint16_t usY_Center, uint16_t usRadius, uint8_t ucFilled )
{
	int16_t sCurrentX, sCurrentY;
	int16_t sError;
	
	
	sCurrentX = 0; sCurrentY = usRadius;	  
	
	sError = 3 - ( usRadius << 1 );     //判断下个点位置的标志
	
	
	while ( sCurrentX <= sCurrentY )
	{
		int16_t sCountY;
		
		
		if ( ucFilled ) 			
			for ( sCountY = sCurrentX; sCountY <= sCurrentY; sCountY ++ ) 
			{                      
				ILI9341_SetPointPixel ( usX_Center + sCurrentX, usY_Center + sCountY );           //1，研究对象 
				ILI9341_SetPointPixel ( usX_Center - sCurrentX, usY_Center + sCountY );           //2       
				ILI9341_SetPointPixel ( usX_Center - sCountY,   usY_Center + sCurrentX );           //3
				ILI9341_SetPointPixel ( usX_Center - sCountY,   usY_Center - sCurrentX );           //4
				ILI9341_SetPointPixel ( usX_Center - sCurrentX, usY_Center - sCountY );           //5    
        ILI9341_SetPointPixel ( usX_Center + sCurrentX, usY_Center - sCountY );           //6
				ILI9341_SetPointPixel ( usX_Center + sCountY,   usY_Center - sCurrentX );           //7 	
        ILI9341_SetPointPixel ( usX_Center + sCountY,   usY_Center + sCurrentX );           //0				
			}
		
		else
		{          
			ILI9341_SetPointPixel ( usX_Center + sCurrentX, usY_Center + sCurrentY );             //1，研究对象
			ILI9341_SetPointPixel ( usX_Center - sCurrentX, usY_Center + sCurrentY );             //2      
			ILI9341_SetPointPixel ( usX_Center - sCurrentY, usY_Center + sCurrentX );             //3
			ILI9341_SetPointPixel ( usX_Center - sCurrentY, usY_Center - sCurrentX );             //4
			ILI9341_SetPointPixel ( usX_Center - sCurrentX, usY_Center - sCurrentY );             //5       
			ILI9341_SetPointPixel ( usX_Center + sCurrentX, usY_Center - sCurrentY );             //6
			ILI9341_SetPointPixel ( usX_Center + sCurrentY, usY_Center - sCurrentX );             //7 
			ILI9341_SetPointPixel ( usX_Center + sCurrentY, usY_Center + sCurrentX );             //0
    }			
		
		
		sCurrentX ++;

		
		if ( sError < 0 ) 
			sError += 4 * sCurrentX + 6;	  
		
		else
		{
			sError += 10 + 4 * ( sCurrentX - sCurrentY );   
			sCurrentY --;
		} 	
		
		
	}
	
	
}

/**
 * @brief  在 ILI9341 显示器上显示一个英文字符
 * @param  usX ：在特定扫描方向下字符的起始X坐标
 * @param  usY ：在特定扫描方向下该点的起始Y坐标
 * @param  cChar ：要显示的英文字符
 * @note 可使用LCD_SetBackColor、LCD_SetTextColor、LCD_SetColors函数设置颜色
 * @retval 无
 */
void ILI9341_DispChar_EN ( uint16_t usX, uint16_t usY, const char cChar )
{
	uint8_t  byteCount, bitCount,fontLength;	
	uint16_t ucRelativePositon;
	uint8_t *Pfont;
	
	//对ascii码表偏移（字模表不包含ASCII表的前32个非图形符号）
	ucRelativePositon = cChar - ' ';
	
	//每个字模的字节数
	fontLength = (LCD_Currentfonts->Width*LCD_Currentfonts->Height)/8;           //font:8x16
		
	//字模首地址
	/*ascii码表偏移值乘以每个字模的字节数，求出字模的偏移位置*/
	Pfont = (uint8_t *)&LCD_Currentfonts->table[ucRelativePositon * fontLength];
	
	//设置显示窗口
	ILI9341_OpenWindow ( usX, usY, LCD_Currentfonts->Width, LCD_Currentfonts->Height);
	
	ILI9341_Write_Cmd ( CMD_SetPixel );			

	//按字节读取字模数据
	//由于前面直接设置了显示窗口，显示数据会自动换行
	for ( byteCount = 0; byteCount < fontLength; byteCount++ )
	{
			//一位一位处理要显示的颜色
			for ( bitCount = 0; bitCount < 8; bitCount++ )
			{
					if ( Pfont[byteCount] & (0x80>>bitCount) )
						ILI9341_Write_Data ( CurrentTextColor );			
					else
						ILI9341_Write_Data ( CurrentBackColor );
			}	
	}	
}


/**
 * @brief  在 ILI9341 显示器上显示英文字符串
 * @param  line ：在特定扫描方向下字符串的起始Y坐标
  *   本参数可使用宏LINE(0)、LINE(1)等方式指定文字坐标，
  *   宏LINE(x)会根据当前选择的字体来计算Y坐标值。
	*		显示中文且使用LINE宏时，需要把英文字体设置成Font8x16
 * @param  pStr ：要显示的英文字符串的首地址
 * @note 可使用LCD_SetBackColor、LCD_SetTextColor、LCD_SetColors函数设置颜色
 * @retval 无
 */
void ILI9341_DispStringLine_EN (  uint16_t line,  char * pStr )
{
	uint16_t usX = 0;
	
	while ( * pStr != '\0' )
	{
		if ( ( usX - ILI9341_DispWindow_X_Star + LCD_Currentfonts->Width ) > LCD_X_LENGTH )
		{
			usX = ILI9341_DispWindow_X_Star;
			line += LCD_Currentfonts->Height;
		}
		
		if ( ( line - ILI9341_DispWindow_Y_Star + LCD_Currentfonts->Height ) > LCD_Y_LENGTH )
		{
			usX = ILI9341_DispWindow_X_Star;
			line = ILI9341_DispWindow_Y_Star;
		}
		
		ILI9341_DispChar_EN ( usX, line, * pStr);
		
		pStr ++;
		
		usX += LCD_Currentfonts->Width;
		
	}
	
}


/**
 * @brief  在 ILI9341 显示器上显示英文字符串
 * @param  usX ：在特定扫描方向下字符的起始X坐标
 * @param  usY ：在特定扫描方向下字符的起始Y坐标
 * @param  pStr ：要显示的英文字符串的首地址
 * @note 可使用LCD_SetBackColor、LCD_SetTextColor、LCD_SetColors函数设置颜色
 * @retval 无
 */
void ILI9341_DispString_EN ( 	uint16_t usX ,uint16_t usY,  char * pStr )
{
	while ( * pStr != '\0' )
	{
		if ( ( usX - ILI9341_DispWindow_X_Star + LCD_Currentfonts->Width ) > LCD_X_LENGTH )
		{
			usX = ILI9341_DispWindow_X_Star;
			usY += LCD_Currentfonts->Height;
		}
		
		if ( ( usY - ILI9341_DispWindow_Y_Star + LCD_Currentfonts->Height ) > LCD_Y_LENGTH )
		{
			usX = ILI9341_DispWindow_X_Star;
			usY = ILI9341_DispWindow_Y_Star;
		}
		
		ILI9341_DispChar_EN ( usX, usY, * pStr);
		
		pStr ++;
		
		usX += LCD_Currentfonts->Width;
		
	}
	
}


/**
 * @brief  在 ILI9341 显示器上显示英文字符串(沿Y轴方向)
 * @param  usX ：在特定扫描方向下字符的起始X坐标
 * @param  usY ：在特定扫描方向下字符的起始Y坐标
 * @param  pStr ：要显示的英文字符串的首地址
 * @note 可使用LCD_SetBackColor、LCD_SetTextColor、LCD_SetColors函数设置颜色
 * @retval 无
 */
void ILI9341_DispString_EN_YDir (	 uint16_t usX,uint16_t usY ,  char * pStr )
{	
	while ( * pStr != '\0' )
	{
		if ( ( usY - ILI9341_DispWindow_Y_Star + LCD_Currentfonts->Height ) >LCD_Y_LENGTH  )
		{
			usY = ILI9341_DispWindow_Y_Star;
			usX += LCD_Currentfonts->Width;
		}
		
		if ( ( usX - ILI9341_DispWindow_X_Star + LCD_Currentfonts->Width ) >  LCD_X_LENGTH)
		{
			usX = ILI9341_DispWindow_X_Star;
			usY = ILI9341_DispWindow_Y_Star;
		}
		
		ILI9341_DispChar_EN ( usX, usY, * pStr);
		
		pStr ++;
		
		usY += LCD_Currentfonts->Height;		
	}	
}

/**
 * @brief  在 ILI9341 显示器上显示一个中文字符
 * @param  usX ：在特定扫描方向下字符的起始X坐标
 * @param  usY ：在特定扫描方向下字符的起始Y坐标
 * @param  usChar ：要显示的中文字符（国标码）
 * @note 可使用LCD_SetBackColor、LCD_SetTextColor、LCD_SetColors函数设置颜色
 * @retval 无
 */ 
void ILI9341_DispChar_CH ( uint16_t usX, uint16_t usY, uint16_t usChar )
{
	uint8_t rowCount, bitCount;
	uint8_t ucBuffer [ WIDTH_CH_CHAR*HEIGHT_CH_CHAR/8 ];	
  uint16_t usTemp; 	

	//设置显示窗口
	ILI9341_OpenWindow ( usX, usY, WIDTH_CH_CHAR, HEIGHT_CH_CHAR );
	
	ILI9341_Write_Cmd ( CMD_SetPixel );
	
	//取字模数据  
  GetGBKCode ( ucBuffer, usChar );	
	
	for ( rowCount = 0; rowCount < HEIGHT_CH_CHAR; rowCount++ )
	{
    /* 取出两个字节的数据，在lcd上即是一个汉字的一行 */
		usTemp = ucBuffer [ rowCount * 2 ];
		usTemp = ( usTemp << 8 );
		usTemp |= ucBuffer [ rowCount * 2 + 1 ];
		
		for ( bitCount = 0; bitCount < WIDTH_CH_CHAR; bitCount ++ )
		{			
			if ( usTemp & ( 0x8000 >> bitCount ) )  //高位在前 
			  ILI9341_Write_Data ( CurrentTextColor );				
			else
				ILI9341_Write_Data ( CurrentBackColor );			
		}		
	}
	
}


/**
 * @brief  在 ILI9341 显示器上显示中文字符串
 * @param  line ：在特定扫描方向下字符串的起始Y坐标
  *   本参数可使用宏LINE(0)、LINE(1)等方式指定文字坐标，
  *   宏LINE(x)会根据当前选择的字体来计算Y坐标值。
	*		显示中文且使用LINE宏时，需要把英文字体设置成Font8x16
 * @param  pStr ：要显示的英文字符串的首地址
 * @note 可使用LCD_SetBackColor、LCD_SetTextColor、LCD_SetColors函数设置颜色
 * @retval 无
 */
void ILI9341_DispString_CH ( 	uint16_t usX , uint16_t usY, char * pStr )
{	
	uint16_t usCh;

	
	while( * pStr != '\0' )
	{		
		if ( ( usX - ILI9341_DispWindow_X_Star + WIDTH_CH_CHAR ) > LCD_X_LENGTH )
		{
			usX = ILI9341_DispWindow_X_Star;
			usY += HEIGHT_CH_CHAR;
		}
		
		if ( ( usY - ILI9341_DispWindow_Y_Star + HEIGHT_CH_CHAR ) > LCD_Y_LENGTH )
		{
			usX = ILI9341_DispWindow_X_Star;
			usY = ILI9341_DispWindow_Y_Star;
		}	
		
		usCh = * ( uint16_t * ) pStr;	
	  usCh = ( usCh << 8 ) + ( usCh >> 8 );

		ILI9341_DispChar_CH ( usX, usY, usCh );
		
		usX += WIDTH_CH_CHAR;
		
		pStr += 2;           //一个汉字两个字节 

	}	   
	
}


/**
 * @brief  在 ILI9341 显示器上显示中英文字符串
 * @param  line ：在特定扫描方向下字符串的起始Y坐标
  *   本参数可使用宏LINE(0)、LINE(1)等方式指定文字坐标，
  *   宏LINE(x)会根据当前选择的字体来计算Y坐标值。
	*		显示中文且使用LINE宏时，需要把英文字体设置成Font8x16
 * @param  pStr ：要显示的字符串的首地址
 * @note 可使用LCD_SetBackColor、LCD_SetTextColor、LCD_SetColors函数设置颜色
 * @retval 无
 */
void ILI9341_DispStringLine_EN_CH (  uint16_t line, char * pStr )
{
	uint16_t usCh;
	uint16_t usX = 0;
	
	while( * pStr != '\0' )
	{
		if ( * pStr <= 126 )	           	//英文字符
		{
			if ( ( usX - ILI9341_DispWindow_X_Star + LCD_Currentfonts->Width ) > LCD_X_LENGTH )
			{
				usX = ILI9341_DispWindow_X_Star;
				line += LCD_Currentfonts->Height;
			}
			
			if ( ( line - ILI9341_DispWindow_Y_Star + LCD_Currentfonts->Height ) > LCD_Y_LENGTH )
			{
				usX = ILI9341_DispWindow_X_Star;
				line = ILI9341_DispWindow_Y_Star;
			}			
		
		  ILI9341_DispChar_EN ( usX, line, * pStr );
			
			usX +=  LCD_Currentfonts->Width;
		
		  pStr ++;

		}
		
		else	                            //汉字字符
		{
			if ( ( usX - ILI9341_DispWindow_X_Star + WIDTH_CH_CHAR ) > LCD_X_LENGTH )
			{
				usX = ILI9341_DispWindow_X_Star;
				line += HEIGHT_CH_CHAR;
			}
			
			if ( ( line - ILI9341_DispWindow_Y_Star + HEIGHT_CH_CHAR ) > LCD_Y_LENGTH )
			{
				usX = ILI9341_DispWindow_X_Star;
				line = ILI9341_DispWindow_Y_Star;
			}	
			
			usCh = * ( uint16_t * ) pStr;	
			
			usCh = ( usCh << 8 ) + ( usCh >> 8 );		

			ILI9341_DispChar_CH ( usX, line, usCh );
			
			usX += WIDTH_CH_CHAR;
			
			pStr += 2;           //一个汉字两个字节 
		
    }
		
  }	
} 

/**
 * @brief  在 ILI9341 显示器上显示中英文字符串
 * @param  usX ：在特定扫描方向下字符的起始X坐标
 * @param  usY ：在特定扫描方向下字符的起始Y坐标
 * @param  pStr ：要显示的字符串的首地址
 * @note 可使用LCD_SetBackColor、LCD_SetTextColor、LCD_SetColors函数设置颜色
 * @retval 无
 */
void ILI9341_DispString_EN_CH ( 	uint16_t usX , uint16_t usY, char * pStr )
{
	uint16_t usCh;
	
	while( * pStr != '\0' )
	{
		if ( * pStr <= 126 )	           	//英文字符
		{
			if ( ( usX - ILI9341_DispWindow_X_Star + LCD_Currentfonts->Width ) > LCD_X_LENGTH )
			{
				usX = ILI9341_DispWindow_X_Star;
				usY += LCD_Currentfonts->Height;
			}
			
			if ( ( usY - ILI9341_DispWindow_Y_Star + LCD_Currentfonts->Height ) > LCD_Y_LENGTH )
			{
				usX = ILI9341_DispWindow_X_Star;
				usY = ILI9341_DispWindow_Y_Star;
			}			
		
		  ILI9341_DispChar_EN ( usX, usY, * pStr );
			
			usX +=  LCD_Currentfonts->Width;
		
		  pStr ++;

		}
		
		else	                            //汉字字符
		{
			if ( ( usX - ILI9341_DispWindow_X_Star + WIDTH_CH_CHAR ) > LCD_X_LENGTH )
			{
				usX = ILI9341_DispWindow_X_Star;
				usY += HEIGHT_CH_CHAR;
			}
			
			if ( ( usY - ILI9341_DispWindow_Y_Star + HEIGHT_CH_CHAR ) > LCD_Y_LENGTH )
			{
				usX = ILI9341_DispWindow_X_Star;
				usY = ILI9341_DispWindow_Y_Star;
			}	
			
			usCh = * ( uint16_t * ) pStr;	
			
			usCh = ( usCh << 8 ) + ( usCh >> 8 );		

			ILI9341_DispChar_CH ( usX, usY, usCh );
			
			usX += WIDTH_CH_CHAR;
			
			pStr += 2;           //一个汉字两个字节 
		
    }
		
  }	
} 

/**
 * @brief  在 ILI9341 显示器上显示中英文字符串(沿Y轴方向)
 * @param  usX ：在特定扫描方向下字符的起始X坐标
 * @param  usY ：在特定扫描方向下字符的起始Y坐标
 * @param  pStr ：要显示的中英文字符串的首地址
 * @note 可使用LCD_SetBackColor、LCD_SetTextColor、LCD_SetColors函数设置颜色
 * @retval 无
 */
void ILI9341_DispString_EN_CH_YDir (  uint16_t usX,uint16_t usY , char * pStr )
{
	uint16_t usCh;
	
	while( * pStr != '\0' )
	{			
			//统一使用汉字的宽高来计算换行
			if ( ( usY - ILI9341_DispWindow_Y_Star + HEIGHT_CH_CHAR ) >LCD_Y_LENGTH  )
			{
				usY = ILI9341_DispWindow_Y_Star;
				usX += WIDTH_CH_CHAR;
			}			
			if ( ( usX - ILI9341_DispWindow_X_Star + WIDTH_CH_CHAR ) >  LCD_X_LENGTH)
			{
				usX = ILI9341_DispWindow_X_Star;
				usY = ILI9341_DispWindow_Y_Star;
			}
			
		//显示	
		if ( * pStr <= 126 )	           	//英文字符
		{			
			ILI9341_DispChar_EN ( usX, usY, * pStr);
			
			pStr ++;
			
			usY += HEIGHT_CH_CHAR;		
		}
		else	                            //汉字字符
		{			
			usCh = * ( uint16_t * ) pStr;	
			
			usCh = ( usCh << 8 ) + ( usCh >> 8 );		

			ILI9341_DispChar_CH ( usX,usY , usCh );
			
			usY += HEIGHT_CH_CHAR;
			
			pStr += 2;           //一个汉字两个字节 
		
    }
		
  }	
} 

/***********************缩放字体****************************/
#define ZOOMMAXBUFF 500
uint8_t zoomBuff[ZOOMMAXBUFF] = {0};	//用于缩放的缓存，最大支持到128*128
uint8_t zoomTempBuff[500] = {0};

/**
 * @brief  缩放字模，缩放后的字模由1个像素点由8个数据位来表示
										0x01表示笔迹，0x00表示空白区
 * @param  in_width ：原始字符宽度
 * @param  in_heig ：原始字符高度
 * @param  out_width ：缩放后的字符宽度
 * @param  out_heig：缩放后的字符高度
 * @param  in_ptr ：字库输入指针	注意：1pixel 1bit
 * @param  out_ptr ：缩放后的字符输出指针 注意: 1pixel 8bit
 *		out_ptr实际上没有正常输出，改成了直接输出到全局指针zoomBuff中
 * @param  en_cn ：0为英文，1为中文
 * @retval 无
 */
void ILI9341_zoomChar(uint16_t in_width,	//原始字符宽度
									uint16_t in_heig,		//原始字符高度
									uint16_t out_width,	//缩放后的字符宽度
									uint16_t out_heig,	//缩放后的字符高度
									uint8_t *in_ptr,	//字库输入指针	注意：1pixel 1bit
									uint8_t *out_ptr, //缩放后的字符输出指针 注意: 1pixel 8bit
									uint8_t en_cn)		//0为英文，1为中文	
{
	uint8_t *pts,*ots;
	//根据源字模及目标字模大小，设定运算比例因子，左移16是为了把浮点运算转成定点运算
	unsigned int xrIntFloat_16=(in_width<<16)/out_width+1; 
  unsigned int yrIntFloat_16=(in_heig<<16)/out_heig+1;
	
	unsigned int srcy_16=0;
	unsigned int y,x;
	uint8_t *pSrcLine;
	
	uint16_t byteCount,bitCount;
	
	//检查参数是否合法
	if(in_width >= 32) return;												//字库不允许超过32像素
	if(in_width * in_heig == 0) return;	
	if(in_width * in_heig >= 1024 ) return; 					//限制输入最大 32*32
	
	if(out_width * out_heig == 0) return;	
	if(out_width * out_heig >= ZOOMMAXBUFF ) return; //限制最大缩放 128*128
	pts = (uint8_t*)&zoomTempBuff;
	
	//为方便运算，字库的数据由1 pixel/1bit 映射到1pixel/8bit
	//0x01表示笔迹，0x00表示空白区
	if(en_cn == 0x00)//英文
	{
		//英文和中文字库上下边界不对，可在此处调整。需要注意tempBuff防止溢出
			for(byteCount=0;byteCount<in_heig*in_width/8;byteCount++)	
			{
				for(bitCount=0;bitCount<8;bitCount++)
					{						
						//把源字模数据由位映射到字节
						//in_ptr里bitX为1，则pts里整个字节值为1
						//in_ptr里bitX为0，则pts里整个字节值为0
						*pts++ = (in_ptr[byteCount] & (0x80>>bitCount))?1:0; 
					}
			}				
	}
	else //中文
	{			
			for(byteCount=0;byteCount<in_heig*in_width/8;byteCount++)	
			{
				for(bitCount=0;bitCount<8;bitCount++)
					{						
						//把源字模数据由位映射到字节
						//in_ptr里bitX为1，则pts里整个字节值为1
						//in_ptr里bitX为0，则pts里整个字节值为0
						*pts++ = (in_ptr[byteCount] & (0x80>>bitCount))?1:0; 
					}
			}		
	}

	//zoom过程
	pts = (uint8_t*)&zoomTempBuff;	//映射后的源数据指针
	ots = (uint8_t*)&zoomBuff;	//输出数据的指针
	for (y=0;y<out_heig;y++)	/*行遍历*/
    {
				unsigned int srcx_16=0;
        pSrcLine=pts+in_width*(srcy_16>>16);				
        for (x=0;x<out_width;x++) /*行内像素遍历*/
        {
            ots[x]=pSrcLine[srcx_16>>16]; //把源字模数据复制到目标指针中
            srcx_16+=xrIntFloat_16;			//按比例偏移源像素点
        }
        srcy_16+=yrIntFloat_16;				  //按比例偏移源像素点
        ots+=out_width;						
    }
	/*！！！缩放后的字模数据直接存储到全局指针zoomBuff里了*/
	out_ptr = (uint8_t*)&zoomBuff;	//out_ptr没有正确传出，后面调用直接改成了全局变量指针！
	
	/*实际中如果使用out_ptr不需要下面这一句！！！
		只是因为out_ptr没有使用，会导致warning。强迫症*/
	out_ptr++; 
}			


/**
 * @brief  利用缩放后的字模显示字符
 * @param  Xpos ：字符显示位置x
 * @param  Ypos ：字符显示位置y
 * @param  Font_width ：字符宽度
 * @param  Font_Heig：字符高度
 * @param  c ：要显示的字模数据
 * @param  DrawModel ：是否反色显示 
 * @retval 无
 */
void ILI9341_DrawChar_Ex(uint16_t usX, //字符显示位置x
												uint16_t usY, //字符显示位置y
												uint16_t Font_width, //字符宽度
												uint16_t Font_Height,  //字符高度 
												uint8_t *c,						//字模数据
												uint16_t DrawModel)		//是否反色显示
{
  uint32_t index = 0, counter = 0;

	//设置显示窗口
	ILI9341_OpenWindow ( usX, usY, Font_width, Font_Height);
	
	ILI9341_Write_Cmd ( CMD_SetPixel );		
	
	//按字节读取字模数据
	//由于前面直接设置了显示窗口，显示数据会自动换行
	for ( index = 0; index < Font_Height; index++ )
	{
			//一位一位处理要显示的颜色
			for ( counter = 0; counter < Font_width; counter++ )
			{
					//缩放后的字模数据，以一个字节表示一个像素位
					//整个字节值为1表示该像素为笔迹
					//整个字节值为0表示该像素为背景
					if ( *c++ == DrawModel )
						ILI9341_Write_Data ( CurrentBackColor );			
					else
						ILI9341_Write_Data ( CurrentTextColor );
			}	
	}	
}


/**
 * @brief  利用缩放后的字模显示字符串
 * @param  Xpos ：字符显示位置x
 * @param  Ypos ：字符显示位置y
 * @param  Font_width ：字符宽度，英文字符在此基础上/2。注意为偶数
 * @param  Font_Heig：字符高度，注意为偶数
 * @param  c ：要显示的字符串
 * @param  DrawModel ：是否反色显示 
 * @retval 无
 */
void ILI9341_DisplayStringEx(uint16_t x, 		//字符显示位置x
														 uint16_t y, 				//字符显示位置y
														 uint16_t Font_width,	//要显示的字体宽度，英文字符在此基础上/2。注意为偶数
														 uint16_t Font_Height,	//要显示的字体高度，注意为偶数
														 uint8_t *ptr,					//显示的字符内容
														 uint16_t DrawModel)  //是否反色显示



{
	uint16_t Charwidth = Font_width; //默认为Font_width，英文宽度为中文宽度的一半
	uint8_t *psr;
	uint8_t Ascii;	//英文
	uint16_t usCh;  //中文
	uint8_t ucBuffer [ WIDTH_CH_CHAR*HEIGHT_CH_CHAR/8 ];	
	
	while ( *ptr != '\0')
	{
			/****处理换行*****/
			if ( ( x - ILI9341_DispWindow_X_Star + Charwidth ) > LCD_X_LENGTH )
			{
				x = ILI9341_DispWindow_X_Star;
				y += Font_Height;
			}
			
			if ( ( y - ILI9341_DispWindow_Y_Star + Font_Height ) > LCD_Y_LENGTH )
			{
				x = ILI9341_DispWindow_X_Star;
				y = ILI9341_DispWindow_Y_Star;
			}	
			
		if(*ptr > 0x80) //如果是中文
		{			
			Charwidth = Font_width;
			usCh = * ( uint16_t * ) ptr;				
			usCh = ( usCh << 8 ) + ( usCh >> 8 );
			GetGBKCode ( ucBuffer, usCh );	//取字模数据
			//缩放字模数据，源字模为16*16
			ILI9341_zoomChar(WIDTH_CH_CHAR,HEIGHT_CH_CHAR,Charwidth,Font_Height,(uint8_t *)&ucBuffer,psr,1); 
			//显示单个字符
			ILI9341_DrawChar_Ex(x,y,Charwidth,Font_Height,(uint8_t*)&zoomBuff,DrawModel);
			x+=Charwidth;
			ptr+=2;
		}
		else
		{
				Charwidth = Font_width / 2;
				Ascii = *ptr - 32;
				//使用16*24字体缩放字模数据
				ILI9341_zoomChar(16,24,Charwidth,Font_Height,(uint8_t *)&Font16x24.table[Ascii * Font16x24.Height*Font16x24.Width/8],psr,0);
			  //显示单个字符
				ILI9341_DrawChar_Ex(x,y,Charwidth,Font_Height,(uint8_t*)&zoomBuff,DrawModel);
				x+=Charwidth;
				ptr++;
		}
	}
}


/**
 * @brief  利用缩放后的字模显示字符串(沿Y轴方向)
 * @param  Xpos ：字符显示位置x
 * @param  Ypos ：字符显示位置y
 * @param  Font_width ：字符宽度，英文字符在此基础上/2。注意为偶数
 * @param  Font_Heig：字符高度，注意为偶数
 * @param  c ：要显示的字符串
 * @param  DrawModel ：是否反色显示 
 * @retval 无
 */
void ILI9341_DisplayStringEx_YDir(uint16_t x, 		//字符显示位置x
																		 uint16_t y, 				//字符显示位置y
																		 uint16_t Font_width,	//要显示的字体宽度，英文字符在此基础上/2。注意为偶数
																		 uint16_t Font_Height,	//要显示的字体高度，注意为偶数
																		 uint8_t *ptr,					//显示的字符内容
																		 uint16_t DrawModel)  //是否反色显示
{
	uint16_t Charwidth = Font_width; //默认为Font_width，英文宽度为中文宽度的一半
	uint8_t *psr;
	uint8_t Ascii;	//英文
	uint16_t usCh;  //中文
	uint8_t ucBuffer [ WIDTH_CH_CHAR*HEIGHT_CH_CHAR/8 ];	
	
	while ( *ptr != '\0')
	{			
			//统一使用汉字的宽高来计算换行
			if ( ( y - ILI9341_DispWindow_X_Star + Font_width ) > LCD_X_LENGTH )
			{
				y = ILI9341_DispWindow_X_Star;
				x += Font_width;
			}
			
			if ( ( x - ILI9341_DispWindow_Y_Star + Font_Height ) > LCD_Y_LENGTH )
			{
				y = ILI9341_DispWindow_X_Star;
				x = ILI9341_DispWindow_Y_Star;
			}	
			
		if(*ptr > 0x80) //如果是中文
		{			
			Charwidth = Font_width;
			usCh = * ( uint16_t * ) ptr;				
			usCh = ( usCh << 8 ) + ( usCh >> 8 );
			GetGBKCode ( ucBuffer, usCh );	//取字模数据
			//缩放字模数据，源字模为16*16
			ILI9341_zoomChar(WIDTH_CH_CHAR,HEIGHT_CH_CHAR,Charwidth,Font_Height,(uint8_t *)&ucBuffer,psr,1); 
			//显示单个字符
			ILI9341_DrawChar_Ex(x,y,Charwidth,Font_Height,(uint8_t*)&zoomBuff,DrawModel);
			y+=Font_Height;
			ptr+=2;
		}
		else
		{
				Charwidth = Font_width / 2;
				Ascii = *ptr - 32;
				//使用16*24字体缩放字模数据
				ILI9341_zoomChar(16,24,Charwidth,Font_Height,(uint8_t *)&Font16x24.table[Ascii * Font16x24.Height*Font16x24.Width/8],psr,0);
			  //显示单个字符
				ILI9341_DrawChar_Ex(x,y,Charwidth,Font_Height,(uint8_t*)&zoomBuff,DrawModel);
				y+=Font_Height;
				ptr++;
		}
	}
}


/**
  * @brief  设置英文字体类型
  * @param  fonts: 指定要选择的字体
	*		参数为以下值之一
  * 	@arg：Font24x32;
  * 	@arg：Font16x24;
  * 	@arg：Font8x16;
  * @retval None
  */
void LCD_SetFont(sFONT *fonts)
{
  LCD_Currentfonts = fonts;
}

/**
  * @brief  获取当前字体类型
  * @param  None.
  * @retval 返回当前字体类型
  */
sFONT *LCD_GetFont(void)
{
  return LCD_Currentfonts;
}


/**
  * @brief  设置LCD的前景(字体)及背景颜色,RGB565
  * @param  TextColor: 指定前景(字体)颜色
  * @param  BackColor: 指定背景颜色
  * @retval None
  */
void LCD_SetColors(uint16_t TextColor, uint16_t BackColor) 
{
  CurrentTextColor = TextColor; 
  CurrentBackColor = BackColor;
}

/**
  * @brief  获取LCD的前景(字体)及背景颜色,RGB565
  * @param  TextColor: 用来存储前景(字体)颜色的指针变量
  * @param  BackColor: 用来存储背景颜色的指针变量
  * @retval None
  */
void LCD_GetColors(uint16_t *TextColor, uint16_t *BackColor)
{
  *TextColor = CurrentTextColor;
  *BackColor = CurrentBackColor;
}

/**
  * @brief  设置LCD的前景(字体)颜色,RGB565
  * @param  Color: 指定前景(字体)颜色 
  * @retval None
  */
void LCD_SetTextColor(uint16_t Color)
{
  CurrentTextColor = Color;
}

/**
  * @brief  设置LCD的背景颜色,RGB565
  * @param  Color: 指定背景颜色 
  * @retval None
  */
void LCD_SetBackColor(uint16_t Color)
{
  CurrentBackColor = Color;
}

/**
  * @brief  清除某行文字
  * @param  Line: 指定要删除的行
  *   本参数可使用宏LINE(0)、LINE(1)等方式指定要删除的行，
  *   宏LINE(x)会根据当前选择的字体来计算Y坐标值，并删除当前字体高度的第x行。
  * @retval None
  */
void LCD_ClearLine(uint16_t Line)
{
  ILI9341_Clear(0,Line,LCD_X_LENGTH,((sFONT *)LCD_GetFont())->Height);	/* 清屏，显示全黑 */

}


//unsigned char arr[]={0+0x30,9+0x30,0x41,0x44};
//void Display()
//{
//	unsigned char a=0xff;
//	Display_Chinese(100,100,"电视开机关机");
//	ILI9341_DisplayStringEx(100,20,16,16,arr,0);
//	//	ILI9341_zoomChar(16,16,16,16,(uint8_t *)&dat1,dat1,1); 	
////	ILI9341_DrawChar_Ex(100,100,16,16,(uint8_t*)&zoomBuff,0);
//}
void Display_Chinese(unsigned char X,unsigned char Y,unsigned char *Str)
{
	unsigned char buff[500],i,j,m;
		unsigned int Address;
	
    for(j=0;Str[j]!='\0';)
{	
		i=0;
		while(font16x16[i] > 127)  //ASCII码为7位二进制表示的，因此最大为127,若大于127,已说明其不是ASCII码了。
													 //汉字表示需两个字节，为兼容ASCII码，计算机中用内码表示，两个字节最高位均为1，即大于127
		{
				if(font16x16[i] == Str[j])		   //若汉字数组中第i个汉字与实参第j个字的第一个字节相同
				{
						if(font16x16[i+1] == Str[j+1]) //且汉字数组中第i个汉字与实参第j个字的第二个字节也相同
						{
								Address = i*16;			   //则实际第j个汉字就采用font16*16的第i个汉字的编码
								break;
						}
				}
				i += 2;            				   //汉字索引前移一个,指向汉字数组中的下一个汉字
		}	
				// 所明汉字的字符串非空，或表示实参的编码可以在汉字数组对应编码中找到	
        if(Address != 1)  
				{
					for(m=0;m<32;m++)
					{
						buff[m]=font16x16_Library[Address];
						Address++;
					}
					j+=2;
				}					
		ILI9341_zoomChar(16,16,16,16,(uint8_t *)&buff,buff,1); 	
		ILI9341_DrawChar_Ex(X+j*8,Y,16,16,(uint8_t*)&zoomBuff,0);
	
}

}
void LCD_Test(void)
{
	 
	  	unsigned char dispBuff[4]={0};
//Display_Chinese(AIR_X_START,AIR_Y_START+LINE_SPACE,"空调模式： 自动");
//	ILI9341_DisplayStringEx(TV_X_START+100,TV_Y_START+LINE_SPACE,16,16,dispBuff,0);
			if(	Slave_adrrA_AIR_BUFF[0]!=0x00)
			{
					 //ILI9341_DispStringLine_EN_CH(LINE(0),"空调状态：     ");
					 Display_Chinese(AIR_X_START,AIR_Y_START,"空调状态：开机");
						switch(Slave_adrrA_AIR_BUFF[1])
						{
							case 0:   Display_Chinese(AIR_X_START,AIR_Y_START+LINE_SPACE*2,"空调模式：自动");
												break;
							case 1:   Display_Chinese(AIR_X_START,AIR_Y_START+LINE_SPACE*2,"空调模式：制冷");
												break;
							case 2:   Display_Chinese(AIR_X_START,AIR_Y_START+LINE_SPACE*2,"空调模式：抽湿");
												break;
							case 3:   Display_Chinese(AIR_X_START,AIR_Y_START+LINE_SPACE*2,"空调模式：制热");
												break;
							default: Display_Chinese(AIR_X_START,AIR_Y_START+LINE_SPACE*2,"空调模式：自动");
						}
							 dispBuff[0]=(Slave_adrrA_AIR_BUFF[2]+17)/10+0x30;
								dispBuff[1]=(Slave_adrrA_AIR_BUFF[2]+17)%10+0x30;
								dispBuff[2]='\0';
							ILI9341_DisplayStringEx(AIR_X_START+110,AIR_Y_START+LINE_SPACE,16,16,dispBuff,0);
						 // sprintf(dispBuff,"空调温度： %d℃",)
		   }
			else
			{
				 Display_Chinese(AIR_X_START,AIR_Y_START,"空调状态：关机");
				Display_Chinese(AIR_X_START,AIR_Y_START+LINE_SPACE*2,"空调模式：");
				 //ILI9341_DispStringLine_EN_CH(LINE(4),"空调温度：     ");
				 Display_Chinese(AIR_X_START,AIR_Y_START+LINE_SPACE,"空调温度℃：");
			}
			if(Slave_adrrB_Light_BUFF[0]!=0x00)
			{
					Display_Chinese(LIGHT_X_START,LIGHT_Y_START,"灯状态：开灯");
				Display_Chinese(LIGHT_X_START,LIGHT_Y_START+LINE_SPACE,"灯个数：");
					dispBuff[0]=(Slave_adrrB_Light_BUFF[1]+0x30);dispBuff[1]='\0';
				ILI9341_DisplayStringEx(LIGHT_X_START+80,LIGHT_Y_START+LINE_SPACE,16,16,dispBuff,0);
			}
			else
			{
					Display_Chinese(LIGHT_X_START,LIGHT_Y_START,"灯状态：关灯");
		     	Display_Chinese(LIGHT_X_START,LIGHT_Y_START+LINE_SPACE,"灯个数：");
			}

			if(Slave_adrrC_TV_BUFF[0]!=0x00)
			{
						  Display_Chinese(TV_X_START,TV_Y_START,"电视状态：开机");
				     // LCD_ClearLine(LINE(12));		
						   Display_Chinese(TV_X_START,TV_Y_START+LINE_SPACE,"电视频道：");	    
					dispBuff[0]=Slave_adrrC_TV_BUFF[1]/10+0x30;
					dispBuff[1]=Slave_adrrC_TV_BUFF[1]%10+0x30;
					dispBuff[2]='\0';
					ILI9341_DisplayStringEx(TV_X_START+100,TV_Y_START+LINE_SPACE,16,16,dispBuff,0);

			}
		else{
					 Display_Chinese(TV_X_START,TV_Y_START,"电视状态：关机");
				 //  LCD_ClearLine(LINE(12));
					 Display_Chinese(TV_X_START,TV_Y_START+LINE_SPACE,"电视频道：");
				}
		
}
/*********************end of file*************************/



