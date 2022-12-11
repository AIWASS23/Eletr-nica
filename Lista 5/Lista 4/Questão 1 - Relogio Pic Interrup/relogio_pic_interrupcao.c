#include <p18f4520.h>

#pragma config WDT = OFF // Configura o PIC para não reiniciar depois de um tempo

int seg=0,min=0,hora=0,ctrl=0,ajuste=0;

char num[10] =          // digitos para o display
{
    0b10111111,
    0b10000110,
    0b11011011,
    0b11001111,
    0b11100110,
    0b11101101,
    0b11111101,
    0b10000111,
    0b11111111,
    0b11101111
};

char display[7] =      // habilita cada um dos displays ou os desliga
{
    0b11111111,
    0b11011111,
    0b11101111,
    0b11110111,
    0b11111011,
    0b11111101,
    0b11111110
}; 
 
void delay_miliseg();
void configura_pic();
void registradores();
void mostra_relogio();
void atualiza_relogio();
void interrupcao_alta_priori();


void delay_miliseg(unsigned long int miliseg){
    unsigned long int i;
    for(miliseg; miliseg >0; miliseg--){
    	for(i=0; i<79; i++) {};
    }
}

//**************************     Relogio     *********************************

void mostra_relogio(int ctrl){
    
	if(ctrl == 0){	
        // mostra display us
		PORTD = display[0];
		PORTC = num[seg%10]; // Depois se é mostrado a unidade dos segundos
		PORTD = display[1]; // E por último se é abilitado apenas o display da unidade dos segundos
		delay_miliseg(1);
	}
	
	if(ctrl == 1){	
        // mostra display ds
		PORTD = display[0];
		PORTC = num[(seg/10) %10]; // Depois se é mostrado a dezena dos segundos
		PORTD = display[2]; // E por último se é abilitado apenas o display da dezena dos segundos
		delay_miliseg(1);
	}

	if(ctrl == 2){
        // mostra display um
		PORTD = display[0];
		PORTC = num[min%10]; // Depois se é mostrado a unidade dos minutos
		PORTD = display[3]; // E por último se é abilitado apenas o display da unidade dos minutos
		delay_miliseg(1);
	}

	if(ctrl == 3){		
        // mostra display dm
		PORTD = display[0];
		PORTC = num[(min/10) %10]; // Depois se é mostrado a dezena dos minutos
		PORTD = display[4]; // E por último se é abilitado apenas o display da dezena dos minutos
		delay_miliseg(1);
	}
	
	if(ctrl == 4){
        // mostra display uh
		PORTD = display[0];
		PORTC = num[hora%10]; // Depois se é mostrado a unidade das horas
		PORTD = display[5]; // E por último se é abilitado apenas o display da unidade das horas
		delay_miliseg(1);
	}
	
	if(ctrl == 5){
        // mostra display dh
		PORTD = display[0];
		PORTC = num[(hora/10) %10]; // Depois se é mostrado a dezena das horas
		PORTD = display[6]; // E por último se é abilitado apenas o display da dezena das horas
		delay_miliseg(1);
	}		
}

void atualiza_relogio(void){
	seg++;

	if(seg == 60){
		seg = 0;
		min++;
	
		if(min == 60){
            min = 0;
            hora++;

            if(hora == 24){
                hora = 0;
            }
		}
	}
}

//**************************     PIC    *********************************

void configura_pic(void){
    
    TRISC = 0b10000000; // Todos de PORTC como saida, menos RC7, que mostrará o ponto quando o relógio entrar em modo de ajuste
    TRISD = 0b00000000; // PORTD como saida   
    
    TRISBbits.RB0 = 1; // Configura RB0 como entrada
    TRISBbits.RB1 = 1; // Configura RB1 como entrada
    TRISBbits.RB2 = 1; // Configura RB2 como entrada
    
    ADCON1 = 0x0F; // Configurando todas as portas do PIC para digital
}

void registradores(void){
    
    RCONbits.IPEN = 1;          // Habilita níveis de priori
    INTCONbits.GIE = 1;         // Habilita interrupções de alta priori
    INTCONbits.PEIE = 1;        // Habilita interrupções de baixa priori
    
// Habilita interrupções externas
    INTCONbits.INT0IE = 1;
    INTCON3bits.INT1IE = 1;
    INTCON3bits.INT2IE = 1;
    
// Configura interrupções INT como alta priori
    INTCON3bits.INT1IP = 1;    
    INTCON3bits.INT2IP = 1;
    
// Interrupções na subida
    INTCON2bits.INTEDG0 = 1;  
    INTCON2bits.INTEDG1 = 1;  
    INTCON2bits.INTEDG2 = 1;   
    
// Timer 0 (configurações)
    T0CONbits.TMR0ON = 1;   // Ligando timer 0
    T0CONbits.T08BIT = 0;   // Timer 0 configurado para 16 bits
    T0CONbits.T0CS = 0;     // Fonte do timer 0 é o clock interno do PIC
    T0CONbits.PSA = 0;      // Habilitando Prescaler
    T0CONbits.T0PS2 = 1;
    T0CONbits.T0PS1 = 1;    // 1:128 Prescaler
    T0CONbits.T0PS0 = 0;
    
    // Conta 1 segundo
    TMR0H = 0xC2; 
    TMR0L = 0xF7; 

// Timer 0 (Habilitando)

    INTCONbits.TMR0IE = 1;  // Habilitando as interrupções vindas do Timer 0
    INTCON2bits.TMR0IP = 1; // Interrupções vindas de Timer 0 como alta priori

// Timer 1 (configurações)

    T1CONbits.TMR1ON = 1;   // Ligando timer 1
    T1CONbits.T1RUN = 0;    // Clock defonte diferente do oscilador do timer 1
    T1CONbits.T1OSCEN = 0;  // Oscilador do timer 1 desligado
    T1CONbits.RD16 = 1;     // Timer 1 configurado para 16 bits
    T1CONbits.TMR1CS = 0;   // Fonte do timer 1 é o clock interno
    
    // Conta 5 ms
    TMR1H = 0xD8;           
    TMR1L = 0xF0;           

// Timer 1 (Habilitando)

    PIE1bits.TMR1IE = 1;     // Habilitando as interrupções vindas do Timer 1
    IPR1bits.TMR1IP = 1;     // Setando as interrupções vindas de Timer 1 como alta prioridade
    
// Timer 3 (configurações)

    T3CONbits.TMR3ON = 1;    // Ligando timer 3
    T3CONbits.T3CKPS1 = 1;  
    T3CONbits.T3CKPS0 = 1;   //  1:8 Prescaler
    T3CONbits.RD16 = 1;      // Timer 3 configurado para 16 bits
    T3CONbits.TMR3CS = 0;    // Fonte do timer 3 é o clock interno
    
    // Conta 200 ms
    TMR3H = 0x3C;            
    TMR3L = 0xB0;            
// Parte alta junto com a parte baixa formam o número donde o timer começa a contar

// Timer 3 (Habilitando)

    PIE2bits.TMR3IE = 1; // Habilitando as interrupções vindas do Timer 3
    IPR2bits.TMR3IP = 1; // Setando as interrupções vindas de Timer 3 como alta prioridade
    
}

#pragma code alta_prioridade = 0x08

void interrupcao_de_alta_prioridade(void)
{
  _asm goto interrupcao_alta_priori _endasm
}

//*************************     Interrupção     *******************************

#pragma code

#pragma interrupt interrupcao_alta_priori 

// Define qual prioridade acontece
void interrupcao_alta_priori(void){
     
    if(INTCONbits.TMR0IF){      // Timer0 de 1 seg, atualiza o relogio
        
        TMR0H = 0xC2; 
        TMR0L = 0xF7; 
        atualiza_relogio();
        INTCONbits.TMR0IF = 0; 
    }
        
    if(INTCONbits.INT0IF){      // Se retorna 1, então botão ajuste pressionado

        INTCON2bits.INTEDG0 = ~INTCON2bits.INTEDG0;     // Interrupção na borda oposta -> novo pressionar ser detectado
        INTCONbits.INT0IF = 0;
        TMR3H = 0x3C; 
        TMR3L = 0xB0; 
        ajuste = 0; // Conta 3 segundos do ajuste
    }

    if(INTCON3bits.INT1IF){     // Se retorna 1, então o botão hora pressionado

        if(TRISCbits.RC7 == 0){ // Se RC7 0, então o ajuste está ligado
        
            TMR3H = 0x3C; 
            TMR3L = 0xB0; 
            ajuste = 0;
            hora++;
            
            if(hora == 24){
            	hora = 0;
            }
        }
    
        INTCON3bits.INT1IF = 0;
    }

    if(INTCON3bits.INT2IF){     // Se retorna 1, então o botão minuto pressionado
        
        if(TRISCbits.RC7 == 0){
            TMR3H = 0x3C; 
            TMR3L = 0xB0; 
            ajuste = 0; 
            min++;
            
            if(min == 60){
                min = 0;
                hora++;
               	if(hora == 24){
                	hora = 0;
                }
            }
        }
        
        INTCON3bits.INT2IF = 0;
    }
   
    if(PIR1bits.TMR1IF){        // Timer1 de 5 ms, mostra algum digito no display
        
        TMR1H = 0xD8; 
        TMR1L = 0xF0;
        mostra_relogio(ctrl);
        ctrl++;                 // Controla qual digito aparece
        
        if(ctrl == 6){
        	ctrl = 0;           // Mostrou-se o último dígito do display
        	}
        PIR1bits.TMR1IF = 0; 
    }
    
    if(PIR2bits.TMR3IF){        // Timer que conta 200 ms 
        
        TMR3H = 0x3C; 
        TMR3L = 0xB0; 
        ajuste++;
        
        if( ajuste == 15){      // Tempo de 3 segundos
            
            if(PORTBbits.RB0 == 1){     // Ainda em ajuste 
    					
    			TRISCbits.RC7 = 0;
    			ajuste = 0; 
    		}
            
            else{                      // Ajuste ainda não pressionado, 3s, fim de ajuste
    			TRISCbits.RC7 = 1;
    			ajuste = 0;
    		}
    	}
        
        PIR2bits.TMR3IF = 0;
    }    
}
    
main(){    
    configura_pic();
    registradores();
    while(1){
    }
}
