#include "wind_direction.h"
#include  "stm32f10x.h"
#include "wind_speed.h"
#include "stdio.h"
#include "wind.h"
#include "calc.h"
#include "step.h"
 u8 Wind_Data[48]=
{
        0xB7,0xE7,' ',0xCB,0xD9,':',' ',' ','-',' ',' ','m','/','s',' ',0x0A,    //����
        0xB7,0xE7,' ',0xCF,0xF2,':',' ',' ','-',' ',' ',' ',' ',' ',' ',0x0A,    //����
        0xB7,0xE7,' ',0xC1,0xA6,':',' ',' ','-',' ',' ',0xBC,0xB6,' ',' ',0x0A,  //����
        
};


//Wind wind_data={0,0,0};



//��ȡ������
u8 Wind_Get_Data(Wind *Data )
{
    
      Data->Wind_Speed= Receive_Wind_Speed();
	Data->Wind_Force= (u16)Get_Wind_Force(Data->Wind_Speed);
	Data->Wind_Direction=Receive_Wind_Direction();
	return 0;

}



void Save_Wind_Speed_To_String(u16 data ,char res[5])
{
	u8 length;
	length = intlen(data);
        switch(length)
        {
            case 1:
                itoa(data%10, res+4, 1);
		    res[3] = '.';
                res[2] = '0';
                break;
            case 2:
                
		    itoa(data%10, res+4, 1);
                data/=10;
                res[3] = '.';
                itoa(data%10, res+2, 1);
			
             //data/=10;
                //itoa(data%10, res+2, 1);
                break;
            case 3:
                itoa(data%10, res+4, 1);
                data/=10;
                res[3] = '.';
                itoa(data%10, res+2, 1);
                data/=10;
                itoa(data%10, res+1, 1);
                break;
            default:
                break;
        }
}
//���������ʾ����

u8 Save_Wind_Speed(u16 data,u8 row )
{

char res[5]={' ',' ',' ',' ',' '};
	     u8 i;
	     u8 shift =row*Offset ;
     
	Save_Wind_Speed_To_String( data , res);
        for(i = 0;i < 5; i++)
        {
          Wind_Data[5+shift+i] = res[i];
        }
        
		return 0;

}

void Save_Wind_Force_To_String(u16 data ,char res[5])
{
   u8 length;
 if(data==0)
   res[2]='0';
  else
 {
	length = intlen(data);
        switch(length)
        {
            case 1:
                itoa(data%10, res+2, 1);
                break;
            case 2:
                itoa(data%10, res+3, 1);
             data/=10;
                itoa(data%10, res+2, 1);
                break;
            case 3:
                itoa(data%10, res+3, 1);
               data/=10;
                itoa(data%10, res+2, 1);
               data/=10;
                itoa(data%10, res+1, 1);
                break;
            default:
                break;
        }

	}
}
//���������ʾ����
u8  Save_Wind_Force( u16 data,u8 row )
{

char res[5]={' ',' ',' ',' ',' '};
	     u8 i;
	     u8 shift =row*Offset ;
 
  Save_Wind_Force_To_String( data , res);
   for(i = 0;i < 5; i++)
     {
      Wind_Data[6+shift+i] = res[i];
     }
        
		return 0;

}


void Save_Wind_Direction_To_String(u16 data ,char res[8])
{
switch(data)
        {
            case 0x000F:
                // printf("��������\r\n");
                res[3] = 0xB1;//��
                res[2] = 0xB1;
                res[1] = 0xFD;//��
                res[0] = 0xD5;
                break;
            case 0x0000:
                // printf("���򣺶���ƫ��\r\n");
                res[7] = 0xB1;//��
                res[6] = 0xB1;
                res[5] = 0xAB;//ƫ
                res[4] = 0xC6;
                res[3] = 0xB1;//��
                res[2] = 0xB1;
                res[1] = 0xAB;//��
                res[0] = 0xB6;
                break;
            case 0x0001:
                // printf("���򣺶���\r\n");
                res[3] = 0xB1;//��
                res[2] = 0xB1;
                res[1] = 0xAB;//��
                res[0] = 0xB6;
                break;
            case 0x0002:
                // printf("����:����ƫ��\r\n");
                res[7] = 0xAB;//��
                res[6] = 0xB6;
                res[5] = 0xAB;//ƫ
                res[4] = 0xC6;
                res[3] = 0xB1;//��
                res[2] = 0xB1;
                res[1] = 0xAB;//��
                res[0] = 0xB6;
                break;
            case 0x0003:
                // printf("��������\r\n");
                res[3] = 0xAB;//��
                res[2] = 0xB6;
                res[1] = 0xFD;//��
                res[0] = 0xD5;
                break;
            case 0x0004:
                // printf("���򣺶���ƫ��\r\n");
                res[7] = 0xAB;//��
                res[6] = 0xB6;
                res[5] = 0xAB;//ƫ
                res[4] = 0xC6;
                res[3] = 0xCF;//��
                res[2] = 0xC4;
                res[1] = 0xAB;//��
                res[0] = 0xB6;
                break;
            case 0x0005:
                // printf("���򣺶���\r\n");
                res[3] = 0xCF;//��
                res[2] = 0xC4;
                res[1] = 0xAB;//��
                res[0] = 0xB6;
                break;
            case 0x0006:
                // printf("���򣺶���ƫ��\r\n");
                res[7] = 0xCF;//��
                res[6] = 0xC4;
                res[5] = 0xAB;//ƫ
                res[4] = 0xC6;
                res[3] = 0xCF;//��
                res[2] = 0xC4;
                res[1] = 0xAB;//��
                res[0] = 0xB6;
                break;
            case 0x0007:
                // printf("��������\r\n");
                res[3] = 0xCF;//��
                res[2] = 0xC4;
                res[1] = 0xFD;//��
                res[0] = 0xD5;
                break;
            case 0x0008:
                // printf("��������ƫ��\r\n");
                res[7] = 0xCF;//��
                res[6] = 0xC4;
                res[5] = 0xAB;//ƫ
                res[4] = 0xC6;
                res[3] = 0xCF;//��
                res[2] = 0xC4;
                res[1] = 0xF7;//��
                res[0] = 0xCE;
                break;
            case 0x0009:
                // printf("��������\r\n");
                res[3] = 0xCF;//��
                res[2] = 0xC4;
                res[1] = 0xF7;//��
                res[0] = 0xCE;
                break;
            case 0x000A:
                // printf("��������ƫ��\r\n");
                res[7] = 0xF7;//��
                res[6] = 0xCE;
                res[5] = 0xAB;//ƫ
                res[4] = 0xC6;
                res[3] = 0xCF;//��
                res[2] = 0xC4;
                res[1] = 0xF7;//��
                res[0] = 0xCE;
                break;
            case 0x000B:
                // printf("��������\r\n");
                res[3] = 0xF7;//��
                res[2] = 0xCE;
                res[1] = 0xFD;//��
                res[0] = 0xD5;
                break;
            case 0x000C:
                // printf("����:����ƫ��\r\n");
                res[7] = 0xF7;//��
                res[6] = 0xCE;
                res[5] = 0xAB;//ƫ
                res[4] = 0xC6;
                res[3] = 0xB1;//��
                res[2] = 0xB1;
                res[1] = 0xF7;//��
                res[0] = 0xCE;
                break;
            case 0x000D:
                // printf("��������\r\n");
                res[3] = 0xB1;//��
                res[2] = 0xB1;
                res[1] = 0xF7;//��
                res[0] = 0xCE;
                break;
            case 0x000E:
                // printf("��������ƫ��\r\n");
                res[7] = 0xB1;//��
                res[6] = 0xB1;
                res[5] = 0xAB;//ƫ
                res[4] = 0xC6;
                res[3] = 0xB1;//��
                res[2] = 0xB1;
                res[1] = 0xF7;//��
                res[0] = 0xCE;
                break;
            default:
                break;
        }
}
//���������ʾ����
u8  Save_Wind_Direction( u16 data,u8 row )
{
  char res[8]={' ',' ',' ',' ',' ',' ',' ',' '};
	     u8 i;
	     u8 shift =row*Offset ;
 
       Save_Wind_Direction_To_String( data , res);
        

      for(i = 0;i < 8; i++)
        {
          Wind_Data[6+shift+i] = res[i];
        }
        
		return 0;



}

//�������з������ʾ����
u8 Process_Wind(Wind *Data)
{
    Save_Wind_Speed( Data->Wind_Speed,0 );
    Save_Wind_Direction( Data->Wind_Direction,1 );
    Save_Wind_Force( Data->Wind_Force,2 );
   
	return 0;
	
}



