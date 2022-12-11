/* Marcelo De Ara�jo */

#include <p18f4520.h>
#include <string.h>

//RC0 transmissor e RC1 receptor
#define TX LATCbits.LATC0
#define RX LATCbits.LATC1

int potenciacao_2(int potencia)
{
	int i,resultado = 1;
	for(i=0; i<potencia; i++)
	{	
		resultado *= 2;
	}
	return resultado;
}

// 817,5 us
void delay_1200bps(void)
{
	int i;
	for(i = 0; i < 89; i++){}
}

// 709,5 us
void delay_1200bps2(void)
{
	int i;
	for(i = 0; i < 77; i++){}
}

// 772,5 us
void delay_1200bps3(void)
{
	int i;
	for(i = 0; i < 84; i++){}
}

// 416,5 us
void delay_600bps(void)
{
	int i;
	for(i = 0; i < 44; i++){}
	Nop();Nop();Nop();Nop();Nop();Nop();Nop();Nop();
}

void escrever(char letra)
{
	int i;

	TX=0; 					// Come�a a escrita colocando TX para 0
	delay_1200bps();		// passa um tempo para come�ar a escrita do byte

	for(i = 0; i<8; i++) 	// escreve cada um dos 8 bits do caractere e passa um tempo para a escrita do pr�ximo bit
	{
		TX = letra % 2;
		letra = letra/2;
		delay_1200bps2();
	}
	TX = 1; 				// Se encerra a escrita colocando TX para 1
    delay_1200bps(); 		// passa um tempo antes de come�ar a pr�xima escrita 
}

char ler(void)
{
	char resultado = 0;		 // vari�vel onde ser� armazenado o valor do caractere
	int j;

	while(RX){} 			// enquanto RX estiver em 1, a transmiss�o do caractere n�o come�ou
	
	delay_600bps();			// Ap�s RX cair para zero, ser� dado um delay de 600bps

	if(!RX) 				// Se o RX continuar em 0 significa que a queda n�o foi um ru�do e a leitura ser� realizada
	{
		delay_1200bps();	// Delay para a inicializa��o da leitura

		for(j = 0; j<8; j++)// ser� analisado se cada um dos bits lidos s�o 1 ou 0
		{
			if(!RX){} 		// Se o bit for 0, ele n�o contribuir� para o resultado
			else 
			{
				resultado += potenciacao_2(j); // Se ele for 1, ent�o ser� somada a respectiva pot�ncia de 2 relacianada a posi��o desse bit ao resultado
			}
			delay_1200bps3(); 				   // Delay para a leitura do pr�ximo bit
		}
		delay_1200bps(); 					   // Delay para a finaliza��o da leitura
	}
	return resultado;
}

main()
{
	TRISCbits.RC0 = 0;
	TRISCbits.RC1 = 1;
	TX = 1;
	delay_1200bps();

	while(1)
	{
		escrever(ler());
	}
}