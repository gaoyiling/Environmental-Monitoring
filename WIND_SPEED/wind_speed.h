#ifndef __WIND_SPEED_H__
#define __WIND_SPEED_H__
#include  "stm32f10x.h"
#define  Wind_Speed_Receive_Len 9
//u16 Wind_Speed=0;
//u8 Wind_Force;
//u8 Wind_Data[48]=
//{
	
//};
void  Send_Wind_Speed(u8 *Data,u8 Len);
 u16 Receive_Wind_Speed(void);
 u8 Get_Wind_Force(u16 windspeed);
 
 
  
  

#endif
