#include "led_screen.h"
#include "rs485.h"
#include "step.h"
 
   /*48*128*/
 u8 data[102] = {
		
        0xAA,0xA5,//起始符                           
       0x56,0x00,//控制符，无CRC校验，数据长度为86位
       LED_Screen_Group,LED_Screen_ID,//地址为第一组第一个
       0x12,0x34,//发送端地址
	 0xB0,0xA1,//会话ID
        0x38,0x02,//6*2=12
        0x00,0x00,0x00,0x00,
        0x60,0x00,0x30,0x00,
        0x00,0x00,0x00,0x00,// 3*4=12
        0x40,0x00,//2
        ' ',' ',0xD1,0xEF,0xB3,0xBE,0xBC,0xE0,0xB2,0xE2,0xCF,0xB5,0xCD,0xB3,' ',0x0A,//
	//42
        ' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',
        ' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',
        ' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',//16*4=64
        0x00,0x00,
        0x5A,0x55//2*2=4
	//2+ 12*2+64+4=24+64+4=88+4=94-102=-8
    };
u8 LED_Screen_Init()
{
	//u8 Res[18];
   u8 Init_Cmd[17]={0xAA,0xA5,0x09,0x00,0x01,0x01,0x12,0x34,0xB0,0xA1,0x7A,0x00,0x01,0x00,0x00,0x5A,0x55};
  RS485_SendData(Init_Cmd,17);
 // RS485_ReceiveData(Res,18);
   //if (!Res[12]&Res[13])
	  // return 0;
  // else 
	   return 1;


}
u8 LED_Screen_Receive_Data(u8 *Data,u8 Len)
{
return RS485_ReceiveData(Data,Len );
}


void LED_Screen_Send_Data(u8 *Data, u8 Len)
{
  RS485_SendData(Data,  Len);
}
u8 Link_LED_Screen_Data(u8 *type )
{   u8 i;
	for(i=0;i<48;i++)
	{
	  data[42+i]=type[i];
	}
return 0;
}
