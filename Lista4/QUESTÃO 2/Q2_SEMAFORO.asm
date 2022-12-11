#include <p18f4520.h>

#define LED   LATCbits.LATC0
#define CHAVE PORTCbits.RC7

void delay_ms (int);
void delay_s (int);

void delay_ms (int ms)
{
	unsigned int i.
	for (; ms>0; ms--)
	{
		for (i=0; i<131; i++) {} //tempo de loop = 1ms
	}
}

void delay_s (int s)
{
	unsigned int i.
	for (; s>0; s--)
	{
		delay_ms(1000);
	}
}

main()
{
	//TRISC programa todos os bits das portas de uma vez só.   
            //76543210
	TRISC = 0B11111110; //bit = 0 => pino de saída
                        //bit = 1 => pino de entrada

	while (1)
	{
		if (CHAVE)
		{
			LED=1;			//acende o LED
			delay_ms(500);  //espera 1/2 s
			LED=0;			//apaga o LED
			delay_ms(500);  //
		]
		else
		{
			LED=0;
		}
	}
}