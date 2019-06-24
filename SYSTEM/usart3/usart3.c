#include "delay.h"
#include "usart3.h"
#include "stdarg.h"
#include "stdio.h"
#include "string.h"

        
//串口接收缓存区
 __align(8) u8 USART3_RX_BUF[USART3_MAX_RECV_LEN];//接收缓冲,最大USART2_MAX_RECV_LEN个字节.


#ifdef USART3_TX_EN //如果使能了发送        
//串口发送缓存区
u8 USART3_TX_BUF[USART3_MAX_SEND_LEN];//发送缓冲,最大USART2_MAX_SEND_LEN字节.
#endif
void USART3_Init(u32 bound)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);// GPIOB时钟
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE);

    USART_DeInit(USART3);                                //复位串口3
    //USART3_TX   PB10
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;            //PB10
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;         //复用推挽输出
    GPIO_Init(GPIOB, &GPIO_InitStructure);               //初始化PB10
    //USART3_RX      PB11
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;            //PB11
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//浮空输入
    GPIO_Init(GPIOB, &GPIO_InitStructure);               //初始化PB11

    //RS485EN  PB0
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;            //PB1
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;    //推挽输出
    GPIO_Init(GPIOB, &GPIO_InitStructure);               //初始化PB1

    
    USART_InitStructure.USART_BaudRate = bound;  //一般设置为9600;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;                    //字长为8位数据格式
    USART_InitStructure.USART_StopBits = USART_StopBits_1;                         //一个停止位
    USART_InitStructure.USART_Parity = USART_Parity_No;                            //无奇偶校验位
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;                   //收发模式



    
    NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2 ;//抢占优先级2
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;      //子优先级3
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;         //IRQ通道使能
    NVIC_Init(&NVIC_InitStructure); //根据指定的参数初始化VIC寄存器
		
		USART_Init(USART3, &USART_InitStructure); //初始化串口3
		USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);//开启中断   
		USART_Cmd(USART3, ENABLE);                //使能串口 
}
/////////////////////////////////////////////////////////////////////////
                      //发送
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
                      //接收
u16 USART3_RX_STA=0;
void USART3_IRQHandler(void)
{
    u8 res;     
    if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)//接收到数据
    {    
        res =USART_ReceiveData(USART3);              
        if(USART3_RX_STA<USART3_MAX_RECV_LEN)       //还可以接收数据
        {
            USART3_RX_BUF[USART3_RX_STA++]=res;     //记录接收到的值
        }
				else 
        {
            USART3_RX_STA|=1<<15;                   //强制标记接收完成
        } 
    }
}
