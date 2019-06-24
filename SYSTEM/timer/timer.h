#ifndef __TIMER_H__
#define __TIMER_H__
#include "stm32f10x.h"
extern u8 count;
extern u8 point;
extern u8 page;
extern u8 read;

void TIM3_Int_Init(u16 arr,u16 psc );
void TIM2_Int_Init(u16 arr,u16 psc);



#endif

