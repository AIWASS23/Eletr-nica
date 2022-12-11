#include <xc.h>
#include <stdio.h> 
#include <stdlib.h>

#define _XTAL_FREQ 4000000 

#define ROW1 PORTBbits.RB3
#define ROW2 PORTBbits.RB4
#define ROW3 PORTBbits.RB5
#define ROW4 PORTBbits.RB6

#define COL1 PORTBbits.RB0
#define COL2 PORTBbits.RB1
#define COL3 PORTBbits.RB2

#define Porta_Lcd PORTD
#define RS PORTCbits.RC0
#define E PORTCbits.RC1

#pragma config PBADEN = OFF

char command=0;
char getChar(int col);
void Envia_Comando_Lcd (char comando);
void Envia_Dado_Lcd (char dado);
void Inicializa_Lcd (void);
void Print_Mensagem_Lcd (char *mensagem);
void Delay_inicializacao_LCD();



void interrupt ISR(){
    if(INTCONbits.INT0IF==1){
        INTCONbits.INT0IF=0;
        command=getChar(1);
    }
    
    if(INTCON3bits.INT1IF==1){
        INTCON3bits.INT1IF=0;
        command=getChar(2);
    }
    
    if(INTCON3bits.INT2IF==1){
        INTCON3bits.INT2IF=0;
        command=getChar(3);
    }
    Envia_Dado_Lcd(command);
    
}

void main(void) {
    
    //Configura ES do porte B 1=INPUT 0=OUTPUT
    TRISB=0b10000111;
    TRISC=0b11111100; 
	TRISD=0b00000000; 
    //Habilita resistor de pullup nas interrupções
    INTCON2bits.RBPU=0;
    
    //Habilita interrupção INT0 e coloca flag em 0
    INTCONbits.INT0IF=0;
    INTCONbits.INT0IE=1;
    
    //Habilita interrupção INT1 e coloca flag em 0
    INTCON3bits.INT1IF=0;
    INTCON3bits.INT1IE=1;
    
    //Habilita interrupção INT2 e coloca flag em 0
    INTCON3bits.INT2IF=0;
    INTCON3bits.INT2IE=1;
    
    //Configura interrupções para borda de descida
    INTCON2bits.INTEDG0=0;
    INTCON2bits.INTEDG1=0;
    INTCON2bits.INTEDG2=0;
    //Habilita interrupções
    INTCONbits.GIE=1;
    
    ROW1=0;
    ROW2=0;
    ROW3=0;
    ROW4=0;
    
    
	Inicializa_Lcd ();
    
    while(1){
        if(command=='#')Envia_Comando_Lcd(1);
        
        
    }
    
    return;
}

char getChar(int col){
    switch(col){
        case 1:
            ROW1=1;
            ROW2=0;
            ROW3=0;
            ROW4=0;
            
            if(COL1==1){
                ROW1=0;
                INTCONbits.INT0IF=0;
                return '1';
            }
            __delay_ms(10);
            ROW1=0;
            ROW2=1;
            ROW3=0;
            ROW4=0;
            
            if(COL1==1){       
                ROW2=0;
                INTCONbits.INT0IF=0;
                return '4';
            }
            __delay_ms(10);
            ROW1=0;
            ROW2=0;
            ROW3=1;
            ROW4=0;
            
            if(COL1==1){ 
                ROW3=0;
                INTCONbits.INT0IF=0;
                return '7';
            }
            __delay_ms(10);
            ROW1=0;
            ROW2=0;
            ROW3=0;
            ROW4=1;
            
            if(COL1==1){ 
                ROW4=0;
                INTCONbits.INT0IF=0;
                return '*';
            }
            __delay_ms(10);
            ROW1=0;
            ROW2=0;
            ROW3=0;
            ROW4=0;
            
         
        case 2:
            ROW1=1;
            ROW2=0;
            ROW3=0;
            ROW4=0;
            
            if(COL2==1){
                ROW1=0;
                INTCON3bits.INT1IF=0;
                return '2';
            }
            __delay_ms(10);
            ROW1=0;
            ROW2=1;
            ROW3=0;
            ROW4=0;
            
            if(COL2==1){    
                ROW2=0;
                INTCON3bits.INT1IF=0;
                return '5';
            }
            __delay_ms(10);
            ROW1=0;
            ROW2=0;
            ROW3=1;
            ROW4=0;
            
            if(COL2==1){
                ROW3=0;
                INTCON3bits.INT1IF=0;
                return '8';
            }
            __delay_ms(10);
            ROW1=0;
            ROW2=0;
            ROW3=0;
            ROW4=1;
            
            if(COL2==1){
                ROW4=0;
                INTCON3bits.INT1IF=0;
                return '0';
            }
            __delay_ms(10);
            ROW1=0;
            ROW2=0;
            ROW3=0;
            ROW4=0;
         
        case 3:
            ROW1=1;
            ROW2=0;
            ROW3=0;
            ROW4=0;
            
            if(COL3==1){
                ROW1=0;
                INTCON3bits.INT2IF=0;
                return '3';
            }
            __delay_ms(10);
            ROW1=0;
            ROW2=1;
            ROW3=0;
            ROW4=0;
            
            if(COL3==1){
                ROW2=0;
                INTCON3bits.INT2IF=0;
                return '6';
            }
            __delay_ms(10);
            ROW1=0;
            ROW2=0;
            ROW3=1;
            ROW4=0;
            
            if(COL3==1){
                ROW3=0;
                INTCON3bits.INT2IF=0;
                return '9';
            }
            __delay_ms(10);
            ROW1=0;
            ROW2=0;
            ROW3=0;
            ROW4=1;
            
            if(COL3==1){
                ROW4=0;
                INTCON3bits.INT2IF=0;
                return '#';
            }
            __delay_ms(10);
            ROW1=0;
            ROW2=0;
            ROW3=0;
            ROW4=0;
         
    }
     
}

void Delay_LCD () // tempo ~ 1ms 
{
	int i;
	for (i=0;i<400;i++){}
}
void Envia_Comando_Lcd (char comando) // Envia_Comando_Lcd (0x38)
{
	E=0; // LCD desabilitado
	RS=0; //seleciona o envio de instrução

	E=1;
	Porta_Lcd=comando; // Envia a metade alta (D4-D7)
	E=0; // Habilita o LCD a receber informação

	E=1; // Prepara para transferir dados
	comando = comando<<4;
	Porta_Lcd = comando;
	E=0; // Habilita o lcd a receber a informacao

	Delay_LCD(); // Da o delay sugerido pelo fabricante
}
void Envia_Dado_Lcd (char dado)
{
	E=0;
	RS=1; // Seleciona o registrador de dados(LCD)
	
	E=1;
	Porta_Lcd=dado;
	E=0;
	
	E=1;
	dado=dado<<4;
	Porta_Lcd=dado;
	E=0;

	Delay_LCD();
}
void Inicializa_Lcd (void)
{
	Delay_inicializacao_LCD ();
	Envia_Comando_Lcd (0b000000010); // Lcd 4 bits
	Envia_Comando_Lcd (0b001001000); // 
	Envia_Comando_Lcd (0b000001110); // 
	Envia_Comando_Lcd (0b000000001); // 
    
    //Envia_Dado_Lcd ('A');
    //while(1);
}

void Print_Mensagem_Lcd (char *mensagem)
{
	int i=0;
	while (mensagem[i]) {
		Envia_Dado_Lcd (mensagem[i]);
		i++;
	}
}

void Delay_inicializacao_LCD () // tempo ~ 1ms
{
	int i;
	for (i=0;i<4000;i++) {}
}
