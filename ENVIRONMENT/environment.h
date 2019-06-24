#ifndef __ENVIRONMENT_H__
#define __ENVIRONMENT_H__
#include "stm32f10x.h"

#define Environment_Receive_Len 31 //接收数据长度
//#define Offset 16    // 行偏移量
extern u8  Enviroment_Data[48];
typedef struct 
{
	u16 Temperature ;
	u16 Humidity;
	u16 Noise;
} Environment;
extern Environment Environment_data;
extern Environment buf_Environment_data;

 
void Environment_Send_Data(u8 *Data, u8 Len);//给传感器发送数据
u8 Environment_Receive_Data(Environment *Data );//接收传感器数据
u8 Process_Environment(Environment *Data);//处理所有环境数据
     
      u8 Save_Humidity(u16 data ,u8 row );
	u8 Save_Temperature(u16 data ,u8 row );
	u8 Save_Noise(u16 data ,u8 row );

void Save_Humidity_To_String(u16 data ,char res[5]);
void Save_Temperature_To_String(u16 data ,char res[5]);
void Save_Noise_To_String(u16 data ,char res[5]);

#endif
