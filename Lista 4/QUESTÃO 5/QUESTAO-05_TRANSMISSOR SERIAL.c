/* Marcelo De Araújo */

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

	TX=0; 					// Começa a escrita colocando TX para 0
	delay_1200bps();		// passa um tempo para começar a escrita do byte

	for(i = 0; i<8; i++) 	// escreve cada um dos 8 bits do caractere e passa um tempo para a escrita do próximo bit
	{
		TX = letra % 2;
		letra = letra/2;
		delay_1200bps2();
	}
	TX = 1; 				// Se encerra a escrita colocando TX para 1
    delay_1200bps(); 		// passa um tempo antes de começar a próxima escrita 
}

char ler(void)
{
	char resultado = 0;		 // variável onde será armazenado o valor do caractere
	int j;

	while(RX){} 			// enquanto RX estiver em 1, a transmissão do caractere não começou
	
	delay_600bps();			// Após RX cair para zero, será dado um delay de 600bps

	if(!RX) 				// Se o RX continuar em 0 significa que a queda não foi um ruído e a leitura será realizada
	{
		delay_1200bps();	// Delay para a inicialização da leitura

		for(j = 0; j<8; j++)// será analisado se cada um dos bits lidos são 1 ou 0
		{
			if(!RX){} 		// Se o bit for 0, ele não contribuirá para o resultado
			else 
			{
				resultado += potenciacao_2(j); // Se ele for 1, então será somada a respectiva potência de 2 relacianada a posição desse bit ao resultado
			}
			delay_1200bps3(); 				   // Delay para a leitura do próximo bit
		}
		delay_1200bps(); 					   // Delay para a finalização da leitura
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