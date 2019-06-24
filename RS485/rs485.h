#ifndef __RS485_H__
#define __RS485_H__

//#include "stm32f10x.h"
#include "usart3.h"
#define RS485EN PBout(1)              //485����ʹ��

void RS458_Init(u32 bound);
u8 RS485_ReceiveData(u8 *Data,u8 Len);//������40���ֽ�
void RS485_SendData(u8 *Data,u8 Len);//�����20���ֽ�
u16 CRC16(u8 *ptr, int len);
void RS485_SendData16(u16 Data);
#endif
