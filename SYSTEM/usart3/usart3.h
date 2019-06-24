#ifndef __USART3_H__
#define __USART3_H__
#include "sys.h"
#define USART3_MAX_RECV_LEN		40				//最大接收缓存字节数
#define USART3_MAX_SEND_LEN		20				//最大发送缓存字节数
#define USART3_RX_EN 			1					//接收.
#define USART3_TX_EN            1                   //发送.
#define USART3_RX_DN            0                    //不接收
#define USART3_TX_DN            0                    //不发送

extern u8  USART3_RX_BUF[USART3_MAX_RECV_LEN]; 		//接收缓冲,最大USART2_MAX_RECV_LEN字节
extern u8  USART3_TX_BUF[USART3_MAX_SEND_LEN]; 		//发送缓冲,最大USART2_MAX_SEND_LEN字节
extern u16 USART3_RX_STA;   						//接收数据状态

void USART3_Init(u32 bound);
void USART3_SendData(u8 *data,u8 len);

#endif

