#include <xc.h>
#include <stdio.h> 
#include <stdlib.h>

#define _XTAL_FREQ 8000000

#pragma config PBADEN = OFF

int captura_h, captura_l;
int periodo=0;
int frequencia=0;

void Display(int num);
void Numero(int num);

__interrupt() void MY_ISR(void)
{
	if (PIR1bits.CCP1IF)
	{
		TMR1H = 0;
		TMR1L = 8;	//desconta a execução do programa
		PIR1bits.CCP1IF = 0;
		captura_h = CCPR1H;
		captura_l = CCPR1L;	
        
        
	}		
}
void Numero(int num){
    
   switch(num)
    {
        case 0: 
            PORTD=0b11000000;
            break;
        case 1: 
            PORTD=0b11111001;
            break;
        case 2: 
            PORTD=0b10100100;
            break;
        case 3: 
            PORTD=0b10110000;
            break;
        case 4: 
            PORTD=0b10011001;
            break;
        case 5: 
            PORTD=0b10010010;
            break;
        case 6: 
            PORTD=0b10000010;
            break;
        case 7: 
            PORTD=0b11111000;
            break;
        case 8: 
            PORTD=0b10000000;
            break;
        case 9: 
            PORTD=0b10010000;
            break;
    }
}
void Display(int num){
    
    Numero(num/1000u);
    PORTCbits.RC4 = 1; 
    __delay_ms(10);
    PORTCbits.RC4 = 0;
    
    Numero((num / 100u) % 10u);
    PORTCbits.RC5 = 1;
    __delay_ms(10);
    PORTCbits.RC5 = 0; 
    
    Numero((num / 10u) % 10u);
    PORTCbits.RC6 = 1;
    __delay_ms(10);
    PORTCbits.RC6 = 0;
    
    Numero(num % 10u);
    PORTCbits.RC7 = 1;
    __delay_ms(10);
    PORTCbits.RC7 = 0;
}

void main()
{
	
//  CONFIGURANDO MODO CAPTURA  //
    
	//pino CCP1 como entrada
	TRISC=0b00001111;
    
	//Captura para toda borda de descida.
	CCP1CONbits.CCP1M3 = 0;	
	CCP1CONbits.CCP1M2 = 1;			
	CCP1CONbits.CCP1M1 = 0;			
	CCP1CONbits.CCP1M0 = 0;	
	
	// configurando TMR1
	T1CON = 0b10010101;	// 16bits, prescaler 1:2, fosc/4 - clock interno, TMR1 ligado
	
	//configura interrupção CCP1	
	IPR1bits.CCP1IP = 1;		//alta priorida
	PIE1bits.CCP1IE = 1;		//int. habilitada
	PIR1bits.CCP1IF = 0;		//inicia flag
	INTCONbits.GIE = 1;
	INTCONbits.PEIE = 1;		//habilitador geral do CCP1

// CONFIGURANDO OUTPUT E VALORES INICIAIS P/ LED-7SEG //
    PORTC=0b00000000;
    TRISD=0b00000000;
    PORTD=0b00000000;
	
	while(1)
	{	
        periodo=captura_h<<8|captura_l;
        frequencia=1l/(periodo*0.000001);
        Display(frequencia);
	}
}
