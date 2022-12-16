#include "display.h"

void display_setup()
{
    TRISD = 0x00;
    PORTD = 0x00;
    __delay_ms(20);
    display_send_nibble(0b00000011);
    __delay_ms(5);
    display_send_nibble(0b00000011);
    __delay_us(150);
    display_send_nibble(0b00000011);
    __delay_us(50);
    display_send_nibble(0b00000010);
    __delay_us(50);
    display_send_command(FUNCTIONSET_4BIT_2LINE);
    display_send_command(DISPLAY_OFF);
    display_send_command(DISPLAY_CLEAR);
    display_send_command(ENTRYMODE_INCREMENTON_SHIFTOFF);
    display_send_command(DISPLAYON_CURSOROFF_BLINKOFF);
}
void display_send_int(long numero)
{
    char s_numero[10];
    if(numero<0)
    {
        display_send_char('-');
        numero=-numero;
    }
    sprintf(s_numero,"%li",numero);
    display_send_string(s_numero);
}
void display_send_float(float numero)
{
    char s_numero[10];
    if(numero<0)
    {
        display_send_char('-');
        numero=-numero;
    }
    sprintf(s_numero,"%f",numero);
    display_send_string(s_numero);
}
void display_send_string(char* str)
{
    int i=0;
    while(str[i]!='\0')
    {
        display_send_char(str[i]);
        i++;
    }
}
void display_send_char(char data)
{
    while(busy());
    __delay_us(1);
    Display_rs=1;
    Display_rw=0;
    __delay_us(1);
    display_send_nibble(data>>4);
    __delay_us(1);
    display_send_nibble(data);
    __delay_us(1);
    return;
}
void display_send_command(char data)
{
    while(busy());
    __delay_us(1);
    Display_rs=0;
    Display_rw=0;
    __delay_us(1);
    display_send_nibble(data>>4);
    __delay_us(1);
    display_send_nibble(data);
    __delay_us(1);
    return;
}
void display_send_nibble(char data)
{
    Display_d4=(data&0x01)?1:0;
    Display_d5=(data&0x02)?1:0;
    Display_d6=(data&0x04)?1:0;
    Display_d7=(data&0x08)?1:0;
    __delay_us(1);
    Display_e=1;
    __delay_us(1);
    Display_e=0;
    __delay_us(1);
}
void display_set_position(char linha, char coluna)
{
   char posicao=0;
   if(linha == 0)
   {
        posicao=0x80;     
   }
   if(linha == 1)
   {
        posicao=0x80|0x40;     
   }
 
    posicao=posicao|coluna;     

    display_send_command(posicao);
    return;
}
char busy()
{
    T_d7=1;
    T_d6=1;
    T_d5=1;
    T_d4=1;
    Display_rs=0;
    Display_rw=1;
    __delay_us(1);
    Display_e=1;
    __delay_us(1);
    char busy=Display_d7;
    Display_e=0;
    __delay_us(1);
    Display_e=1;
    __delay_us(1);
    Display_e=0;
    __delay_us(1);
    T_d7=0;
    T_d6=0;
    T_d5=0;
    T_d4=0;
    return busy;
}