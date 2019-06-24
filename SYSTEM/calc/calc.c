#include "stm32f10x.h"
//#include "calc.h"
#include "string.h"
#include "calc.h"
// �������α����ĳ���  
u8 intlen(u16 m){  
    u8 length = 0;  
    if (m == 0)
    {
        length = 1;
    }
    while (m != 0) 
    {  
        length++;  
        m /= 10;  
    }
    return length;
}

void itoa(u16 n,char str[5], u8 len)
{
  
  u8 i=len-1;

  memset(str,0x20,len);
  do{
  str[i--]=n%10+'0';
  
 }while((n/=10)>0);

 return;
}

// ��int ת��Ϊ��Ӧ���ַ���  
void intToStr(char* num,u16 number){  
    u8 length = intlen(number); 
    itoa(number, num, length);
}
