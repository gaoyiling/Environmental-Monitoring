#include "wind_speed.h"
#include  "stm32f10x.h"
#include "rs485.h"
#include "stdio.h"
#include "delay.h"
#include "step.h"
#include "wind.h"
#include "step.h"
void  Send_Wind_Speed(u8 *Data,u8 Len)
{
  RS485_SendData(Data,  Len);

}
 u16 Receive_Wind_Speed(void)
{   
	
	u8 res[Wind_Speed_Receive_Len] ;
	u16 crc;
	u16 speed;
	u8 cmd[] = {Wind_Speed_Addr, 0x03, 0x06, 0x00, 0x00, 0x02, 0xC5, 0x61};//获取风速
	Send_Wind_Speed( cmd , 8);
	delay_ms(5);
	
	   if (!RS485_ReceiveData( res, Wind_Speed_Receive_Len))
    {        if(res[0]==Wind_Speed_Addr)
	    {
	      crc = CRC16(res,Wind_Speed_Receive_Len-2 );//CRC校验
		    
	       if ((crc&0xff) == res[7] && ((crc>>8)&0xff) == res[8])
	         {
		       speed=0;
	             speed|=res[5];
		       speed=speed<<8;
		       speed|=res[6];
		       return speed;
		   }
	       else 
	         {
	                 // printf("CRC校验失败\r\n");
			     //RS485_SendData(res,9);
				return wind_data.Wind_Speed ;
					
					
	         }
          }
	    else
	    {
	    return wind_data.Wind_Speed;
	    }
    }
    else 
    {
	 // printf( "数据接收失败\r\n");
	  return wind_data.Wind_Speed;
    }
	
}
    




 u8 Get_Wind_Force(u16 windspeed)
{
       if ( windspeed <= 2)
        {
            return 0;
        }
        else if (2 < windspeed && windspeed <= 16)
        {
            return 1;
        }
        else if (16 < windspeed && windspeed <= 34)
        {
            return 2 ;
        }
        else if (34 < windspeed &&windspeed<= 55)
        {
            return 3 ;
        }
        else if (55 < windspeed && windspeed <= 80)
        {
            return 4 ;
        }
        else if (80 < windspeed && windspeed <= 108)
        {
            return 5 ;
        }
        else if (108 < windspeed && windspeed <= 139)
        {
            return 6 ;
        }
        else if (139 < windspeed && windspeed <= 172)
        {
            return 7 ;
        }
        else if (172 < windspeed && windspeed <= 208)
        {
            return 8 ;
        }
        else if (208 < windspeed && windspeed <= 245)
        {
            return 9 ;
        }
        else if (245 < windspeed &&windspeed <= 285)
        {
           return 10 ;
        }
        else if (285 < windspeed && windspeed <= 32.6)
        {
          return 11 ;
        }
        else if (326 < windspeed && windspeed <= 370)
        {
           return 12 ;
        }
        else if (370 < windspeed && windspeed <= 414)
        {
            return 13 ;
        }
        else if (414 < windspeed && windspeed <= 461)
        {
            return 14 ;
        }
        else if (461 < windspeed && windspeed <= 509)
        {
           return 15 ;
        }
        else if (509 < windspeed && windspeed <= 560)
        {
           return 16 ;
        }
        else if (560 < windspeed && windspeed <= 612)
        {
          return 17 ;
        }
	   else 
	  {
		  //printf( "数据处理失败\r\n");
		  return '-'	;
        }  
	
	
}


















