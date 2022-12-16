
#include <xc.h>
#include "uart.h"
#include "pwm.h"
#include <string.h>

#define _XTAL_FREQ 40000000

#define zona1 PORTBbits.RB4
#define zona2 PORTBbits.RB5
#define zona3 PORTBbits.RB6
#define zona4 PORTBbits.RB7


char last_zona1=0,last_zona2=0,last_zona3=0,last_zona4=0;
char direcao=1;
unsigned long contador_tempo=0;
unsigned long contador_alarme=0;
int armado=0,duty=0;
char comando[30];
int i=0;

void ativa_alarme_sonoro();
void configura_alarme_sonoro();
void configura_relogio();
void desativa_alarme_sonoro();
void dispara_alarme(int zona);
void envia_log(int zona);
void loop();
void setup();
unsigned long tempo_corrente();
void set_tempo_corrente(unsigned long tempo_corrente);
void __interrupt(high_priority) isr_uart_rx()
{
    if(PIR1bits.RCIF)
    {
        char c=recebe_char();
        if(c=='\n');
        else if(c!='\r')
        {
            comando[i]=c;
            i+=1;
        }
        else
        {
            comando[i]='\0';
            if(strcmp(comando,"ARMA")==0)
            {
                armado=1;
                transmite_string("Alarme armado\r\n");
            }
            else if(strcmp(comando,"DESARMA")==0)
            {
                armado=0;
                transmite_string("Alarme desarmado\r\n");
            }
            else if(comando[2]==':'&&comando[5]==':')
            {
                unsigned int tempo=0;
                unsigned int aux=0;
                aux=comando[7]-'0';
                aux+=(comando[6]-'0')*10;
                tempo+=aux;
                aux=comando[4]-'0';
                aux+=(comando[3]-'0')*10;
                tempo+=aux*60;
                aux=comando[1]-'0';
                aux+=(comando[0]-'0')*10;
                tempo+=aux*60*60;
                set_tempo_corrente(tempo);
                transmite_string("Horario definido para : ");
                transmite_string(comando);
                transmite_string("\r\n");
            }
            else
            {
                transmite_string("Comando n�o reconhecido : ");
                transmite_string(comando);
                transmite_string("\r\n");
            }
            i=0;
        }
    }
}
void __interrupt(low_priority) isr_low_priority()
{
    if(INTCONbits.TMR0IF)
    {
        INTCONbits.TMR0IF=0;
        contador_tempo++;
        if(contador_tempo>=102998)
        {
            contador_tempo=0;
        }
    }
    if(PIR2bits.TMR3IF)
    {
        PIR2bits.TMR3IF=0;
        contador_alarme++;
        if(contador_alarme>=100)//quase 5 segundos
            desativa_alarme_sonoro();
    }
    if(PIR1bits.TMR2IF)
    {
        PIR1bits.TMR2IF=0;
        if(duty==0);
        else if(direcao==1)
        {
            if(duty<1000)
                duty+=1;
            else
                direcao=0;
        }
        else
        {
            if(duty>500)
                duty-=1;
            else
                direcao=1;
        }
        set_duty_pwm(duty);
    }
    if(INTCONbits.RBIF)
    {
        INTCONbits.RBIF=0;
        if(zona1==1&&last_zona1==0&&armado)
            dispara_alarme(1);
        if(zona2==1&&last_zona2==0&&armado)
            dispara_alarme(2);
        if(zona3==1&&last_zona3==0&&armado)
            dispara_alarme(3);
        if(zona4==1&&last_zona4==0&&armado)
            dispara_alarme(4);
        last_zona1=zona1;
        last_zona2=zona2;
        last_zona3=zona3;
        last_zona4=zona4;
    }
}
void setup()
{
    configura_uart();
    configura_alarme_sonoro();
    configura_relogio();
    TRISB=0b11111111;
    ADCON1=0x0F;
    //interrup��o porta b
    INTCONbits.RBIE=1;
    INTCON2bits.RBIP=0;
    INTCONbits.RBIF=0;
    //fim interrup��o porta b
    INTCONbits.GIE=1;
    INTCONbits.PEIE=1;
    RCONbits.IPEN=1;
}

void main(void) 
{
    setup();
    while(1);
    return;
}
void configura_relogio()
{
    T0CON=0b10000111;
    INTCONbits.TMR0IE=1;
    INTCONbits.TMR0IF=0;
    INTCON2bits.TMR0IP=0;
}
unsigned long tempo_corrente()
{
    return contador_tempo*0.838848;
}
void set_tempo_corrente(unsigned long tempo_corrente)
{
    contador_tempo=tempo_corrente/0.838848;
}
void configura_alarme_sonoro()
{
    configura_pwm();
    T3CON=0b10110100;
    PIE2bits.TMR3IE=1;
    PIR2bits.TMR3IF=0;
    IPR2bits.TMR3IP=0;
}
void ativa_alarme_sonoro()
{
    duty=255;
    set_duty_pwm(duty);
    contador_alarme=0;
    TMR3H=0;
    TMR3L=0;
    T3CONbits.TMR3ON=1;
}
void desativa_alarme_sonoro()
{
    duty=0;
    set_duty_pwm(duty);
    T3CONbits.TMR3ON=0;
}

void dispara_alarme(int zona)
{
    ativa_alarme_sonoro();
    envia_log(zona);
}

void envia_log(int zona)
{
    char string_hora[]="xx:xx:xx";
    unsigned long total=tempo_corrente();
    int ss=total%60;
    total=total/60;
    int min=total%60;
    total=total/60;
    int hh=total%24;
    string_hora[6]=ss/10+'0';
    string_hora[7]=ss%10+'0';
    string_hora[3]=min/10+'0';
    string_hora[4]=min%10+'0';
    string_hora[0]=hh/10+'0';
    string_hora[1]=hh%10+'0';
    transmite_string(string_hora);
    transmite_char('-');
    transmite_string("Alarme ativado na ZONA ");
    transmite_char(zona%10+'0');
    transmite_string("\r\n");
    return;
}
