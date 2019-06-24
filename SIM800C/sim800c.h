#ifndef __SIM800C_H__
#define __SIM800C_H__	 
#include "sys.h"
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK STM32开发板
//ATK-SIM800C GSM/GPRS模块驱动	  
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//修改日期:2016/4/1
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2009-2019
//All rights reserved	
//********************************************************************************
//无
//////////////////////////////////////////////////////////////////////////////////	

#define swap16(x) (x&0XFF)<<8|(x&0XFF00)>>8	//高低字节交换宏定义
#define SIM800C_PWK PAout(4)  // PA8

extern u8 Scan_Wtime;
extern u8 sim800c_busy;


void sim_send_sms(u8*phonenumber,u8*msg);
void sim_at_response(u8 mode);	
u8*  sim800c_check_cmd(char *str);
u8   sim800c_send_cmd(char *cmd,char *ack,u16 waittime);
u8   sim800c_test(void);
u8   sim800c_http_init(void);
u8   sim800c_set_ip(void);
u8   sim800c_post_data(char *data, u32 len);
u8   sim800c_jihuo(void);
u8   sim800c_httpinit(void);
void sim800c_close(void);
void sim800c_shutdown(void);
void upload_data(void);
void prepare_data(char* data);
char* check_sum(char* data,int len);
void luat_send_data(char *data);

void sim800c_tcp_init(void);
//    //void TemperatureToString(u16 Data,char *res);
//   // void HumidityToString(u16 Data,char *res);;
//     //void NoiseToString(u16 Data,char *res);;
//	//
//     //void SpeedToString(u16 Data,char *res); ;
//    // void DirectionToString(u16 Data,char *res);;
//     void ForceToString(u16 Data,char *res);;
//	
//	
//       void PPM1_0ToString(u16 Data,char *res); ;
//	 void PM2_5ToString(u16 Data,char *res);;
//	 void PM10ToString(u16 Data,char *res);;

#endif
