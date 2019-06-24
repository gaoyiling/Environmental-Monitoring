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
	//����������������ʼ��
    Environment_data.Temperature=0;
    Environment_data.Humidity=0;
    Environment_data.Noise=0;
	//���ٴ�����������ʼ��
    wind_data.Wind_Speed=0 ;
    wind_data.Wind_Direction=0;
    wind_data.Wind_Force=0;
	//��������������ʼ��
		
		Pm_data.PM1_0 =0;
		Pm_data.PM2_5=0;
		Pm_data.PM10=0;

		count=0;
		point=0;
		read=0;
		page=0;
			
	
   delay_init();//��ʱ������ʼ��
	 LED_Init();//LEDָʾ�Ƴ�ʼ��
	 RS458_Init(9600);	//485��ʼ��
   LED_Screen_Init();//��ʾ��Ļ��ʼ��
	 IWDG_Init(4,625);    //Ԥ��Ƶ��Ϊ64,����ֵΪ625,���ʱ��Ϊ1sι���ڶ�ʱ��3�ж���
   TIM3_Int_Init(4999,7199);
	 TIM2_Int_Init(7199,7199);
	while(1)
	{ 
		if(read)//ÿ1���ȡһ�δ���������
		{ 
		       read=0;      		
	        Receive_Sensor_Data();//���ո�����������	      						
		}
		
		
		 switch (page)
		 {  
	         case 0 :
		     Send_Display_Environment_Data();//���ͻ������ݵ���Ļ   
	         break;
	         case 1:
		     Send_Display_Wind_Data();//���ͷ��ٵ���Ļ
		      break;
	         case 2:
		     Send_Display_Pm_Data();//����PM���ݵ���Ļ	  	        
		      break;
		  default:
              page=0;
	            point=0;
	            break;
		 }
		
		

		
	
	}
	
	

	
	

}
