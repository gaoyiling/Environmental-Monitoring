#include "environment.h"
#include "string.h"
#include "stdio.h"
#include "stm32f10x.h"
#include "rs485.h"
#include "calc.h"
#include "step.h"

 u8  Enviroment_Data[48]=
{
        0xCE,0xC2,' ',0xB6,0xC8,':',' ',' ','-',' ',' ',' ',0xA1,0xE6,' ',0x0A,  //温度
        0xCA,0xAA,' ',0xB6,0xC8,':',' ',' ','-',' ',' ',' ','%','R','H',0x0A,    //湿度
        0xD4,0xEB,' ',0xD2,0xF4,':',' ',' ','-',' ',' ',' ','d','b',' ',0x0A,    //噪音
};
//Environment Environment_data={0,0,0};





void Environment_Send_Data(u8 *Data, u8 Len)
{
RS485_SendData(Data,  Len);
}


u8 Environment_Receive_Data(Environment *Data )
{
	u8 res[Environment_Receive_Len];//接收缓存数据
	u16 crc=0;
	u8 cmd[] = {Environment_Addr, 0x03, 0x00, 0x00, 0x00, 0x0D, 0x84, 0x0F};//请求传感器发送数据
	memset(res,0,Environment_Receive_Len);//初始化接收缓存
	USART3_RX_STA = 0;//清除接收个数
	Environment_Send_Data(cmd, 8);//发送请求
	
	
   if(!RS485_ReceiveData( res, Environment_Receive_Len))
	{
		if(res[0]==Environment_Addr)
		{
		      crc = CRC16(res ,29);//对接收到的数据进行CRC校验
		
	           if ((crc&0xff) == res[Environment_Receive_Len-2] && ((crc>>8)&0xff) == res[Environment_Receive_Len-1])
	               {
			                        //数据初始化
			                      Data->Humidity=0;
			                      Data->Temperature=0;
			                      Data->Noise=0;
			 
			                       //获取湿度
	                                  Data->Humidity|= res[3];
			 			    Data->Humidity= Data->Humidity << 8;
		                    	    Data->Humidity|= res[4];
			                       //RS485_SendData16(Data->Humidity);
			                  
			 	                 //获取温度
			                    								
						     Data->Temperature |=res[5];
						     Data->Temperature = Data->Temperature<< 8;
						     Data->Temperature |= res[6];
		     
			                     //获取噪音
						     Data->Noise|= res [27];
						     Data->Noise = Data->Noise << 8;
						     Data->Noise|= res[28];
			                    //RS485_SendData(res, Environment_Receive_Len);	/////////////
                                     //Environment_Send_Data(res, Environment_Receive_Len);
			   
					 			return 0;
	               }
	         else
                    {
				 *Data=buf_Environment_data ; 
	             //printf("CRC校验失败\r\n");
			     return 1;
		        } 
		 
	  }
		
	  
	}
	else
	 {
		*Data=buf_Environment_data ;
           		 
		//printf( "数据接收失败\r\n");
	 }
return 1;
	 
	
}
//把温度整型数据换成字符串
void Save_Temperature_To_String(u16 data ,char res[5])
{
	 u8 length;
	 _Bool  minus=0 ;
	if(data & 0x8000)
	         {
	          data = ~(data-1);
		    minus =1;
		  
	         }
   	   
	if(data==0)
	        {
		    res[2]='0';
	        }
	    	       	 
          else
	       {	
	           length = intlen(data);
                 switch(length)
                 {
                  case 1:
                          itoa(data%10, res+4, 1);
			        res[3]='.';
			        res[2]='0';
			       
			        res[1]=minus?'-':' ';
                          break;
                   case 2:
                         itoa(data%10, res+4, 1);
			       data/=10;
                          res[3] = '.';
                         itoa(data%10, res+2, 1);
                          res[1] = minus?'-':' ';
                          break;
                    
                    case 3:
                        itoa(data%10,res+4, 1);
                        data/=10;
                        res[3] = '.';
                        itoa(data%10, res+2, 1);
                        data/=10;
                        itoa(data%10, res+1, 1);
                        res[0] = minus?'-':' ';
                        break;
                
               default:
                      break;
                }
	      }   	  		
}

 
//把湿度整型数据换成字符串
void Save_Humidity_To_String(u16 data ,char res[5])
{
	u8 length;
	if(data==0)
	 res[2]='0';
	else
	{	
	     length = intlen(data);
        switch(length)
        {
            case 1:
                    itoa(data%10, res+4, 1);
			  res[3]='.';
			  res[2]='0';  
                    break;
            case 2:
                    itoa(data%10, res+4, 1);
			  data/=10;
                    res[3] = '.';
                    itoa(data%10, res+2, 1);
                   
                    break;
                    
            case 3:
                    itoa(data%10,res+4, 1);
                    data/=10;
                    res[3] = '.';
                    itoa(data%10, res+2, 1);
                    data/=10;
                    itoa(data%10, res+1, 1);
                    break;
                
            default:
                    break;
        }
	}
}

//把噪声整型数据换成字符串
void Save_Noise_To_String(u16 data ,char res[5])
{
	u8 length;
	if(data==0)
	   res[2]='0';
	else
	{	
	     length = intlen(data);
        switch(length)
        {
            case 1:
                    itoa(data%10, res+4, 1);
			        res[3]='.';
			        res[2]='0';
			       
                    break;
            case 2:
                    itoa(data%10, res+4, 1);
			        data/=10;
                    res[3] = '.';
                    itoa(data%10, res+2, 1);
                   
                    break;
                    
            case 3:
                    itoa(data%10,res+4, 1);
                    data/=10;
                    res[3] = '.';
                    itoa(data%10, res+2, 1);
                    data/=10;
                    itoa(data%10, res+1, 1);
                    break;
                
            default:
                    break;
        }
	}
}




u8 Save_Temperature(u16 data ,u8 row )
{ 
	char res[5]={' ',' ',' ',' '};
	     u8 shift =row*Offset ;
	    u8 i;
	Save_Temperature_To_String( data , res);

        for(i = 0;i < 5; i++)
        {
          Enviroment_Data[6+shift+i] = res[i];
        }
        
		return 0;
	
}


u8 Save_Humidity(u16 data ,u8 row )
{ 
	char res[5]={' ',' ',' ',' '};
	       u8 i;
		 u8 shift =row*Offset ;
	 Save_Humidity_To_String(data , res);

        for(i = 0;i < 5; i++)
        {
          Enviroment_Data[6+shift+i] = res[i];
        }
        
		return 0;
	
}


u8 Save_Noise(u16 data ,u8 row )
{ 
	char res[5]={' ',' ',' ',' '};
	       u8 i;
		 u8 shift =row*Offset ;
	 Save_Noise_To_String( data ,res);

        for(i = 0;i < 5; i++)
        {
          Enviroment_Data[6+shift+i] = res[i];
        }
        
		return 0;
	
}

//把环境数据存储到显示数组
u8 Process_Environment(Environment *Data)
{      Save_Temperature(Data->Temperature,0 );
       Save_Humidity(Data->Humidity,1 );
	 Save_Noise(Data->Noise,2 );
	  return 0;
}



















