
#include <xc.h>
#include "display.h"

#define comprimento_pneu 120

unsigned long contador_vel=0;
unsigned long ultimo_contador_vel=0;
unsigned long contador_rpm=0;
unsigned long ultimo_contador_rpm=0;
double odometro=0;

void main(void) {
    display_setup();
    display_send_string("teste");
    sensores_setup();
    TRISB=0xFF;
    ADCON1=0x0F;
    while(1)
    {
        unsigned long velocidade=comprimento_pneu*360.0/ultimo_contador_vel;
        float rpm=600000.0/ultimo_contador_rpm;
        display_send_command(DISPLAY_OFF);
        display_send_command(DISPLAY_CLEAR);
        display_send_string("VEL(km/h):");
        display_send_int(velocidade);
        display_set_position(1,0);
        display_send_string("RPM:");
        display_send_int((long)rpm);
        display_send_string(" ODO:");
        display_send_int((long)odometro);
        display_send_command(DISPLAYON_CURSOROFF_BLINKOFF);
        __delay_ms(1000);
    }
    return;
}
sensores_setup()
{
    TRISB=0xFF;
    ADCON1=0x0F;
    
    INTCONbits.GIE=1;
    INTCONbits.PEIE=1;
    
    INTCONbits.INT0E=1;
    INTCONbits.INT0F=0;
    
    INTCON3bits.INT2E=1;
    INTCON3bits.INT2F=1;
    
    T2CON=0b00000001;
    PIE1bits.TMR2IE=1;
    PIR1bits.TMR2IF=0;
    PR2=250;
    T2CONbits.TMR2ON=1;
}
void __interrupt() isr()
{
    if(INTCONbits.INT0F)
    {
        INTCONbits.INT0F=0;
        ultimo_contador_vel=contador_vel;
        contador_vel=0;
        odometro+=comprimento_pneu/100000.0;
    }
    if(INTCON3bits.INT2F)
    {
        INTCON3bits.INT2F=0;
        ultimo_contador_rpm=contador_rpm;
        contador_rpm=0;
    }
    if(PIR1bits.TMR2IF)
    {
        PIR1bits.TMR2IF=0;
        contador_vel+=1;
        contador_rpm+=1;
    }
}