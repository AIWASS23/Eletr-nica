#include <p18f4520.h>

#pragma config	OSC = INTIO67
#pragma config	WDT = OFF
#pragma config	MCLRE = ON
#pragma config	PBADEN = OFF
#pragma config	BORV = 3

#define TX 		LATCbits.LATC0
#define RX 		PORTCbits.RC1


void delay_ms (int);
void delay_s (int);
void delay_us (int);
void enviar (char);
char receber(void);


void delay_s (int s)
{
	unsigned long int i;
	for ( ; s > 0; s--)
	{
		for (i=0;i<83333;i++) {};
	}
}


void delay_ms(int ms)

{
	unsigned int i;
	for ( ;ms > 0; ms--)
	{
		for (i=0;i<80;i++) {}		//tempo de loop = 1ms
	}
}

void delay_us(int us)	//funciona para multiplos de 100us
{
	for( ; us > 0; us--){
		unsigned char i;
		for (i=0;i<29;i++) {};
		i=0;
	}
}


void enviar(char c){

	unsigned int i;
	TX = 1; //repouso
	delay_us(8);
	TX = 0; //inicio
	delay_us(8);

	for(i = 0; i < 8; i++){
		TX = c >> i;
		delay_us(8);
	}
	TX = 1; //stop
}

char receber(void){
	
	unsigned int i;
	char c;
	char c_retorno = 0; 
	while(RX) {};	//espera start
	delay_us(4);	//desloca meio bit
	delay_us(8);	//despreza o start
	
	for(i = 0; i < 8; i++){
		c = RX;
		c_retorno = c_retorno|c<<i;
		delay_us(8);
	}
	
	return c_retorno;	
}


void main ()

{	
	//área de configuração (só roda uma vez)
	ADCON1 = 0B00001111; // Seta portas 0, 1, 2 e 3 como analogicas

	OSCCONbits.IRCF2 = 1;
	OSCCONbits.IRCF1 = 1;
	OSCCONbits.IRCF0 = 1;	//OSC = 8MHz

	//BIT=1 -> PORTA DE ENTRADA, BIT=0 -> PORTA DE SAÍDA
	TRISC=0B00000010;	//Porta RC0 como saida e RC1 como entrada

	while(1){
		
	    enviar(receber());
		
	    
	}

}
