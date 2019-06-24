//#include "stm32f10x.h"
#include "delay.h"
#include "rs485.h"
#include "led_screen.h"
#include "wind.h"
#include "string.h"
#include "environment.h"
#include "particulate_matter.h"
#include "iwdg.h"
#include "timer.h"
#include "step.h"
#include "stdio.h"
#include "led.h"
int main(void)
	

{   
//     p=0;
//     e=0;
//     w=0;
	//环境传感器参数初始化
    Environment_data.Temperature=0;
    Environment_data.Humidity=0;
    Environment_data.Noise=0;
	//风速传感器参数初始化
    wind_data.Wind_Speed=0 ;
    wind_data.Wind_Direction=0;
    wind_data.Wind_Force=0;
	//空气颗粒参数初始化
		
		Pm_data.PM1_0 =0;
		Pm_data.PM2_5=0;
		Pm_data.PM10=0;

		count=0;
		point=0;
		read=0;
		page=0;
			
	
   delay_init();//延时函数初始化
	 LED_Init();//LED指示灯初始化
	 RS458_Init(9600);	//485初始化
   LED_Screen_Init();//显示屏幕初始化
	 IWDG_Init(4,625);    //预分频数为64,重载值为625,溢出时间为1s喂狗在定时器3中断中
   TIM3_Int_Init(4999,7199);
	 TIM2_Int_Init(7199,7199);
	while(1)
	{ 
		if(read)//每1秒读取一次传感器数据
		{ 
		       read=0;      		
	        Receive_Sensor_Data();//接收各传感器数据	      						
		}
		
		
		 switch (page)
		 {  
	         case 0 :
		     Send_Display_Environment_Data();//发送环境数据到屏幕   
	         break;
	         case 1:
		     Send_Display_Wind_Data();//发送风速到屏幕
		      break;
	         case 2:
		     Send_Display_Pm_Data();//发送PM数据到屏幕	  	        
		      break;
		  default:
              page=0;
	            point=0;
	            break;
		 }
		
		

		
	
	}
	
	

	
	

}
