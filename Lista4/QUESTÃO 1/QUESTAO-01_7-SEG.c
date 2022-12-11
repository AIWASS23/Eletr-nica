/* Marcelo De Ara�jo */

#include <p18f4520.h>

#define botao_m PORTEbits.RE0 
#define botao_h PORTEbits.RE1

char s=0, m=0, h=0; 			//Contadores
char bcd_s=0, bcd_m=0, bcd_h=0; //N�meros mostrados nos displays

void delay_s (int);
void mostrar_bcd (void);
void contar_relogio(void);
void checar_botao(void);

main()
{
	//TRIS programa todos os bits das portas de uma vez s�.
	//Todas as portas configuradas como sa�da.   
            //76543210
    TRISB = 0B00000000; //bcd horas
	TRISC = 0B00000000;	//bcd minutos
	TRISD = 0B00000000; //bcd segundos
	ADCON1 = 0x0F; 		//Configurando todas as portas do PIC para digital
	
    while(1)
    {
    	mostrar_bcd();
    	delay_s(0.9);		//temporiza o rel�gio
    	contar_relogio();
    	checar_botao();
	}
}

// Fun��es de DELAY: contagem de tempo baseado no clock

void delay_s(int s)
{
	unsigned long int i;
	for(s;s>0;s--)
	{
		for (i=0;i<45450;i++)
		{
			//Verificado se o bot�o foi pressionado e se � mostrado o rel�gio
			//durante a contagem
			checar_botao(); 
			mostrar_bcd();
		}
	}
}

// Fun��o para MOSTRAR os n�meros nos displays
void mostrar_bcd (void)
{
	LATD = bcd_s;
	LATC = bcd_m;
	LATB = bcd_h;
}

void contar_relogio (void)
{
	s++;
	bcd_s++;
	//Verifica se "s" se torna um m�ltiplo de 10 maior que 0 menor que 60,
	//SE SIM, o n�mero mostrado no display deve ser um n�mero 6 unidades 
	//a mais, pois o display � hexadecimal (0,1,2,3,4,5,6,7,8,9,A,B,C,D,E,F)
	if(s>0 && s%10==0 && s<60)
    {
        bcd_s += 6;
    }
    
    if(s==60)
    {
    	s=0;
    	bcd_s = s;
    	m++;
    	bcd_m++;
        if(m>0 && m%10==0 && m<60)
    	{
        	bcd_m += 6;
    	}
    	
    	if(m==60)
    	{
    		m=0;
    		bcd_m = m;
    		h++;
    		bcd_h++;
    		if(h>0 && h%10==0)
    		{
    			bcd_h += 6;
    		}
    		
    		if(h==24)
    		{
    			h=0;
    			bcd_h=h;
    		}
    	}
    	
    }
}

void checar_botao(void)
{
	if(botao_m) // Se RE0=1, bot�o dos minutos pressionado
	{
		while(botao_m){}; // Enquanto o bot�o estiver pressionado, o programa n�o continua para evitar acr�scimos errados no tempo
					  	  // Se o bot�o dos minutos foi pressionado, os segundos s�o resetados e ser� acrescentado 1 minuto ao tempo atual
		s = 0;
		bcd_s = s;
		m++;
		bcd_m++;

		if(m > 0 && m % 10 == 0 && m < 60)
		{
			bcd_m += 6;
		}
	
		if(m == 60)
		{
			m = 0;
			bcd_m = m;
			h++;
			bcd_h++;

			if(h > 0 && h % 10 == 0)
			{
				bcd_h += 6;
			}

			if(h == 24)
			{
				h = 0;
				bcd_h = h;
			}

		}

	}

	if(botao_h) // Se RE1=1, bot�o dos minutos pressionado
	{
		while(botao_h){};	//Se o bot�o das horas foi pressionado, os segundos s�o resetados 
							//e ser� acrescentado 1 hora ao tempo atual
		s = 0;
		bcd_s = s;
		h++;
		bcd_h++;

		if(h > 0 && h % 10 == 0)
		{
			bcd_h += 6;
		}
		if(h == 24)
		{
			h = 0;
			bcd_h = h;
		}

	}

}