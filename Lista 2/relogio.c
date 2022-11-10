#include <p18f4520.h>

#pragma config	OSC = INTIO67
#pragma config	WDT = OFF
#pragma config	MCLRE = ON
#pragma config	PBADEN = OFF
#pragma config	BORV = 3

#define cont_ms 100
int contagem_minutos = 0;
int contagem_segundos = 0;
int contagem_dezenas_minutos = 0;
int contagem_horas = 0;
int contagem_dezenas_horas = 0;

int alarme_contagem_minutos = 9;
int alarme_contagem_dezenas_minutos = 9;
int alarme_contagem_horas = 9;
int alarme_contagem_dezenas_horas = 9;

int aux_contagem_minutos = 0;
int aux_contagem_dezenas_minutos = 0;
int aux_contagem_horas = 0;
int aux_contagem_dezenas_horas = 0;

int flag_alarme = 0;



int modo = 0;
int porta_c7 = 0;

void delay_s (int segundos)
{
	unsigned long int i;
	for (;segundos>0;segundos--)
	{
		for (i=0;i<83333;i++) {};
	}
}

void delay_ms (int ms)
{
	unsigned long int i;
	for (;ms>0;ms--)
	{
		for (i=0;i<80;i++) {};
	}
}

void verifica_segundos(){
	if(contagem_segundos == 120){
		contagem_minutos++;
		contagem_segundos = 0;
		flag_alarme == 0;
	}
	
}

void mostrar_unidade_minutos (int a, int b, int c, int d, int e, int f, int g){
	LATDbits.LATD0 = a;
	LATDbits.LATD1 = b;
	LATDbits.LATD2 = c;
	LATDbits.LATD3 = d;
	LATDbits.LATD4 = e;
	LATDbits.LATD5 = f;
	LATDbits.LATD6 = g;
	
}	
void mostrar_dezenas_minutos (int a, int b, int c, int d, int e, int f, int g){
	LATCbits.LATC0 = a;
	LATCbits.LATC1 = b;
	LATCbits.LATC2 = c;
	LATCbits.LATC3 = d;
	LATCbits.LATC4 = e;
	LATCbits.LATC5 = f;
	LATCbits.LATC6 = g;
}	
void mostrar_unidade_horas (int a, int b, int c, int d, int e, int f, int g){
	LATAbits.LATA0 = a;
	LATAbits.LATA1 = b;
	LATAbits.LATA2 = c;
	LATAbits.LATA3 = d;
	LATAbits.LATA7 = e;
	LATAbits.LATA5 = f;
	LATAbits.LATA6 = g;
}	
void mostrar_dezenas_horas (int a, int b, int c, int d, int e, int f, int g){
	LATBbits.LATB0 = a;
	LATBbits.LATB1 = b;
	LATBbits.LATB2 = c;
	LATBbits.LATB3 = d;
	LATBbits.LATB4 = e;
	LATBbits.LATB5 = f;
	LATBbits.LATB6 = g;
}	


void alarme(){
	int contador_quantidades_bips = 0;
	
	if(flag_alarme == 0 && alarme_contagem_minutos == contagem_minutos && alarme_contagem_horas == contagem_horas && alarme_contagem_dezenas_minutos == contagem_dezenas_minutos && alarme_contagem_dezenas_horas == contagem_dezenas_horas){
		while(contador_quantidades_bips < 4){
			LATCbits.LATC7 = 1;
			delay_s(1);
			LATCbits.LATC7 = 0;
			delay_ms(200);
			contador_quantidades_bips++;
		}
		flag_alarme = 1;	
		
	}

}	
void unidade_minutos(){
	switch(contagem_minutos){
	case 0:
		mostrar_unidade_minutos(0,1,1,1,1,1,1);
		break;
	case 1:
		mostrar_unidade_minutos(0,0,0,0,1,1,0);
		break;
	case 2:
		mostrar_unidade_minutos(1,0,1,1,0,1,1);
		break;
	case 3:
		mostrar_unidade_minutos(1,0,0,1,1,1,1);
		break;
	case 4:
		mostrar_unidade_minutos(1,1,0,0,1,1,0);
		break;
	case 5:
		mostrar_unidade_minutos(1,1,0,1,1,0,1);
		break;
	case 6:
		mostrar_unidade_minutos(1,1,1,1,1,0,1);
		break;
	case 7:
		mostrar_unidade_minutos(0,0,0,0,1,1,1);
		break;
	case 8:
		mostrar_unidade_minutos(1,1,1,1,1,1,1);
		break;
	case 9:
		mostrar_unidade_minutos(1,1,0,1,1,1,1);
		break;
	case 10:
		mostrar_unidade_minutos(0,1,1,1,1,1,1);
		contagem_minutos = 0;
		contagem_dezenas_minutos++;
		break;
	};	
}
void dezenas_minutos(){
	switch(contagem_dezenas_minutos){
	case 0:
		mostrar_dezenas_minutos(0,1,1,1,1,1,1);
		break;
	case 1:
		mostrar_dezenas_minutos(0,0,0,0,1,1,0);
		break;
	case 2:
		mostrar_dezenas_minutos(1,0,1,1,0,1,1);
		break;
	case 3:
		mostrar_dezenas_minutos(1,0,0,1,1,1,1);
		break;
	case 4:
		mostrar_dezenas_minutos(1,1,0,0,1,1,0);
		break;
	case 5:
		mostrar_dezenas_minutos(1,1,0,1,1,0,1);
		break;
	case 6:
		mostrar_dezenas_minutos(0,1,1,1,1,1,1);
		contagem_dezenas_minutos = 0;
		contagem_horas++;
		break;
	};	
}
void unidade_horas(){
	if(contagem_dezenas_horas == 2 && contagem_horas == 4){
    	contagem_horas = 0;
    	contagem_dezenas_horas = 0;
    };
	switch(contagem_horas){
	case 0:
		mostrar_unidade_horas(0,1,1,1,1,1,1);
		break;
	case 1:
		mostrar_unidade_horas(0,0,0,0,1,1,0);
		break;
	case 2:
		mostrar_unidade_horas(1,0,1,1,0,1,1);
		break;
	case 3:
		mostrar_unidade_horas(1,0,0,1,1,1,1);
		break;
	case 4:
		mostrar_unidade_horas(1,1,0,0,1,1,0);
		break;
	case 5:
		mostrar_unidade_horas(1,1,0,1,1,0,1);
		break;
    case 6:
		mostrar_unidade_horas(1,1,1,1,1,0,1);
		break;	
    case 7:
		mostrar_unidade_horas(0,0,0,0,1,1,1);
		break;	
    case 8:
		mostrar_unidade_horas(1,1,1,1,1,1,1);
		break;	
    case 9:
		mostrar_unidade_horas(1,1,0,1,1,1,1);
		break;	
    case 10:
		mostrar_unidade_horas(0,1,1,1,1,1,1);
        contagem_horas = 0;
        contagem_dezenas_horas++;
		break;	
	};	
}
void dezenas_horas(){
	switch(contagem_dezenas_horas){
	case 0:
		mostrar_dezenas_horas(0,1,1,1,1,1,1);
		break;
	case 1:
		mostrar_dezenas_horas(0,0,0,0,1,1,0);
		break;
	case 2:
		mostrar_dezenas_horas(1,0,1,1,0,1,1);
		break;
	case 3:
		mostrar_dezenas_horas(1,0,0,1,1,1,1);
		contagem_dezenas_horas = 0;
		break;
	};	
}

void ajustar_relogio(){
	
//	while(1){
//			if(PORTEbits.RE1 == 1){
//				delay_ms(500);
//				contagem_minutos++;
//				unidade_minutos();
//				dezenas_minutos();				
//			}else{
//				break;
//			}
//			
//	}
//	while(1){
//		if(PORTEbits.RE2 == 1){
//				delay_ms(500);
//				contagem_horas++;
//				unidade_horas();
//				dezenas_horas();
//			}else{
//				break;
//			}
//	}
	if(PORTEbits.RE1 == 1){
		while(PORTEbits.RE1 == 1){
		}
		
		contagem_minutos++;
		flag_alarme = 0;
		unidade_minutos();
		dezenas_minutos();	
		
	}
	
	if(PORTEbits.RE2 == 1){
		while(PORTEbits.RE2 == 1){
		}
		
		contagem_horas++;
		flag_alarme = 0;
		unidade_horas();
		dezenas_horas();	
		
	}

	
	}


void modos(){
	
if(PORTEbits.RE0 == 1){
	int contador = 0;
	while(contador < 4){
		delay_ms(500);
		if(PORTEbits.RE0 == 0){
			return;
		}
		contador++;	
		
	}
		
	if(modo == 0){
		modo = 1;
	}else{
		modo = 0;
	}
	
	aux_contagem_minutos = contagem_minutos;
	aux_contagem_dezenas_minutos = contagem_dezenas_minutos;
	aux_contagem_horas = contagem_horas;
	aux_contagem_dezenas_horas = contagem_dezenas_horas;
	mostrar_unidade_minutos(0,1,1,1,1,1,1);
	mostrar_dezenas_minutos(0,1,1,1,1,1,1);
	mostrar_unidade_horas(0,1,1,1,1,1,1);
	mostrar_dezenas_horas(0,1,1,1,1,1,1);
	
	while(modo == 1){
		int flag_aux = 0;
		ajustar_relogio();
		
		if(PORTEbits.RE0 == 1){
			int contador1 = 0;
			while(contador1 < 4){
			delay_ms(500);
			if(PORTEbits.RE0 == 0){
				flag_aux = 1;
			}
			contador1++;	
		
			}
			
			if(flag_aux == 0){
				
				alarme_contagem_minutos = contagem_minutos;
				alarme_contagem_dezenas_minutos = contagem_dezenas_minutos;
				alarme_contagem_horas = contagem_horas;
				alarme_contagem_dezenas_horas = contagem_dezenas_horas;
				
				contagem_minutos = aux_contagem_minutos;
				contagem_dezenas_minutos = aux_contagem_dezenas_minutos;
				contagem_horas = aux_contagem_horas;
				contagem_dezenas_horas = aux_contagem_dezenas_horas;
				return;
				
			}
			
			
		}
		
	}
	
	
}
}


void main ()
{			
	ADCON1 = 0x0F;
	//BIT=1 -> PORTA DE ENTRADA, BIT=0 -> PORTA DE SAÍDA
  //TRISCbits.RC0 = 0;	// CONFIGURA O LED COMO SAIDA
	        //76543210
	TRISA = 0B00000000;	
	TRISB = 0B00000000;	
	TRISC = 0B00000000;		
	TRISD = 0B00000000;
	TRISE = 0B00001111;
	OSCCONbits.IRCF2 = 1;
	OSCCONbits.IRCF1 = 1;
	OSCCONbits.IRCF0 = 1;	//OSC = 8MHz
	
	
	
	while (1)
	{		
		ajustar_relogio();
		unidade_minutos();
		dezenas_minutos();
		unidade_horas();
		dezenas_horas();
		verifica_segundos();
		delay_ms(500);
		contagem_segundos++;
		modos();
		alarme();
	}
}
