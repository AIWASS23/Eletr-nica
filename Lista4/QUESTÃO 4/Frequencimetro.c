#include <p18f4520.h>

// Marcelo De Ara�jo

//100hz*100=10000hz
//1/10000=0,0001s p/ um delay
//Precisa ser 100x maior pra chegar em 1%

#pragma config OSC = INTIO67 //RA6 e RA7 para I/O

#define sinal PORTAbits.RA7

int p, f;

void delay_100us(void);
void calcular_periodo(void);
void calcular_frequencia(void);
void mostrar_frequencia(void);

main()
{
	OSCCON = 0B01110010; //Setup do oscilador - 8MHz
						 //N�o confi�vel para um rel�gio
	TRISC = 0B00000000;  //PORTC como saida
	TRISAbits.RA7 = 1;   //RA7 como entrada para o sinal
	ADCON1 = 0B00001111; //Configura todas as portas como I/O

	while(1)
	{
		calcular_periodo();
		calcular_frequencia();
		mostrar_frequencia();
	}
}

void delay_100us(void)
{
	int i;
	for(i=0; i<8; i++) 
	{
		Nop();
	};
		Nop();
		Nop();
		Nop();
		Nop();
		Nop();
		Nop();
		Nop();
}

// Fun��o que calcula quantos intervalos de 100 us se passaram desde o in�cio da onda
void calcular_periodo(void)
{
	p = 0;

	// A onda come�ar� a ser contada no momento que ela subir de 0 para 1
	while(sinal){}  //espera subir
	while(!sinal){} //espera descer

	// Calculando quantos intervalos de 100 us a onda fica em 1
	while(sinal)
	{
		delay_100us();
		p++;
	}
	// Calculando quantos intervalos de 100 us a onda fica em 0
	while(!sinal)
	{
		delay_100us();
		p++;
	}
}

// Fun��o que calcula a frequ�ncia dado por 1/per�odo. Sendo o per�odo = p * 10^2 * 10^-6
void calcular_frequencia(void)
{
	// Devido a uma divis�o inteira em C resultar sempre no ground do n�mero resultante, foi-se necess�rio fazer uma divis�o 
	// de float e somar 0,5 para s� em seguida transformar para inteiro, para se conseguir a divis�o com a aproxima��o correta.
	f = (10000.0/p) + 0.5 ;
}

// Fun��o que mostra a frequ�ncia no display
void mostrar_frequencia(void)
{
	// Se a frequ�ncia for maior que 99 Hz, o display mostrar� "EE", sinalizando uma leitura inv�lida
	if(f > 99)
	{
		LATC = 0B11101110;
	}
	else // Se n�o, a frequ�ncia medida � mostrada no display
	{
		LATC = (f/10 << 4) | (f%10);
	}
}