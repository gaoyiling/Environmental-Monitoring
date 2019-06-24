#include "delay.h"
#include "rs485.h"
#include "led_screen.h"
#include "wind.h"
#include "string.h"
#include "environment.h"
#include "particulate_matter.h"
#include "step.h"
#include "string.h"
#include "stdio.h"
#include "calc.h"
#include "string.h"

  Wind           wind_data={0,0,0};
  Environment    Environment_data={0,0,0};
  pm             Pm_data={0,0,0};
  
  Environment     buf_Environment_data={0,0,0};
  pm              buf_Pm_data={0,0,0};
  Wind            buf_Wind_data={0,0,0};
   
   u8 p;
   u8 w;
   u8 e;
u8 Receive_Sensor_Data()
{
      PM_Receive_Data( &Pm_data);//获取空气颗粒传感器数据
	     Wind_Get_Data(&wind_data );//获取风的数据
      Environment_Receive_Data(&Environment_data );//获取环境数据
      return 0;	
}

/*把结构体数据转换为字符串数据并存到数组中*/
//  Res_Data 数据显示缓存
//  Data 要处理的数据
//  Row			行
//  List        列

/*u8 Save_Int_To_Char_To_Arry(u16 Data, u8*Res_Data ,u8 row,u8 List )
{ 
	char res[5]={' ',' ',' ',' ',' '};
	     u8 length,i;
	     u8 minus = 0;
	     u8 shift =row*Offset ;
	
	length = intlen(environment_Data);
        switch(length)
        {
            case 1:
                itoa(environment_Data%10, res+2, 1);
                break;
            case 2:
                itoa(environment_Data%10, res+3, 1);
             environment_Data/=10;
                itoa(environment_Data%10, res+2, 1);
                break;
            case 3:
                itoa(environment_Data%10, res+3, 1);
               environment_Data/=10;
                itoa(environment_Data%10, res+2, 1);
               environment_Data/=10;
                itoa(environment_Data%10, res+1, 1);
                break;
            default:
                break;
        }
        for(i = 0;i < 5; i++)
        {
          Enviroment_Data[7+shift+i] = res[i];
        }
        
		return 0;
	
}


*/

void Send_Display_Pm_Data()
{  
	if(Pm_data.PM10 !=buf_Pm_data.PM10||Pm_data.PM1_0!=buf_Pm_data.PM1_0||Pm_data.PM2_5!=buf_Pm_data.PM2_5||!p)
	{
	       buf_Pm_data.PM10=Pm_data.PM10;
		     buf_Pm_data.PM1_0=Pm_data.PM1_0;
	       buf_Pm_data.PM2_5=Pm_data.PM2_5;
		
	       Process_Pm( &Pm_data);
	       Link_LED_Screen_Data(PM_Data);
	       LED_Screen_Send_Data(data, 102);
		 p=1;
		 w=0;
	}
}





void Send_Display_Environment_Data()
{      
	
	
	
	if(Environment_data.Temperature!=buf_Environment_data.Temperature||Environment_data.Humidity!=buf_Environment_data.Humidity||Environment_data.Noise!=buf_Environment_data.Noise||!e)
			
	     {
	        buf_Environment_data.Temperature= Environment_data.Temperature;
	        buf_Environment_data.Humidity=Environment_data.Humidity;
	        buf_Environment_data.Noise  =   Environment_data.Noise;
		     
		  Process_Environment(&Environment_data);
		  Link_LED_Screen_Data(Enviroment_Data);
		  LED_Screen_Send_Data(data,94);
		  e=1;
		  p=0;
	     }
	       

}
void Send_Display_Wind_Data()

{
  if( wind_data.Wind_Direction!=buf_Wind_data.Wind_Direction||wind_data.Wind_Force!=buf_Wind_data.Wind_Force||wind_data.Wind_Speed!=buf_Wind_data.Wind_Speed||!w)
	 {
	    buf_Wind_data.Wind_Direction=wind_data.Wind_Direction ;
          buf_Wind_data.Wind_Force= wind_data.Wind_Force;
	    buf_Wind_data.Wind_Speed =wind_data.Wind_Speed ;
		 
	    Process_Wind(&wind_data);
          Link_LED_Screen_Data(Wind_Data );
	    LED_Screen_Send_Data(data, 102);
	    w=1;
	    e=0;
     	 }
	  
	
}














