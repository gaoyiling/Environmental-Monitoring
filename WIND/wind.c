#include "wind_direction.h"
#include  "stm32f10x.h"
#include "wind_speed.h"
#include "stdio.h"
#include "wind.h"
#include "calc.h"
#include "step.h"
 u8 Wind_Data[48]=
{
        0xB7,0xE7,' ',0xCB,0xD9,':',' ',' ','-',' ',' ','m','/','s',' ',0x0A,    //风速
        0xB7,0xE7,' ',0xCF,0xF2,':',' ',' ','-',' ',' ',' ',' ',' ',' ',0x0A,    //风向
        0xB7,0xE7,' ',0xC1,0xA6,':',' ',' ','-',' ',' ',0xBC,0xB6,' ',' ',0x0A,  //风力
        
};


//Wind wind_data={0,0,0};



//获取风数据
u8 Wind_Get_Data(Wind *Data )
{
    
      Data->Wind_Speed= Receive_Wind_Speed();
	Data->Wind_Force= (u16)Get_Wind_Force(Data->Wind_Speed);
	Data->Wind_Direction=Receive_Wind_Direction();
	return 0;

}



void Save_Wind_Speed_To_String(u16 data ,char res[5])
{
	u8 length;
	length = intlen(data);
        switch(length)
        {
            case 1:
                itoa(data%10, res+4, 1);
		    res[3] = '.';
                res[2] = '0';
                break;
            case 2:
                
		    itoa(data%10, res+4, 1);
                data/=10;
                res[3] = '.';
                itoa(data%10, res+2, 1);
			
             //data/=10;
                //itoa(data%10, res+2, 1);
                break;
            case 3:
                itoa(data%10, res+4, 1);
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
//保存风速显示数据

u8 Save_Wind_Speed(u16 data,u8 row )
{

char res[5]={' ',' ',' ',' ',' '};
	     u8 i;
	     u8 shift =row*Offset ;
     
	Save_Wind_Speed_To_String( data , res);
        for(i = 0;i < 5; i++)
        {
          Wind_Data[5+shift+i] = res[i];
        }
        
		return 0;

}

void Save_Wind_Force_To_String(u16 data ,char res[5])
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
}
//保存风力显示数据
u8  Save_Wind_Force( u16 data,u8 row )
{

char res[5]={' ',' ',' ',' ',' '};
	     u8 i;
	     u8 shift =row*Offset ;
 
  Save_Wind_Force_To_String( data , res);
   for(i = 0;i < 5; i++)
     {
      Wind_Data[6+shift+i] = res[i];
     }
        
		return 0;

}


void Save_Wind_Direction_To_String(u16 data ,char res[8])
{
switch(data)
        {
            case 0x000F:
                // printf("风向：正北\r\n");
                res[3] = 0xB1;//北
                res[2] = 0xB1;
                res[1] = 0xFD;//正
                res[0] = 0xD5;
                break;
            case 0x0000:
                // printf("风向：东北偏北\r\n");
                res[7] = 0xB1;//北
                res[6] = 0xB1;
                res[5] = 0xAB;//偏
                res[4] = 0xC6;
                res[3] = 0xB1;//北
                res[2] = 0xB1;
                res[1] = 0xAB;//东
                res[0] = 0xB6;
                break;
            case 0x0001:
                // printf("风向：东北\r\n");
                res[3] = 0xB1;//北
                res[2] = 0xB1;
                res[1] = 0xAB;//东
                res[0] = 0xB6;
                break;
            case 0x0002:
                // printf("风向:东北偏东\r\n");
                res[7] = 0xAB;//东
                res[6] = 0xB6;
                res[5] = 0xAB;//偏
                res[4] = 0xC6;
                res[3] = 0xB1;//北
                res[2] = 0xB1;
                res[1] = 0xAB;//东
                res[0] = 0xB6;
                break;
            case 0x0003:
                // printf("风向：正东\r\n");
                res[3] = 0xAB;//东
                res[2] = 0xB6;
                res[1] = 0xFD;//正
                res[0] = 0xD5;
                break;
            case 0x0004:
                // printf("风向：东南偏东\r\n");
                res[7] = 0xAB;//东
                res[6] = 0xB6;
                res[5] = 0xAB;//偏
                res[4] = 0xC6;
                res[3] = 0xCF;//南
                res[2] = 0xC4;
                res[1] = 0xAB;//东
                res[0] = 0xB6;
                break;
            case 0x0005:
                // printf("风向：东南\r\n");
                res[3] = 0xCF;//南
                res[2] = 0xC4;
                res[1] = 0xAB;//东
                res[0] = 0xB6;
                break;
            case 0x0006:
                // printf("风向：东南偏南\r\n");
                res[7] = 0xCF;//南
                res[6] = 0xC4;
                res[5] = 0xAB;//偏
                res[4] = 0xC6;
                res[3] = 0xCF;//南
                res[2] = 0xC4;
                res[1] = 0xAB;//东
                res[0] = 0xB6;
                break;
            case 0x0007:
                // printf("风向：正南\r\n");
                res[3] = 0xCF;//南
                res[2] = 0xC4;
                res[1] = 0xFD;//正
                res[0] = 0xD5;
                break;
            case 0x0008:
                // printf("风向：西南偏南\r\n");
                res[7] = 0xCF;//南
                res[6] = 0xC4;
                res[5] = 0xAB;//偏
                res[4] = 0xC6;
                res[3] = 0xCF;//南
                res[2] = 0xC4;
                res[1] = 0xF7;//西
                res[0] = 0xCE;
                break;
            case 0x0009:
                // printf("风向：西南\r\n");
                res[3] = 0xCF;//南
                res[2] = 0xC4;
                res[1] = 0xF7;//西
                res[0] = 0xCE;
                break;
            case 0x000A:
                // printf("风向：西南偏西\r\n");
                res[7] = 0xF7;//西
                res[6] = 0xCE;
                res[5] = 0xAB;//偏
                res[4] = 0xC6;
                res[3] = 0xCF;//南
                res[2] = 0xC4;
                res[1] = 0xF7;//西
                res[0] = 0xCE;
                break;
            case 0x000B:
                // printf("风向：正西\r\n");
                res[3] = 0xF7;//西
                res[2] = 0xCE;
                res[1] = 0xFD;//正
                res[0] = 0xD5;
                break;
            case 0x000C:
                // printf("风向:西北偏西\r\n");
                res[7] = 0xF7;//西
                res[6] = 0xCE;
                res[5] = 0xAB;//偏
                res[4] = 0xC6;
                res[3] = 0xB1;//北
                res[2] = 0xB1;
                res[1] = 0xF7;//西
                res[0] = 0xCE;
                break;
            case 0x000D:
                // printf("风向：西北\r\n");
                res[3] = 0xB1;//北
                res[2] = 0xB1;
                res[1] = 0xF7;//西
                res[0] = 0xCE;
                break;
            case 0x000E:
                // printf("风向：西北偏北\r\n");
                res[7] = 0xB1;//北
                res[6] = 0xB1;
                res[5] = 0xAB;//偏
                res[4] = 0xC6;
                res[3] = 0xB1;//北
                res[2] = 0xB1;
                res[1] = 0xF7;//西
                res[0] = 0xCE;
                break;
            default:
                break;
        }
}
//保存风向显示数据
u8  Save_Wind_Direction( u16 data,u8 row )
{
  char res[8]={' ',' ',' ',' ',' ',' ',' ',' '};
	     u8 i;
	     u8 shift =row*Offset ;
 
       Save_Wind_Direction_To_String( data , res);
        

      for(i = 0;i < 8; i++)
        {
          Wind_Data[6+shift+i] = res[i];
        }
        
		return 0;



}

//处理所有风参数显示数据
u8 Process_Wind(Wind *Data)
{
    Save_Wind_Speed( Data->Wind_Speed,0 );
    Save_Wind_Direction( Data->Wind_Direction,1 );
    Save_Wind_Force( Data->Wind_Force,2 );
   
	return 0;
	
}



