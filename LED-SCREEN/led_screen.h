#ifndef __LED_SCREEN_H__
#define __LED_SCREEN_H__
#include "stm32f10x.h"

extern u8 data[102];
u8  LED_Screen_Init(void);
void LED_Screen_Send_Data(u8 *Data, u8 Len);
u8 LED_Screen_Receive_Data(u8 *Data,u8 Len);
u8 Link_LED_Screen_Data(u8 *type );

#endif
