#ifndef __WIND_DIRECTION_H__
#define __WIND_DIRECTION_H__
#include  "stm32f10x.h"
#define  Wind_Direction_Receive_Len 7

void  Send_Wind_Direction(u8 *Data,u8 Len);

u16 Receive_Wind_Direction(void);


#endif
