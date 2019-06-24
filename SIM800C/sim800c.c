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
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK STM32������
//ATK-SIM800C GSM/GPRSģ������        
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//�޸�����:2016/4/1
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2009-2019
//All rights reserved    
//********************************************************************************
//��

//////////////////////////////////////////////////////////////////////////////////
//ATK-SIM800C �������(���Ų��ԡ����Ų��ԡ�GPRS���ԡ���������)���ô���

//sim800C���������,�����յ���Ӧ��
//str:�ڴ���Ӧ����
//����ֵ:0,û�еõ��ڴ���Ӧ����
//����,�ڴ�Ӧ������λ��(str��λ��)
u8* sim800c_check_cmd(char *str)
{
    char *strx=0;
    if(USART2_RX_STA)        //���յ�һ��������
    { 
//        USART2_RX_BUF[USART2_RX_STA&0X7FFF]=0;//��ӽ�����
        strx=strstr((const char*)USART2_RX_BUF,(const char*)str);
    } 
    return (u8*)strx;
}
//��sim800C��������
//cmd:���͵������ַ���(����Ҫ��ӻس���),��cmd<0XFF��ʱ��,��������(���緢��0X1A),���ڵ�ʱ�����ַ���.
//ack:�ڴ���Ӧ����,���Ϊ��,���ʾ����Ҫ�ȴ�Ӧ��
//waittime:�ȴ�ʱ��(��λ:10ms)
//����ֵ:0,���ͳɹ�(�õ����ڴ���Ӧ����)
//       1,����ʧ��
u8 sim800c_send_cmd(char *cmd,char *ack,u16 waittime)
{
    u8 res=0; 
    u8 i = 0;
    u8 len = strlen(cmd);
    printf("%s\r\n", cmd);

//    if((u32)cmd<=0XFF)
//    {
//        while(DMA1_Channel7->CNDTR!=0);    //�ȴ�ͨ��7�������     
//        USART2->DR=(u32)cmd;
//    }
//    else u2_printf("%s\r\n",cmd);//��������U
      USART2_RX_STA=0;
    for (i = 0; i < len; i++)
    {
        while(USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET);
        USART_SendData(USART2, (u16) cmd[i]);
    }
        
    if(ack&&waittime)        //��Ҫ�ȴ�Ӧ��
    { 
        while(--waittime)    //�ȴ�����ʱ
        {
            delay_ms(10);
            if(USART2_RX_STA)//���յ��ڴ���Ӧ����
            {
                if(sim800c_check_cmd(ack))break;//�õ���Ч����
            }
        }
        memset(USART2_RX_BUF, 0x00, USART2_RX_STA+1);
        USART2_RX_STA=0; 
        if(waittime==0)res=1; 
    }
    return res;
} 

/*
����sim800c�Ƿ�����
����ֵ��
    0������
    1��������
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
��ʼ��http���ܣ�׼����������
����ֵ��
    0����ʼ���ɹ�
    1����ʼ��ʧ��
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
������ȡIP��ַ
����ֵ��
    0����ȡIP�ɹ�
    1����ȡIPʧ��
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
��������sim800c_post_data
���ܣ�ִ��postָ��ϴ����ݵ�������
������
    data����Ҫ�ϴ�������
    len �����ݳ���
����ֵ��
    0���ϴ��ɹ�
    1���ϴ�ʧ��
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
��������sim800c_close
�������ܣ��ر��������Ӳ��ػ�
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
��������upload_data
���ܣ��ϴ����ݵ�������
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
��������prepare_data
���ܣ�׼����Ҫ�ϴ���������������
������
    data������ָ��
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
//	//���ٴ�����������ʼ��
//    wind_data.Wind_Speed=0 ;
//    wind_data.Wind_Direction=0;
//    wind_data.Wind_Force=0;
//	//��������������ʼ��
//	
//      Pm_data.PM1_0 =0;
//	Pm_data.PM2_5=0;
//	Pm_data.PM10=0;

