#include "sim800c.h"
#include "usart.h"        
#include "delay.h"    
#include "string.h"        
#include "usart2.h" 
#include "config.h"
#include "rs485.h"
#include "calc.h"
#include "wind.h"
#include "environment.h"
#include "particulate_matter.h"
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
//ATK-SIM800C 各项测试(拨号测试、短信测试、GPRS测试、蓝牙测试)共用代码

//sim800C发送命令后,检测接收到的应答
//str:期待的应答结果
//返回值:0,没有得到期待的应答结果
//其他,期待应答结果的位置(str的位置)
u8* sim800c_check_cmd(char *str)
{
    char *strx=0;
    if(USART2_RX_STA)        //接收到一次数据了
    { 
//        USART2_RX_BUF[USART2_RX_STA&0X7FFF]=0;//添加结束符
        strx=strstr((const char*)USART2_RX_BUF,(const char*)str);
    } 
    return (u8*)strx;
}
//向sim800C发送命令
//cmd:发送的命令字符串(不需要添加回车了),当cmd<0XFF的时候,发送数字(比如发送0X1A),大于的时候发送字符串.
//ack:期待的应答结果,如果为空,则表示不需要等待应答
//waittime:等待时间(单位:10ms)
//返回值:0,发送成功(得到了期待的应答结果)
//       1,发送失败
u8 sim800c_send_cmd(char *cmd,char *ack,u16 waittime)
{
    u8 res=0; 
    u8 i = 0;
    u8 len = strlen(cmd);
    printf("%s\r\n", cmd);

//    if((u32)cmd<=0XFF)
//    {
//        while(DMA1_Channel7->CNDTR!=0);    //等待通道7传输完成     
//        USART2->DR=(u32)cmd;
//    }
//    else u2_printf("%s\r\n",cmd);//发送命令U
      USART2_RX_STA=0;
    for (i = 0; i < len; i++)
    {
        while(USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET);
        USART_SendData(USART2, (u16) cmd[i]);
    }
        
    if(ack&&waittime)        //需要等待应答
    { 
        while(--waittime)    //等待倒计时
        {
            delay_ms(10);
            if(USART2_RX_STA)//接收到期待的应答结果
            {
                if(sim800c_check_cmd(ack))break;//得到有效数据
            }
        }
        memset(USART2_RX_BUF, 0x00, USART2_RX_STA+1);
        USART2_RX_STA=0; 
        if(waittime==0)res=1; 
    }
    return res;
} 

/*
测试sim800c是否在线
返回值：
    0：在线
    1：不在线
*/
u8 sim800c_test()
{
    u8 i = 1;
//    u8 times = 10;

    if(!sim800c_send_cmd("AT\r\n", "OK",100))
    {
        SIM800C_PWK = 0;
        i = 0;
    }
    else
    {
        SIM800C_PWK = 1;
        printf("SIM800C power on.....\r\n");
//      while(SIM800C_PWK && times--)
//      {
//           if(!sim800c_send_cmd("AT", "OK", 100))
//           {
//                SIM800C_PWK = 0;
//                i = 0;
//                break;
//           }
//           delay_ms(10);
//      }
    }
    return i;
}
/*
初始化http功能，准备发送数据
返回值：
    0：初始化成功
    1：初始化失败
*/
u8 sim800c_http_init(void)
{
    if (!sim800c_send_cmd("AT+SAPBR=2,1\r\n", "1,1,", 100))
    {
        if (!sim800c_send_cmd("AT+HTTPINIT\r\n", "OK", 100))
        {
            if (!sim800c_send_cmd("AT+HTTPPARA=\"CID\",1\r\n", "OK", 100))
            {
               if (!sim800c_send_cmd("AT+HTTPPARA=\"URL\",\"qtmami.com/device/data\"\r\n", "OK", 100))
               {
                       return 0;
               }
            }
        }
    }
    else
    {
        if (!sim800c_set_ip())
        {
            delay_ms(100);
            if (!sim800c_httpinit())
            {
                if (!sim800c_send_cmd("AT+HTTPPARA=\"CID\",1\r\n", "OK", 100))
                {
                    if (!sim800c_send_cmd("AT+HTTPPARA=\"URL\",\"qtmami.com/device/data\"\r\n", "OK", 100))
                    {
                        return 0;
                    }
                }
            }
        }
    }
    return 1;
}

u8 sim800c_httpinit(void)
{
    u8 times = 4;
    while(--times)
    {
        if(!sim800c_send_cmd("AT+HTTPINIT\r\n", "OK", 100))
        {
            break;
        }
    }
    if(times)
    {
        return 0;
    }
    else
    {
        return 1;
    }
}

/*
联网获取IP地址
返回值：
    0：获取IP成功
    1：获取IP失败
*/
u8 sim800c_set_ip(void)
{
    if (!sim800c_send_cmd("AT+SAPBR=3,1,\"CONTYPE\",\"GPRS\"\r\n", "OK", 10))
    {
        if (!sim800c_send_cmd("AT+SAPBR=3,1,\"APN\",\"CMNET\"\r\n", "OK", 10))
        {
            if (!sim800c_jihuo())
            {
                return 0;
            }
        }
    }
    return 1;
}

u8 sim800c_jihuo(void)
{
    u8 times = 4;
    while(--times)
    {
        if(!sim800c_send_cmd("AT+SAPBR=1,1\r\n", "OK", 200))
        {
            break;
        }
        else
        {
            if (!sim800c_send_cmd("AT+SAPBR=2,1\r\n", "1,1,", 100))
            {
                break;
            }
        }
    }
    if (!sim800c_send_cmd("AT+SAPBR=2,1\r\n", "3,1,", 100))
    {
        return 1;
    }
    else
    {
        if (!sim800c_send_cmd("AT+SAPBR=2,1\r\n", "1,1,", 100))
        {
            return 0;
        }
        else
        {
            return 1;
        }
    }
}

/*
函数名：sim800c_post_data
功能：执行post指令，上传数据到服务器
参数：
    data：需要上传的数据
    len ：数据长度
返回值：
    0：上传成功
    1：上传失败
*/
u8 sim800c_post_data(char *data, u32 len)
{
    u8 i = 1;
    if (!sim800c_http_init())
    {
        char cmd_httpdata[25] = {0};
        char length[3] = {0};
        strcat(cmd_httpdata, "AT+HTTPDATA=");
        intToStr(length, len);
        strcat(cmd_httpdata, length);
        strcat(cmd_httpdata,",100000");
                strcat(cmd_httpdata,"\r\n");
        if (!sim800c_send_cmd(cmd_httpdata, "DOWNLOAD", 100))
        {
            if(!sim800c_send_cmd(data, "OK", 100))
            {
                if (!sim800c_send_cmd("AT+HTTPACTION=1\r\n", "200", 2000))
                {
                    printf("send data success\r\n");
                    i = 0;
                }
                else
                {
                    printf("send data failed\r\n");
                }
            }
        }
    }
    sim800c_close();
    return i;
}

/*
函数名：sim800c_close
函数功能：关闭网络连接并关机
*/
void sim800c_close()
{
    sim800c_send_cmd("AT+HTTPTERM\r\n","OK", 20);
    if(sim800c_send_cmd("AT+SAPBR=0,1\r\n", "OK", 200))
        {
            sim800c_shutdown();
        }
}

void sim800c_shutdown()
{
    if(!sim800c_send_cmd("AT+CPOWD=1\r\n", "POWER DOWN", 200))
    {
        printf("power off\r\n");
    }
    else
    {
        printf("power off hard\r\n");
        SIM800C_PWK = 1;
        delay_ms(1000);
        delay_ms(1000);
        SIM800C_PWK = 0;
    }
}

void sim800c_tcp_init(void)
{
	
}

/*
函数名：upload_data
功能：上传数据到服务器
*/
void upload_data()
{
    char data[512] = {0};
    prepare_data(data);
		luat_send_data(data);
//    sim800c_post_data(data, strlen(data));
//    if(sim800c_post_data(data, strlen(data)))sim800c_shutdown();
}

void luat_send_data(char* data)
{
	if(!sim800c_send_cmd(data,"OK", 200))
	{
		printf("send luat success\r\n");
	}
	else
	{
		printf("send luat failed\r\n");
	}
//	sim800c_send_cmd(check_sum(data,strlen(data)),"",0);
//	sim800c_send_cmd(&end,"OK", 200);
}

/*
函数名：prepare_data
功能：准备需要上传到服务器的数据
参数：
    data：数据指针
*/
void prepare_data(char* data)
{
    
    char num[5] = {0};
    char str_temp[20]  = {0};
    char str_hum[20]  = {0};
    char str_nos[20]  = {0};
    char str_ws[20]  = {0};
    char str_wd[20]  = {0};
    char str_pm1d0[20]  = {0};
    char str_pm2d5[20]  = {0};
    char str_pm10[20]  = {0};

    char head[24] = "{\"device\":\"";
    strcat(data,head);//{"device":"
    strcat(data,device_id);//{"device":"YR171120000
    strcat(data,"\",\"data\":{");//{"device":"YR171120000","data":{

    strcat(str_temp, "\"temp\":");
    strcat(str_hum, "\"hum\":");
    strcat(str_nos, "\"nos\":");
    strcat(str_pm1d0, "\"pm1d0\":");
    strcat(str_pm2d5, "\"pm2d5\":");
    strcat(str_pm10, "\"pm10\":");
    strcat(str_ws, "\"ws\":");
    strcat(str_wd, "\"wd\":");
    
    


}


//    
//    Environment_data.Temperature=0;
//    Environment_data.Humidity=0;
//    Environment_data.Noise=0;
//	//风速传感器参数初始化
//    wind_data.Wind_Speed=0 ;
//    wind_data.Wind_Direction=0;
//    wind_data.Wind_Force=0;
//	//空气颗粒参数初始化
//	
//      Pm_data.PM1_0 =0;
//	Pm_data.PM2_5=0;
//	Pm_data.PM10=0;

