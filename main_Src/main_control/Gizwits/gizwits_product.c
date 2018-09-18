/**
************************************************************
* @file         gizwits_product.c
* @brief        Gizwits control protocol processing, and platform-related hardware initialization 
* @author       Gizwits
* @date         2017-07-19
* @version      V03030000
* @copyright    Gizwits
*
* @note         Gizwits is only for smart hardware
*               Gizwits Smart Cloud for Smart Products
*               Links | Value Added | Open | Neutral | Safety | Own | Free | Ecology
*               www.gizwits.com
*
***********************************************************/
#include <stdio.h>
#include <string.h>
#include "gizwits_product.h"
#include "usart.h"
#include "bsp_led.h"
#include "gizwits_protocol.h"
#include "Slave.h"
#include "bsp_ili9341_lcd.h"
#include "fonts.h"
static uint32_t timerMsCount;
extern u8 WIFI_Send_flag;
extern unsigned char Send2Slave[];
/** Current datapoint */
dataPoint_t currentDataPoint;
extern unsigned char Slave_adrrA_AIR_BUFF[];/*={0x00,0x00,0x00}; */  //分别是 开关机 模式（0-3） 温度（0-13）+17
extern unsigned char Slave_adrrB_Light_BUFF[];/*={0x00,0x00}; */   //开关机  灯等级（0-7）
extern unsigned char Slave_adrrC_TV_BUFF[];/*={0x00,0x00};  */    //开关机   频道 0-99
extern u8 BLE_SEND_DAT_FLAG;
/**@} */
/**@name Gizwits User Interface
* @{
*/

/**
* @brief Event handling interface

* Description:

* 1. Users can customize the changes in WiFi module status

* 2. Users can add data points in the function of event processing logic, such as calling the relevant hardware peripherals operating interface

* @param [in] info: event queue
* @param [in] data: protocol data
* @param [in] len: protocol data length
* @return NULL
* @ref gizwits_protocol.h
*/
int8_t gizwitsEventProcess(eventInfo_t *info, uint8_t *gizdata, uint32_t len)
{
  uint8_t i = 0;
  dataPoint_t *dataPointPtr = (dataPoint_t *)gizdata;
  moduleStatusInfo_t *wifiData = (moduleStatusInfo_t *)gizdata;
  protocolTime_t *ptime = (protocolTime_t *)gizdata;
  
#if MODULE_TYPE
  gprsInfo_t *gprsInfoData = (gprsInfo_t *)gizdata;
#else
  moduleInfo_t *ptModuleInfo = (moduleInfo_t *)gizdata;
#endif

  if((NULL == info) || (NULL == gizdata))
  {
    return -1;
  }

  for(i=0; i<info->num; i++)
  {
    switch(info->event[i])
    {
      case EVENT_AIR_Power:
        currentDataPoint.valueAIR_Power = dataPointPtr->valueAIR_Power;
       // ////GIZWITS_LOG("Evt: EVENT_AIR_Power %d \n", currentDataPoint.valueAIR_Power);
        if(0x01 == currentDataPoint.valueAIR_Power)
        {
          //user handle
						Send2Slave[2]='A';
						Send2Slave[3]='a';
						//Slave_adrrA_AIR_BUFF[0]=POWER_ON;
        }
        else
        {
						Send2Slave[2]='A';
						Send2Slave[3]='b'; 
						//Slave_adrrA_AIR_BUFF[0]=POWER_OFF;
        }
        break;
      case EVENT_Light_Power:
        currentDataPoint.valueLight_Power = dataPointPtr->valueLight_Power;
        //////GIZWITS_LOG("Evt: EVENT_Light_Power %d \n", currentDataPoint.valueLight_Power);
        if(0x01 == currentDataPoint.valueLight_Power)
        {
          	Send2Slave[2]='B';
						Send2Slave[3]='a';
						//Slave_adrrB_Light_BUFF[0]=POWER_ON;
        }
        else
        {
          	Send2Slave[2]='B';
						Send2Slave[3]='b'; 
					//	Slave_adrrB_Light_BUFF[0]=POWER_OFF;
        }
        break;
      case EVENT_TV_Power:
        currentDataPoint.valueTV_Power = dataPointPtr->valueTV_Power;
        //////GIZWITS_LOG("Evt: EVENT_TV_Power %d \n", currentDataPoint.valueTV_Power);
        if(0x01 == currentDataPoint.valueTV_Power)
        {
          //user handle
					//LED_RED;
						Send2Slave[2]='C';
						Send2Slave[3]='a';
				//	Slave_adrrC_TV_BUFF[0]=POWER_ON;
        }
        else
        {
          //user handle 
					//LED_YELLOW;	
          	Send2Slave[2]='C';
						Send2Slave[3]='b';
					//Slave_adrrC_TV_BUFF[0]=POWER_OFF;					
        }
        break;

      case EVENT_AIR_Model:
        currentDataPoint.valueAIR_Model = dataPointPtr->valueAIR_Model;
       // ////GIZWITS_LOG("Evt: EVENT_AIR_Model %d\n", currentDataPoint.valueAIR_Model);
        switch(currentDataPoint.valueAIR_Model)
        {
          case AIR_Model_VALUE0:
            //user handle
          	Send2Slave[2]='A';
						Send2Slave[3]='i';	
					//	Slave_adrrA_AIR_BUFF[1]=0;		
            break;
          case AIR_Model_VALUE1:
          	Send2Slave[2]='A';
						Send2Slave[3]='f';	
					//	Slave_adrrA_AIR_BUFF[1]=1;	
            break;
          case AIR_Model_VALUE2:
          	Send2Slave[2]='A';
						Send2Slave[3]='h';	
					//	Slave_adrrA_AIR_BUFF[1]=2;	
            break;
          case AIR_Model_VALUE3:
          	Send2Slave[2]='A';
						Send2Slave[3]='g';
						//Slave_adrrA_AIR_BUFF[1]=3;		
            break;
          default:
            break;
        }
        break;

      case EVENT_AIR_temper:
        currentDataPoint.valueAIR_temper = dataPointPtr->valueAIR_temper;
        //////GIZWITS_LOG("Evt:EVENT_AIR_temper %d\n",currentDataPoint.valueAIR_temper);
          	Send2Slave[2]='A';
						Send2Slave[3]='m'+dataPointPtr->valueAIR_temper-17 ;	
					//	Slave_adrrA_AIR_BUFF[2]=dataPointPtr->valueAIR_temper;	
        break;
      case EVENT_Light_level:
        currentDataPoint.valueLight_level = dataPointPtr->valueLight_level;
        //////GIZWITS_LOG("Evt:EVENT_Light_level %d\n",currentDataPoint.valueLight_level);
        //user handle
			if(currentDataPoint.valueLight_level==0)
			{
			    	Send2Slave[2]='B';
						Send2Slave[3]='g';
						//Slave_adrrB_Light_BUFF[1]=0;
			}
			else if(currentDataPoint.valueLight_level==1)
			{
			    	Send2Slave[2]='B';
						Send2Slave[3]='f';	
						//Slave_adrrB_Light_BUFF[1]=1;
			}
			else if(currentDataPoint.valueLight_level==2)	
			{
							Send2Slave[2]='B';
							Send2Slave[3]='e';	
						//	Slave_adrrB_Light_BUFF[1]=2;
			}
        break;
      case EVENT_TV_Channel:
        currentDataPoint.valueTV_Channel = dataPointPtr->valueTV_Channel;
       // ////GIZWITS_LOG("Evt:EVENT_TV_Channel %d\n",currentDataPoint.valueTV_Channel);
        //user handle
							Send2Slave[2]='C';
							Send2Slave[3]='m'+ dataPointPtr->valueTV_Channel;	
							//Slave_adrrC_TV_BUFF[0]=dataPointPtr->valueTV_Channel;

        break;


      case WIFI_SOFTAP:
        break;
      case WIFI_AIRLINK:
        break;
      case WIFI_STATION:
        break;
      case WIFI_CON_ROUTER:
 
        break;
      case WIFI_DISCON_ROUTER:
 
        break;
      case WIFI_CON_M2M:
 
        break;
      case WIFI_DISCON_M2M:
        break;
      case WIFI_RSSI:
       // ////GIZWITS_LOG("RSSI %d\n", wifiData->rssi);
        break;
      case TRANSPARENT_DATA:
       // ////GIZWITS_LOG("TRANSPARENT_DATA \n");
        //user handle , Fetch data from [data] , size is [len]
        break;
      case WIFI_NTP:
        //////GIZWITS_LOG("WIFI_NTP : [%d-%d-%d %02d:%02d:%02d][%d] \n",ptime->year,ptime->month,ptime->day,ptime->hour,ptime->minute,ptime->second,ptime->ntp);
        break;
      case MODULE_INFO:
           // ////GIZWITS_LOG("MODULE INFO ...\n");
      #if MODULE_TYPE
           // ////GIZWITS_LOG("GPRS MODULE ...\n");
            //Format By gprsInfo_t
      #else
           // ////GIZWITS_LOG("WIF MODULE ...\n");
            //Format By moduleInfo_t
           // ////GIZWITS_LOG("moduleType : [%d] \n",ptModuleInfo->moduleType);
      #endif
    break;
      default:
        break;
    }
		//userHandle();
		
  }
			BLE_Send();
		//BLE_SEND_DAT_FLAG=1;
  return 0;
}

/**
* User data acquisition

* Here users need to achieve in addition to data points other than the collection of data collection, can be self-defined acquisition frequency and design data filtering algorithm

* @param none
* @return none
*/
void userHandle(void)
{
 /*
    currentDataPoint.valueLight_Num = ;//Add Sensor Data Collection
    currentDataPoint.valueTV_NOW_Channel = ;//Add Sensor Data Collection

    */
	
//	if(WIFI_Send_flag)
//	{
		WIFI_Send_flag=0;
		currentDataPoint.valueAIR_Power=(Slave_adrrA_AIR_BUFF[0]>120)?1:0;
		currentDataPoint.valueLight_Power=(Slave_adrrB_Light_BUFF[0]>120)?1:0;		
		currentDataPoint.valueTV_Power=(Slave_adrrC_TV_BUFF[0]>120)?1:0;	
		
		currentDataPoint.valueAIR_Model = Slave_adrrA_AIR_BUFF[1];
		currentDataPoint.valueAIR_temper =Slave_adrrA_AIR_BUFF[2]+17;
		
		currentDataPoint.valueLight_level = Slave_adrrB_Light_BUFF[1]/3;
		
		currentDataPoint.valueLight_Num = Slave_adrrB_Light_BUFF[1];		
		currentDataPoint.valueTV_Channel = Slave_adrrC_TV_BUFF[1];;

			currentDataPoint.valueTV_NOW_Channel = Slave_adrrC_TV_BUFF[1];
			gizwitsHandle((dataPoint_t*)&currentDataPoint);   
//	}
	
}

/**
* Data point initialization function

* In the function to complete the initial user-related data
* @param none
* @return none
* @note The developer can add a data point state initialization value within this function
*/
void userInit(void)
{
    memset((uint8_t*)&currentDataPoint, 0, sizeof(dataPoint_t));
    
    /** Warning !!! DataPoint Variables Init , Must Within The Data Range **/ 
    
		currentDataPoint.valueAIR_Power=(Slave_adrrA_AIR_BUFF[0]>120)?1:0;
		currentDataPoint.valueLight_Power=(Slave_adrrB_Light_BUFF[0]>120)?1:0;		
		currentDataPoint.valueTV_Power=(Slave_adrrC_TV_BUFF[0]>120)?1:0;	
		
		currentDataPoint.valueAIR_Model = Slave_adrrA_AIR_BUFF[1];
		currentDataPoint.valueAIR_temper =Slave_adrrA_AIR_BUFF[2]+17;//蓝牙传送温度为0-13，显示则为17-30
		
		currentDataPoint.valueLight_level = Slave_adrrB_Light_BUFF[1]/3;
		
		currentDataPoint.valueLight_Num = Slave_adrrB_Light_BUFF[1];		
		currentDataPoint.valueTV_Channel = Slave_adrrC_TV_BUFF[1];
 }


/**
* @brief  gizTimerMs

* millisecond timer maintenance function ,Millisecond increment , Overflow to zero

* @param none
* @return none
*/
void gizTimerMs(void)
{
    timerMsCount++;
}

/**
* @brief gizGetTimerCount

* Read system time, millisecond timer

* @param none
* @return System time millisecond
*/
uint32_t gizGetTimerCount(void)
{
    return timerMsCount;
}

/**
* @brief mcuRestart

* MCU Reset function

* @param none
* @return none
*/
void mcuRestart(void)
{
  __set_FAULTMASK(1);
	 NVIC_SystemReset();
}
/**@} */

/**
* @brief TIMER_IRQ_FUN

* Timer Interrupt handler function

* @param none
* @return none
*/
void TIMER_IRQ_FUN(void)
{
  gizTimerMs();
}

/**
* @brief UART_IRQ_FUN

* UART Serial interrupt function ，For Module communication

* Used to receive serial port protocol data between WiFi module

* @param none
* @return none
*/
void UART_IRQ_FUN(void)
{
  uint8_t value = 0;
  //value = USART_ReceiveData(USART2);//STM32 test demo
  gizPutData(&value, 1);
}


/**
* @brief uartWrite

* Serial write operation, send data to the WiFi module

* @param buf      : Data address
* @param len       : Data length
*
* @return : Not 0,Serial send success;
*           -1，Input Param Illegal
*/
int32_t uartWrite(uint8_t *buf, uint32_t len)
{
    uint32_t i = 0;
    
    if(NULL == buf)
    {
        return -1;
    }
    
    #ifdef PROTOCOL_DEBUG
//    //GIZWITS_LOG("MCU2WiFi[%4d:%4d]: ", gizGetTimerCount(), len);
		#endif
    for(i=0; i<len; i++)
    {
			  USART_SendData(USART1, buf[i]);//STM32 test demo
				while(USART_GetFlagStatus(USART1,USART_FLAG_TC)==RESET);
			  #ifdef PROTOCOL_DEBUG
      //  //GIZWITS_LOG("%02x ", buf[i]);
			  #endif
   // }
   
 

//    for(i=0; i<len; i++)
//    {
//        USART_SendData(DEBUG_USARTx3, buf[i]);//STM32 test demo
//			  while(USART_GetFlagStatus(USART3,USART_FLAG_TC)==RESET);
        //Serial port to achieve the function, the buf[i] sent to the module
        if(i >=2 && buf[i] == 0xFF)
        {
          //Serial port to achieve the function, the 0x55 sent to the module
          USART_SendData(USART1, 0x55);//STM32 test demo
					while(USART_GetFlagStatus(USART1,USART_FLAG_TC)==RESET);
					 #ifdef PROTOCOL_DEBUG
           //GIZWITS_LOG("%02x ", 0x55);
			     #endif
        }
    }

   
    
    return len;
}



