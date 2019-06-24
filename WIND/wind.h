#ifndef __WIND_H__
#define __WIND_H__
#include "stm32f10x.h"

//#define Offset 16  

 extern  u8 Wind_Data[48];
 typedef struct 
{
	u16 Wind_Speed ;
	u16 Wind_Direction;
	u16 Wind_Force;
}Wind;
extern  Wind wind_data;
extern  Wind buf_Wind_data;

 //pm Pm_Data={0,0,0};
//u8 cmd[8] = {0x04, 0x03, 0x00, 0x00, 0x00, 0x06, 0xC5, 0x9D};

u8 Wind_Get_Data(Wind *Data );//接收数据
u8 Process_Wind(Wind *Data);//处理所有数据
//u8 Save_Wind(u16 wind_Data,u8 row );//以字符串方式存储数据单个换将数据
u8  Save_Wind_Speed( u16 data,u8 row );
u8	Save_Wind_Force( u16 data,u8 row );
u8  Save_Wind_Direction( u16 data,u8 row );
void Save_Wind_Speed_To_String(u16 data ,char res[5]);
void Save_Wind_Force_To_String(u16 data ,char res[5]);
void Save_Wind_Direction_To_String(u16 data ,char res[8]);



#endif
