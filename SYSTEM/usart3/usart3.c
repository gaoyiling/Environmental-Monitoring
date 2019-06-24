#include "delay.h"
#include "usart3.h"
#include "stdarg.h"
#include "stdio.h"
#include "string.h"

        
//���ڽ��ջ�����
 __align(8) u8 USART3_RX_BUF[USART3_MAX_RECV_LEN];//���ջ���,���USART2_MAX_RECV_LEN���ֽ�.


#ifdef USART3_TX_EN //���ʹ���˷���        
//���ڷ��ͻ�����
u8 USART3_TX_BUF[USART3_MAX_SEND_LEN];//���ͻ���,���USART2_MAX_SEND_LEN�ֽ�.
#endif
void USART3_Init(u32 bound)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);// GPIOBʱ��
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE);

    USART_DeInit(USART3);                                //��λ����3
    //USART3_TX   PB10
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;            //PB10
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;         //�����������
    GPIO_Init(GPIOB, &GPIO_InitStructure);               //��ʼ��PB10
    //USART3_RX      PB11
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;            //PB11
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//��������
    GPIO_Init(GPIOB, &GPIO_InitStructure);               //��ʼ��PB11

    //RS485EN  PB0
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;            //PB1
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;    //�������
    GPIO_Init(GPIOB, &GPIO_InitStructure);               //��ʼ��PB1

    
    USART_InitStructure.USART_BaudRate = bound;  //һ������Ϊ9600;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;                    //�ֳ�Ϊ8λ���ݸ�ʽ
    USART_InitStructure.USART_StopBits = USART_StopBits_1;                         //һ��ֹͣλ
    USART_InitStructure.USART_Parity = USART_Parity_No;                            //����żУ��λ
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;                   //�շ�ģʽ



    
    NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2 ;//��ռ���ȼ�2
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;      //�����ȼ�3
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;         //IRQͨ��ʹ��
    NVIC_Init(&NVIC_InitStructure); //����ָ���Ĳ�����ʼ��VIC�Ĵ���
		
		USART_Init(USART3, &USART_InitStructure); //��ʼ������3
		USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);//�����ж�   
		USART_Cmd(USART3, ENABLE);                //ʹ�ܴ��� 
}
/////////////////////////////////////////////////////////////////////////
                      //����
void USART3_SendData(u8 *data,u8 len)
{
     u8 j;
for (j = 0; j < len; j++)
    {
        USART_SendData(USART3, (u16) data[j]);
        while(!USART_GetFlagStatus(USART3,USART_FLAG_TC));
    }
	 
	
}

////////////////////////////////////////////////////////////////////////
                      //����
u16 USART3_RX_STA=0;
void USART3_IRQHandler(void)
{
    u8 res;     
    if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)//���յ�����
    {    
        res =USART_ReceiveData(USART3);              
        if(USART3_RX_STA<USART3_MAX_RECV_LEN)       //�����Խ�������
        {
            USART3_RX_BUF[USART3_RX_STA++]=res;     //��¼���յ���ֵ
        }
				else 
        {
            USART3_RX_STA|=1<<15;                   //ǿ�Ʊ�ǽ������
        } 
    }
}
