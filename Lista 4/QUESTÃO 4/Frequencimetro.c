#include <p18f4520.h>

// Marcelo De Araújo

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
						 //Não confiável para um relógio
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

// Função que calcula quantos intervalos de 100 us se passaram desde o início da onda
void calcular_periodo(void)
{
	p = 0;

	// A onda começará a ser contada no momento que ela subir de 0 para 1
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

// Função que calcula a frequência dado por 1/período. Sendo o período = p * 10^2 * 10^-6
void calcular_frequencia(void)
{
	// Devido a uma divisão inteira em C resultar sempre no ground do número resultante, foi-se necessário fazer uma divisão 
	// de float e somar 0,5 para só em seguida transformar para inteiro, para se conseguir a divisão com a aproximação correta.
	f = (10000.0/p) + 0.5 ;
}

// Função que mostra a frequência no display
void mostrar_frequencia(void)
{
	// Se a frequência for maior que 99 Hz, o display mostrará "EE", sinalizando uma leitura inválida
	if(f > 99)
	{
		LATC = 0B11101110;
	}
	else // Se não, a frequência medida é mostrada no display
	{
		LATC = (f/10 << 4) | (f%10);
	}
}