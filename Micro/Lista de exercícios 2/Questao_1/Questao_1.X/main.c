#include <xc.h>
#include <stdio.h> 
#include <stdlib.h>
#include <p18f4520.h>
#define _XTAL_FREQ 4000000 
#define LED PORTDbits.RD0

#pragma config PBADEN = OFF

void interrupt ISR(){
    if(INTCONbits.INT0IF==1){
        INTCON2bits.INTEDG0=!INTCON2bits.INTEDG0;
        INTCONbits.INT0IF=0;
        LED=!LED;
    }
    
    if(INTCON3bits.INT1IF==1){
        INTCON2bits.INTEDG1=!INTCON2bits.INTEDG1;
        INTCON3bits.INT1IF=0;
        LED=!LED;
    }
    
    if(INTCON3bits.INT2IF==1){
        INTCON2bits.INTEDG2=!INTCON2bits.INTEDG2;
        INTCON3bits.INT2IF=0;
        LED=!LED;
    } 
}

void main(void) {
    TRISD=0b11111110;
    TRISBbits.TRISB0=1;
    LED=0;
    INTCON2bits.RBPU=0;
    
    INTCONbits.INT0IF=0;
    INTCONbits.INT0IE=1;
    
    INTCON3bits.INT1IF=0;
    INTCON3bits.INT1IE=1;
    
    INTCON3bits.INT2IF=0;
    INTCON3bits.INT2IE=1;
    
    INTCON2bits.INTEDG0=0;
    INTCON2bits.INTEDG1=0;
    INTCON2bits.INTEDG2=0;
    
    INTCONbits.GIE=1;
    
    while(1){
        __delay_ms(1000);
    }
    
    return;
}
