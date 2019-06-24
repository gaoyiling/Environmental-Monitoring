#ifndef __CALC_H__
#define __CALC_H__
#include "stm32f10x.h"
//#include "calc.h"
u8 intlen(u16 m);
void itoa(u16 n,char str[5], u8 len);
void intToStr(char* num,u16 number);
#endif
