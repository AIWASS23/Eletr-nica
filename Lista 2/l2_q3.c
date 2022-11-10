#include <p18f4520.h>

#pragma config OSC = INTIO67
#pragma config WDT = OFF
#pragma config BORV = 3
#pragma config MCLRE = ON
#pragma config PBADEN = OFF

#define SINAL PORTAbits.RA0
#define OUT_DEZENA LATC
#define OUT_UNIDADE LATD

int contador_sinal = 0;
char frequencia_sinal = 0;

char decimal_to_7seg[11] = {
	0b00111111,
	0b00000110,
	0b01011011,
	0b01001111,
	0b01100110,
	0b01101101,
	0b01111101,
	0b00000111,
	0b01111111,
	0b01101111,
	0b01111001
};

void config(void) 
{
	OSCCON |= 0b01110000;
	TRISAbits.RA0 = 1;
	TRISC = TRISD = 0;
}

void delay_quase_100us()
{
	char i;
	for(i=0;i<28;i++);
}

void medir_sinal(void)
{
	contador_sinal = 0;
	while(SINAL) {}
	while(~SINAL) {}
	
	while(SINAL) {
		delay_quase_100us();
		contador_sinal++;
	}
	
	contador_sinal *= 2;
	
}

void calcular_frequencia(void)
{
	frequencia_sinal = 1 / (contador_sinal * 100E-6);
}

void mostrar_frequencia(void)
{
	if (frequencia_sinal < 10 \
	    || frequencia_sinal > 99)
	{
		// Mostrar erro
		OUT_DEZENA = decimal_to_7seg[10];
		OUT_UNIDADE = decimal_to_7seg[10];
	}
	else
	{
		OUT_DEZENA = decimal_to_7seg[frequencia_sinal / 10];
		OUT_UNIDADE = decimal_to_7seg[frequencia_sinal % 10];
	}
}

void main(void)
{
	config();
	while(1) 
	{
		medir_sinal();
		calcular_frequencia();
		mostrar_frequencia();
	}
}