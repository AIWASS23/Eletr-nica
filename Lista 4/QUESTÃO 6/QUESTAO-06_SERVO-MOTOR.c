/* Marcelo De Ara�jo */

#include <p18f4520.h>

//Definindo as portas para a chave seletora
#define PORTA1 PORTBbits.RB0
#define PORTA2 PORTBbits.RB1
#define PORTA3 PORTBbits.RB2
#define PORTA4 PORTBbits.RB3
#define PORTA5 PORTBbits.RB4
#define SERVO  PORTDbits.RD0

// 495 us
void delay_500us(void)
{
	int i;
	for(i=0; i< 53; i++){}
	Nop();Nop();Nop();
}

// 50,5 us
void delay_55us(void)
{
	int i,j;
	for(i=0; i< 3; i++){Nop();Nop();Nop();}
	Nop();Nop();Nop();
}

// 49 us
void delay_55us2(void)
{
	int i,j;
	for(i=0; i< 3; i++){Nop();Nop();Nop();}
}

// Fun��o que constr�i a onda que ser� recebida pelo servo para mostrar o �ngulo necess�rio
void servo_angulo(int angulo)
{
	int k = 0;
	
	// Saida em 1 e intervalo = 500 us
	SERVO = 1;
	delay_500us();

	// O servo motor deve ter um per�odo de 20ms, dividindo-se esse per�odo em intervalos
	// Nos primeiros 500 us a onda estar� sempre em 1, s�o 1755 intervalos de aproximadamente 11,1us
	// Mostrar apenas �ngulos m�ltiplos de 5 em intervalos extendidos para 55,5 us, aproximadamente.

	while(k != angulo)
	{
		k += 5;
		delay_55us();
	}

	SERVO = 0;

	// Deixando a fun��o em 0 at� se alcan�ar o per�odo de 20 ms
	while(k != 1755)
	{
		k += 5;
		delay_55us2();

	}
}

// Fun��o que mostrar� 5 �ngulos diferentes com base em qual entrada estar� enviando 1
void mostra_angulo(void)
{
	if(PORTA1) //10 graus
	{
		servo_angulo(10);
	}
	if(PORTA2) //55 graus
	{
		servo_angulo(55);
	}
	if(PORTA3) //90 graus
	{
		servo_angulo(90);
	}
	if(PORTA4) //135 graus
	{
		servo_angulo(135);
	}
	if(PORTA5) //180 graus
	{
		servo_angulo(180);
	}
}

main()
{
	TRISB = 0B11111111; 
	ADCON1= 0B11111111;
	TRISDbits.RD0 = 0;

	while(1)
	{
		mostra_angulo();
	}
}