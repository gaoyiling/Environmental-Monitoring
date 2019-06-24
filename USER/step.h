#ifndef __STEP_H__
#define __STEP_H__
#include "stm32f10x.h"
       //设备地址
#define LED_Screen_Group    0x01
#define LED_Screen_ID       0x01

#define Environment_Addr    0X01
#define Wind_Direction_Addr 0x02
#define Wind_Speed_Addr     0x03
#define Pm_Addr             0X04


//LED屏幕数据显示偏移量

#define  Environment_List_Offset  7
#define  Environment_Row_Offset  16


#define  Wind_List_Offset 7
#define  Wind_Row_Offset 16

#define  Pm_List_Offset 7
#define  Pm_Row_Offset 16

#define Offset 16    // 行偏移量
//typedef enum {false = 0,true = 1} bool;
//#define false 0;
//#define true  1;
  extern u8 p;
  extern u8 w;
  extern u8 e;





void Process_LED_Screen_Data(u8 *data ,u8 len);
u8 Receive_Sensor_Data(void );
void Send_Display_Pm_Data(void);
void Send_Display_Wind_Data(void);
void Send_Display_Environment_Data(void);



#endif
