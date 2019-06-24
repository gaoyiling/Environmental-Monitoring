#include "wind_direction.h"
#include  "stm32f10x.h"
#include "rs485.h"
#include "stdio.h"
#include "delay.h"
#include "step.h"
#include "wind.h"
void  Send_Wind_Direction(u8 *Data,u8 Len)
{
   RS485_SendData(Data,  Len);
}
 u16 Receive_Wind_Direction(void)
{
	u8 res[Wind_Direction_Receive_Len];
	u16 crc=0;
	u16 Wind_Direction=0;
	u8 cmd[] = {Wind_Direction_Addr, 0x04, 0x00, 0x03, 0x00, 0x01, 0xC1, 0xF9};//获取风向
	 Send_Wind_Direction(cmd, 8);
	delay_ms(20);
	if(  !RS485_ReceiveData(res ,Wind_Direction_Receive_Len))
	{
	    if(res[0]==Wind_Direction_Addr)
	    {
		crc = CRC16(res,Wind_Direction_Receive_Len-2 );
		
		              if ((crc&0xff) == res [5] && ((crc>>8)&0xff) == res[6])
						   {    
							   Wind_Direction=0; 
							   
							    Wind_Direction |=res[3];
								Wind_Direction = Wind_Direction << 8;
								Wind_Direction |= res[4];
								return Wind_Direction ; 
						  }
						else
						  {
								//printf("CRC校验失败\r\n");
							    return wind_data.Wind_Direction;
		 
						  }
	
	   }
	    else
		    {
					
	         return wind_data.Wind_Direction;
		    }
	  
   }
	else
	//  printf( "数据接收失败\r\n");
	  return wind_data.Wind_Direction;
			
	
     
}
