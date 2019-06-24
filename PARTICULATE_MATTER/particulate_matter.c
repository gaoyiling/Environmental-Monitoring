#include "string.h"
#include "particulate_matter.h"
#include "stdio.h"
#include "stm32f10x.h"
#include "rs485.h"
#include "calc.h"
#include "delay.h"
#include "step.h"
u8 PM_Data[48]=
{
 'P','M','1','.','0',':',' ',' ','-',' ',' ','u','g','/','m','3',//16
 'P','M','2','.','5',':',' ',' ','-',' ',' ','u','g','/','m','3',
 'P','M',' ','1','0',':',' ',' ','-',' ',' ','u','g','/','m','3',
};

//pm Pm_data={0,0,0};




//给pm控制器发送命令数据
void PM_Send_Data(u8 *Data, u8 Len)
{

RS485_SendData(Data,  Len);

}

//接收pm数据
u8 PM_Receive_Data( pm *Data)
{
	u8 res[PM_Receive_Len];
	u16 crc=0;
	u8 cmd[8] = {Pm_Addr, 0x03, 0x00, 0x00, 0x00, 0x06, 0xC5, 0x9D};
	PM_Send_Data(cmd, 8);
	delay_ms(20);
	
    if(!RS485_ReceiveData( res, PM_Receive_Len))
	{
	crc = CRC16(res ,PM_Receive_Len-2);
	     if ((crc&0xff) == res[PM_Receive_Len-2] && ((crc>>8)&0xff) == res[PM_Receive_Len-1])
	     {
								Pm_data.PM1_0 =0;
								Pm_data.PM2_5=0;
								Pm_data.PM10=0;
				 
	              Data->PM1_0|= res[3];
						    Data->PM1_0= Data->PM1_0 << 8;
						    Data->PM1_0|= res[4];
			 
						    Data->PM2_5 |=res[5];
						    Data->PM2_5 = Data->PM2_5<< 8;
						    Data->PM2_5 |= res[6];
			 
						    Data->PM1_0|= res [7];
						    Data->PM10 = Data->PM10 << 8;
						    Data->PM10|= res[8];
								return 0;
	     }
	     else
         {      *Data=buf_Pm_data;
	       // printf("CRC校验失败\r\n");
		    return 1;
        }
	
   }
	
   else
   {      *Data=buf_Pm_data;
	    //printf( "数据接收失败\r\n");
	  return 1;
			
   }
}

//存储pm数据
void Save_Pm1_0_To_String(u16 data ,char res[5])
{
u8 length;
	length = intlen(data);
        switch(length)
        {
            case 1:
                itoa(data%10, res+2, 1);
                break;
            case 2:
                itoa(data%10, res+3, 1);
             data/=10;
                itoa(data%10, res+2, 1);
                break;
            case 3:
                itoa(data%10, res+3, 1);
               data/=10;
                itoa(data%10, res+2, 1);
               data/=10;
                itoa(data%10, res+1, 1);
                break;
            default:
                break;
        }
}

void Save_Pm2_5_To_String(u16 data ,char res[5])
{
 u8 length;
	length = intlen(data);
        switch(length)
        {
            case 1:
                itoa(data%10, res+2, 1);
                break;
            case 2:
                itoa(data%10, res+3, 1);
             data/=10;
                itoa(data%10, res+2, 1);
                break;
            case 3:
                itoa(data%10, res+3, 1);
               data/=10;
                itoa(data%10, res+2, 1);
               data/=10;
                itoa(data%10, res+1, 1);
                break;
            default:
                break;
        }
}

void Save_Pm10_To_String(u16 data ,char res[5])
{
    u8 length;
	length = intlen(data);
        switch(length)
        {
            case 1:
                itoa(data%10, res+2, 1);
                break;
            case 2:
                itoa(data%10, res+3, 1);
             data/=10;
                itoa(data%10, res+2, 1);
                break;
            case 3:
                itoa(data%10, res+3, 1);
               data/=10;
                itoa(data%10, res+2, 1);
               data/=10;
                itoa(data%10, res+1, 1);
                break;
            default:
                break;
        }
  }

  u8 Save_Pm1_0(u16 data,u8 row )

{ 
	char res[5]={' ',' ',' ',' ',' '};
	     u8 i;
	     u8 shift =row*Offset ;
	Save_Pm1_0_To_String( data , res);
        for(i = 0;i < 5; i++)
        {
           PM_Data[5+shift+i] = res[i];
        }
        
		return 0;
	
}
u8 Save_Pm2_5(u16 data,u8 row )

{ 
	char res[5]={' ',' ',' ',' ',' '};
	     u8 i;
	     u8 shift =row*Offset ;
	
	    Save_Pm2_5_To_String( data , res);
	
        for(i = 0;i < 5; i++)
        {
           PM_Data[5+shift+i] = res[i];
        }
        
		return 0;
	
}
u8 Save_Pm10(u16 data,u8 row )

{ 
	char res[5]={' ',' ',' ',' ',' '};
	     u8 i;
	     u8 shift =row*Offset ;
	Save_Pm10_To_String( data , res);
        for(i = 0;i < 5; i++)
        {
           PM_Data[5+shift+i] = res[i];
        }
        
		return 0;
	
}

//	处理所有pm数据
u8 Process_Pm( pm *Data)
{
	
Save_Pm1_0(Data->PM1_0,0 );//处理第一行数据
Save_Pm2_5(Data->PM2_5,1 );//处理第二行数据
Save_Pm10(Data->PM10,2 );//处理第三行数据	
	return 0;

}

















