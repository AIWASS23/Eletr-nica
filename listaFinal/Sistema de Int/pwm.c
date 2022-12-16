#include <p18f4520.h>

void delay_ms(int ms)

{
	unsigned int i;
	for (;ms>0;ms--)
	{
	for (i=0;i<131;i++) {}		//tempo de loop = 1ms
	}
}

void delay_s(int s)
{
	int i;
	for (;s>0;s--)
	{
		delay_ms(1001);		//delay de 1 seg]
		for (i=0;i<60;i++) {}	
	}
}

void main()
{
	unsigned char periodo=20;
	unsigned char inc_dec=1;
	
	// configurando o PWM 1
	
	
	//SAIDA PWM
	TRISCbits.RC2 = 0;			//pino do PWM 1
			
	//TIMER2
	
	T2CONbits.T2CKPS1 = 1;		//prescaler do TMR2 de 1:16  (8us a 8MHz)
	T2CONbits.T2CKPS0 = 1;
	T2CONbits.TMR2ON = 1;		//liga TMR2
	
	//MODULO PWM

	PR2 = periodo;			//Período do pwm  8us x PR2 = 1Khz
	CCPR1L = periodo/2;		//Duty cicle do PWM (min)
	
	CCP1CONbits.CCP1M3 = 1;		//modo PWM 1
	CCP1CONbits.CCP1M2 = 1;			
	CCP1CONbits.CCP1M1 = 0;			
	CCP1CONbits.CCP1M0 = 0;	
	
		
	while(1)
	{	
		PR2=periodo;			//periodo do PWM
		CCPR1L = periodo/10;		//duty cycle do PWM	
		delay_ms(2);
		
		if (inc_dec==1)
		{
			periodo=periodo+1;
		}
		else
		{
			periodo=periodo-1;
		}
		
		if (periodo>240)
		{
			inc_dec=0;
		}
		if (periodo<30)
		{
			inc_dec=1;
		}
		
	}
}

