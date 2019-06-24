#include "string.h"
#include "stm32f10x.h"
#include "usart3.h"
#include "rs485.h"
#include "delay.h"
void RS458_Init(u32 bound)
{
	USART3_Init( bound);//��ʼ������3
	
}
u8 RS485_ReceiveData(u8 *Data,u8 Len)//200MSδ���ܵ����ݷ���0
{  

	u8 time=50;
	memset(Data, 0, Len);
	while(time)
	{
	  if ((USART3_RX_STA & 0xff)== (u16)Len)//�жϽ�����ȷ
	    { 
		u8 i;
	       for( i=0;i<Len;i++)
	       Data[i]=USART3_RX_BUF[i];
		 USART3_RX_STA=0;
	    return 0;
	    } 
  		
	 	else
		 {
			 delay_ms(4);
			 time--;
		 }
	 }
		
		 return 0;
	 	  
}


void RS485_SendData(u8 *Data,u8 Len)
{
  RS485EN = 1;
  delay_us(150);
  USART3_SendData( Data,Len);
  RS485EN =0;
  USART3_RX_STA=0;
	
}

void RS485_SendData16(u16 Data)
 { u8 res;
  RS485EN = 1;
  delay_ms (40);
  res=Data&0xff;
  USART3_SendData(&res,1);
	 Data=Data>>8;
	 res=Data;
	 USART3_SendData(&res,1);
	 
  USART3_RX_STA=0;
  RS485EN =0;
	
}

/*******************************************************************************
*   ��������: CRC16
*   ��������: �������CRC16.
*   �������: dataIn -- ��У������
*             length -- ���ݳ���
*   �� �� ֵ: У��ֵ
*******************************************************************************/
u16 CRC16(u8 *ptr, int len)
{
    int j, k;
    unsigned int c, crc = 0xFFFF;
    //����CRC
    for (k = 0; k < len; k++)
    {
        c = *ptr & 0x00FF;
        crc ^= c;
        ptr++;
        for(j = 0; j < 8; j++)
        {
            if (crc & 0x0001)
            {
                crc >>= 1;
                crc ^= 0xA001;
            }
            else crc >>= 1;
        }
    }
    return(crc);
}

