#ifndef __USART3_H__
#define __USART3_H__
#include "sys.h"
#define USART3_MAX_RECV_LEN		40				//�����ջ����ֽ���
#define USART3_MAX_SEND_LEN		20				//����ͻ����ֽ���
#define USART3_RX_EN 			1					//����.
#define USART3_TX_EN            1                   //����.
#define USART3_RX_DN            0                    //������
#define USART3_TX_DN            0                    //������

extern u8  USART3_RX_BUF[USART3_MAX_RECV_LEN]; 		//���ջ���,���USART2_MAX_RECV_LEN�ֽ�
extern u8  USART3_TX_BUF[USART3_MAX_SEND_LEN]; 		//���ͻ���,���USART2_MAX_SEND_LEN�ֽ�
extern u16 USART3_RX_STA;   						//��������״̬

void USART3_Init(u32 bound);
void USART3_SendData(u8 *data,u8 len);

#endif

