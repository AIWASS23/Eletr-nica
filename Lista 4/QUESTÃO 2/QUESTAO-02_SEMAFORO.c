/* Marcelo De Araújo */

#include <p18f4520.h>

#define LED_VERM_S1	LATCbits.LATC0
#define LED_AMAR_S1	LATCbits.LATC1
#define LED_VERD_S1	LATCbits.LATC2
#define LED_VERM_S2	LATCbits.LATC3
#define LED_AMAR_S2	LATCbits.LATC4
#define LED_VERD_S2	LATCbits.LATC5
#define CHAVE		PORTCbits.RC7

void delay_ms (unsigned int);
void delay_s (unsigned int);

void semaforos_c1 (void);
void semaforos_c2 (void);

main()
{
	//TRISC programa todos os bits das portas de uma vez só.   
            //76543210
	TRISC = 0B11000000; //bit = 0 => pino de saída		(LED)
                        //bit = 1 => pino de entrada	(CHAVE)
    while(1)
    {
    	if(CHAVE)
	    {
		    semaforos_c1 ();
	    }
	    else
	    {
	    	semaforos_c2 ();
	    }
	}
}

// Funções de DELAY: contagem de tempo baseado no clock
void delay_ms(unsigned int ms)
{
	unsigned int i;
	for(;ms>0;ms--)
	{
		for (i=0;i<131;i++){} //tempo de loop = 1ms
	}
}

void delay_s(unsigned int s)
{
	unsigned int i;
	for(;s>0;s--)
	{
		delay_ms(1001); //delay de 1s
		for (i=0;i<60;i++){}
	}
}

// Funções dos SEMÁFOROS do ciclo 1 e 2
void semaforos_c1 (void)
{
	LED_VERD_S1=1;
	LED_VERM_S2=1;
	delay_s(12);
	LED_VERD_S1=0;
	LED_VERM_S2=1;
	LED_AMAR_S1=1;
	delay_s(3);
	LED_AMAR_S1=0;
	LED_VERM_S1=1;
	LED_VERM_S2=1;
	delay_s(1);
	LED_VERM_S2=0;
	LED_VERM_S1=1;
	LED_VERD_S2=1;
	delay_s(12);
	LED_VERD_S2=0;
	LED_VERM_S1=1;
	LED_AMAR_S2=1;
	delay_s(3);
	LED_AMAR_S2=0;
	LED_VERM_S1=1;
	LED_VERM_S2=1;
	delay_s(1);
	LED_VERM_S1=0;
}

void semaforos_c2 (void)
{
	LED_VERD_S1=1;
	LED_VERM_S2=1;
	delay_s(8);
	LED_VERD_S1=0;
	LED_VERM_S2=1;
	LED_AMAR_S1=1;
	delay_s(2);
	LED_AMAR_S1=0;
	LED_VERM_S1=1;
	LED_VERM_S2=1;
	delay_s(1);
	LED_VERM_S2=0;
	LED_VERM_S1=1;
	LED_VERD_S2=1;
	delay_s(8);
	LED_VERD_S2=0;
	LED_VERM_S1=1;
	LED_AMAR_S2=1;
	delay_s(2);
	LED_AMAR_S2=0;
	LED_VERM_S1=1;
	LED_VERM_S2=1;
	delay_s(1);
	LED_VERM_S1=0;
}