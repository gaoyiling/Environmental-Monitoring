/*---------------------------------------------------------------------*/
/* --- STC MCU Limited ------------------------------------------------*/
/* --- STC15F4K60S4 ϵ�� ��ʱ��2��������1�Ĳ����ʷ���������------------*/
/* --- Mobile: (86)13922805190 ----------------------------------------*/
/* --- Fax: 86-0513-55012956,55012947,55012969 ------------------------*/
/* --- Tel: 86-0513-55012928,55012929,55012966-------------------------*/
/* --- Web: www.STCMCU.com --------------------------------------------*/
/* --- Web: www.GXWMCU.com --------------------------------------------*/
/* ���Ҫ�ڳ�����ʹ�ô˴���,���ڳ�����ע��ʹ����STC�����ϼ�����        */
/* ���Ҫ��������Ӧ�ô˴���,����������ע��ʹ����STC�����ϼ�����        */
/*---------------------------------------------------------------------*/

//��ʾ����Keil������������ѡ��Intel��8058оƬ�ͺŽ��б���
//�����ر�˵��,����Ƶ��һ��Ϊ11.0592MHz

#include "stc15.h"
#include "intrins.h"

typedef unsigned char BYTE;
typedef unsigned int WORD;
typedef bit BOOL;

#define FOSC 11059200L          //ϵͳƵ��
#define BAUD_physic 9600             //���ڲ�����
#define BAUD  0xFE80                  // 9600bps @ 11.0592MHz

#define S1_S0 0x40              //P_SW1.6
#define S1_S1 0x80              //P_SW1.7

sbit RXB = P5 ^ 5;                      //define UART TX/RX port
sbit LED = P3 ^ 3;
sbit EN_485 = P3 ^ 2;
bit busy;
bit get_data;
bit addr_ok = 0;

BYTE ADDR = 0x04;	 //0000 0100
BYTE RBUF;     //����/���ջ���
BYTE RDAT;     //����/��������
BYTE RCNT;
BYTE RBIT;     //����/����ֹͣλ
BOOL RING;     //���ڷ���/����
BOOL REND;     //����/�������

WORD times = 0;
BYTE t;
//�������ݼ���
BYTE count = 0;
BYTE count_485 = 0;
//485�ӿ�������Ϣ
BYTE data_485[8] = {0};
//485�ӿ������Ϣ����
BYTE buf[11] = {0};
//��ַ��Ϣ
BYTE buf_addr[8] = {0};
//pm2.5����
BYTE pm_data[32] = {0};

//ģ�⴮�ڳ�ʼ��
void UART_INIT();
//��ʱ��0��ʼ��
void Timer0_INIT();
//����ģ�⴮������
void Save_RXB_data();
//ģ�⴮���������
void Send_TXB_data();
//�������������
void SendData(BYTE dat);
void SendString(char *s);
//��485�������
void SendData_485(BYTE *ptr, int len);
void SendPmInfo();
void SendAddrInfo();
//����CRCУ��λ
unsigned int calc_crc(BYTE *ptr, int len);

void main()
{
    int CRC;
    P3M1 = 0x00;
    P3M0 = 0x08;
    EN_485 = 0;
    WDT_CONTR = 0x24;                               //���Ź�������Լ1.14s��λ
    UART_INIT();
    ACC = P_SW1;
    ACC &= ~(S1_S0 | S1_S1);                        //S1_S0=0 S1_S1=0
    P_SW1 = ACC;                                    //(P3.0/RxD, P3.1/TxD)
    SCON = 0x50;                                    //8λ�ɱ䲨����
    Timer0_INIT();
    ES = 1;                                         //ʹ�ܴ���1�ж�
    EA = 1;
    SendString("MCU START OK!\r\n");
    while (1)
    {
        if (get_data && (data_485[0] == ADDR))  
        {
            CRC = calc_crc(data_485, 6);
            if (((CRC & 0xff) == data_485[6]) && ((CRC >> 8) & 0xff) == data_485[7])
            {
                switch(data_485[1])
                {
                case 0x03:
                    SendPmInfo();
                    break;
                case 0x06:
                    SendAddrInfo();
                    break;
                default:
                    break;
                }
            }
        }
        if (REND)
        {
            REND = 0;
            Save_RXB_data();
        }
        //ι��
        WDT_CONTR |= 0x10;
        if(times == 65535)
        {
            LED = ~LED;
            times = 0;
        }
        times ++;
    }
}

/*----------------------------
UART �жϷ������
-----------------------------*/
void Uart() interrupt 4 using 1
{
    if (RI)
    {
        RI = 0;                 //���RIλ
        if (count_485 == 0)
        {
            data_485[0] = SBUF;
            if (data_485[0] == ADDR)
            {
                count_485++;
            }
        }
        else if (count_485 == 1)
        {
            if (SBUF == 0x03 || SBUF == 0x06)
            {
                data_485[count_485] = SBUF;
                count_485 ++;
            }
            else
            {
                count_485 = 0;
            }
        }
        else if (1 < count_485 && count_485 < 7)
        {

            data_485[count_485] = SBUF;
            count_485 ++;
        }
        else if (count_485 >= 7)
        {
            data_485[count_485] = SBUF;
            addr_ok = 0;
            count_485 = 0;
            get_data = 1;
        }
    }
    if (TI)
    {
        TI = 0;
        busy = 0;
    }
}

/*----------------------------
���ʹ�������
----------------------------*/
void SendData(BYTE dat)
{
    while (busy);
    busy = 1;
    SBUF = dat;                 //д���ݵ�UART���ݼĴ���
}

/*----------------------------
�����ַ���
----------------------------*/
void SendString(char *s)
{
    while (*s)                  //����ַ���������־
    {
        SendData(*s++);         //���͵�ǰ�ַ�
    }
}

//-----------------------------------------
//Timer interrupt routine for UART

void tm0() interrupt 1 using 1
{
    if (RING)
    {
        if (--RCNT == 0)
        {
            RCNT = 3;                   //reset send baudrate counter
            if (--RBIT == 0)
            {
                RBUF = RDAT;            //save the data to RBUF
                RING = 0;               //stop receive
                REND = 1;               //set receive completed flag
            }
            else
            {
                RDAT >>= 1;
                if (RXB) RDAT |= 0x80;  //shift RX data to RX buffer
            }
        }
    }
    else if (!RXB)
    {
        RING = 1;                       //set start receive flag
        RCNT = 4;                       //initial receive baudrate counter
        RBIT = 9;                       //initial receive bit number (8 data bits + 1 stop bit)
    }
}

//initial UART module variable
//����ֻ����

void UART_INIT()
{
    RING = 0;
    REND = 0;
    RCNT = 0;
}
//initial timer0
//Ϊģ�⴮���ṩʱ���ź�
void Timer0_INIT()
{
    TMOD = 0x00;                        //timer0 in 16λ�Զ���װ
    TL0 = BAUD;
    TH0 = BAUD >> 8;                    //initial timer0 ������װֵ
    T2L = (65536 - (FOSC / 4 / BAUD_physic));       //���ò�������װֵ
    T2H = (65536 - (FOSC / 4 / BAUD_physic)) >> 8;
    AUXR = 0x14;                                    //T2Ϊ1Tģʽ, ��������ʱ��2
    AUXR |= 0x01;                                   //ѡ��ʱ��2Ϊ����1�Ĳ����ʷ�����
    AUXR |= 0x80;                        //timer0 working at 1T mode
    TR0 = 1;                                        //tiemr0 start running
    ET0 = 1;                                        //enable timer0 interrupt
    PT0 = 1;                                        //improve timer0 interrupt priority
}

//����ģ�⴮������
void Save_RXB_data()
{
    if (count == 0 && RBUF == 0x42)
    {
        pm_data[count] = RBUF;
        count ++;
    }
    else if (count == 1 && RBUF == 0x4d)
    {
        pm_data[count] = RBUF;
        count ++;
    }
    else if (count > 1 && count < 31)
    {
        if (pm_data[count] == 0x4d && pm_data[count - 1] == 0x42)
        {
            count = 1;
            pm_data[0] = 0x42;
            pm_data[1] = 0x4d;
        }
        else
        {
            pm_data[count] = RBUF;
            count ++;
        }
    }
    else if (count >= 31)
    {
        pm_data[31] = RBUF;
        count = 0;
    }
    else
    {
        return;
    }
}

//�����ڷ���pm2.5����
void SendPmInfo()
{
    unsigned int crc;
    buf[0] = ADDR;
    buf[1] = 0x03;
    buf[2] = 0x06;
    // buf[3] = pm_data[4];
    // buf[4] = pm_data[5];
    // buf[5] = pm_data[6];
    // buf[6] = pm_data[7];
    // buf[7] = pm_data[8];
    // buf[8] = pm_data[9];
    buf[3] = pm_data[10];
    buf[4] = pm_data[11];
    buf[5] = pm_data[12];
    buf[6] = pm_data[13];
    buf[7] = pm_data[14];
    buf[8] = pm_data[15];
    crc = calc_crc(buf, 9);
    buf[9] = crc;
    buf[10] = crc >> 8;
    SendData_485(buf, 11);
}

void SendAddrInfo()
{
    unsigned int crc;
    buf[0] = 0x04;
    buf[1] = 0x06;
    buf[2] = 0x00;
    buf[3] = 0x00;
    buf[4] = 0x00;
    buf[5] = 0x00;
    crc = calc_crc(buf_addr, 6);
    buf[6] = crc;
    buf[7] = crc >> 8;
    SendData_485(buf_addr, 8);
}

void SendData_485(BYTE *ptr, int len)
{
    int i;
    EN_485 = 1;
    _nop_();
    _nop_();
    for (i = 0; i < len; i++)
    {
        SBUF = *ptr;
        while (!TI);
        if (TI)
        {
            TI = 0;
        }
        ptr++;
    }
    EN_485 = 0;
    get_data = 0;
}

unsigned int calc_crc(BYTE *ptr, int len)
{
    int j, k;
    unsigned int c, crc = 0xFFFF;
    //����CRC
    for (k = 0; k < len; k++)
    {
        c = *ptr & 0x00FF;
        crc ^= c;
        ptr++;
        for(j = 0; j < 8; j++)
        {
            if (crc & 0x0001)
            {
                crc >>= 1;
                crc ^= 0xA001;
            }
            else crc >>= 1;
        }
    }
    return(crc);
}
