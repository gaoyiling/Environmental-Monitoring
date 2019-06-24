#ifndef __PARTICULATE_MATTER_H__
#define __PARTICULATE_MATTER_H__
#include "stm32f10x.h"

#define PM_Receive_Len 11  //�������ݳ���
//#define Offset 16    // ��ƫ����

extern u8 PM_Data[48];
typedef struct 
{
	u16 PM1_0 ;
	u16 PM2_5;
	u16 PM10;
} pm;

extern pm Pm_data;
extern pm buf_Pm_data;
 //pm Pm_Data={0,0,0};
//u8 cmd[8] = {0x04, 0x03, 0x00, 0x00, 0x00, 0x06, 0xC5, 0x9D};
void PM_Send_Data(u8 *Data, u8 Len);//����������������
u8 PM_Receive_Data(pm *Data );//���մ���������
u8 Process_Pm( pm *Data);//��������pm����
u8 Save_Pm(u16 Pm_Data,u8 row );//���ַ�����ʽ�洢���ݵ���pm����
u8 Save_Pm1_0(u16 data,u8 row );
u8 Save_Pm2_5(u16 data,u8 row );
u8 Save_Pm10(u16 data,u8 row );
void Save_Pm1_0_To_String(u16 data ,char res[5]);
void Save_Pm2_5_To_String(u16 data ,char res[5]);
void Save_Pm10_To_String(u16 data ,char res[5]);

#endif
