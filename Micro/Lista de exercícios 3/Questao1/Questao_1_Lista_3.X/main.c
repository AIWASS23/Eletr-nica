#include <xc.h>
#include <stdio.h> 
#include <stdlib.h>

#pragma config PBADEN = OFF

#define _XTAL_FREQ 4000000

#define Porta_Lcd PORTD
#define RS PORTCbits.RC0
#define E PORTCbits.RC1

#define NUMBER_OF_PULSES 10//pulsos por volta da roda
#define SIZE_OF_WHEEL 2.5//circuferencia da roda em metros

void Envia_Comando_Lcd (char comando);
void Envia_Dado_Lcd (char dado);
void Inicializa_Lcd (void);
void Print_Mensagem_Lcd (char *mensagem);
void Delay_inicializacao_LCD();
void init_interruption();
void init_timer();
float calculate_speed(int count);
void interrupt ISR();

int Timer0_count=0;
int RPM_count=0;//RPM do motor,no INT0
int SPEED_count=0;//Velocidade da roda,no INT1

int RPM_output;
float speed;//velocidade em km/h
double mileage=0;//quilometragem em metros

void interrupt ISR(){
    if(INTCONbits.INT0IF==1){       
        INTCONbits.INT0IF=0;
        RPM_count++;
        
    }
    
    if(INTCON3bits.INT1IF==1){
        INTCON3bits.INT1IF=0;
        SPEED_count++;
    }
    
    if(INTCONbits.TMR0IF==1){
        Timer0_count++;
        INTCONbits.TMR0IF=0;
        TMR0L=0;
    }
    
    if(Timer0_count>3333){
        Timer0_count=0;//reseta TIMER0
        
        RPM_output=RPM_count*60;
        speed=calculate_speed(SPEED_count);
        mileage=mileage+speed/3.6;
        
        RPM_count=0;
        SPEED_count=0;
        
       
   
    }
    
    
}

void main(void) {
    TRISC=0b11111100; // Pinos de controle do LCD como saida
	TRISD=0b00000000; // Configura a porta do lcd
    TRISB=0b01111111;// Configura interrupcaoes como entrada
    
    init_interruption();
    init_timer();
    
	Inicializa_Lcd ();
    //Print_Mensagem_Lcd ("ola");
    while(1){
        char buf[16];
        sprintf(buf, "RPM: %d\n", RPM_output);
        
        
        Envia_Comando_Lcd(1);
        Print_Mensagem_Lcd(buf);
        __delay_ms(1000);
        
        Envia_Comando_Lcd(1);
        sprintf(buf, "Speed: %.1f Km/h\n", speed);
        Print_Mensagem_Lcd(buf);
        
        __delay_ms(1000);
        
         Envia_Comando_Lcd(1);
        sprintf(buf, "Mileage: %.1f m\n", mileage);
        Print_Mensagem_Lcd(buf);
        
        __delay_ms(1000);
        
        
    }
    return;
}
float calculate_speed(int count){
    float real_speed=((count*SIZE_OF_WHEEL)/NUMBER_OF_PULSES)*3.6;
    return real_speed;
}

void init_interruption(){
    INTCON2bits.RBPU=0;//habilita resistores de pullup
    
    INTCONbits.INT0IF=0;//limpa flag da int0
    INTCONbits.INT0IE=1;//habilita int0
    
    INTCON3bits.INT1IF=0;//limpa flag da int1
    INTCON3bits.INT1IE=1;//habilita int1
    
    INTCON2bits.INTEDG0=0;//configura int0 para ativar na descida
    INTCON2bits.INTEDG1=0;//configura int1 para ativar na descida
    
    INTCONbits.GIE=1;//habilita interrupcoes
    
    
}

void init_timer(){
    INTCONbits.PEIE=1;
    INTCONbits.TMR0IE=1;//habilita TIMER0
    INTCONbits.TMR0IF=0;//limpa flag do TIMER0
    T0CONbits.TMR0ON=1;//habilita TIMER0
    T0CONbits.T08BIT=1;//configura TIMER0 com 8bits
    T0CONbits.T0CS=0;//seleciona fonte de clock do TIMER0
    
    TMR0L=0;//zera contador do TIMER0
    
    INTCONbits.GIE=1;//habilita interrupcoes
}

void Delay_LCD () // tempo ~ 1ms 
{
	int i;
	for (i=0;i<400;i++){}
}

void Envia_Comando_Lcd (char comando) // Envia_Comando_Lcd (0x38)
{
	E=0; // LCD desabilitado
	RS=0; //seleciona o envio de instrução

	E=1;
	Porta_Lcd=comando; // Envia a metade alta (D4-D7)
	E=0; // Habilita o LCD a receber informação

	E=1; // Prepara para transferir dados
	comando = comando<<4;
	Porta_Lcd = comando;
	E=0; // Habilita o lcd a receber a informacao

	Delay_LCD(); // Da o delay sugerido pelo fabricante
}

void Envia_Dado_Lcd (char dado)
{
	E=0;
	RS=1; // Seleciona o registrador de dados(LCD)
	
	E=1;
	Porta_Lcd=dado;
	E=0;
	
	E=1;
	dado=dado<<4;
	Porta_Lcd=dado;
	E=0;

	Delay_LCD();
}

void Inicializa_Lcd (void)
{
	Delay_inicializacao_LCD ();
	Envia_Comando_Lcd (0b000000010); // Lcd 4 bits
	Envia_Comando_Lcd (0b001001000); // 
	Envia_Comando_Lcd (0b000001110); // 
	Envia_Comando_Lcd (0b000000001); // 
    
    //Envia_Dado_Lcd ('A');
    //while(1);
}

void Print_Mensagem_Lcd (char *mensagem)
{
	int i=0;
	while (mensagem[i]) {
		Envia_Dado_Lcd (mensagem[i]);
		i++;
	}
}

void Delay_inicializacao_LCD () // tempo ~ 1ms
{
	int i;
	for (i=0;i<4000;i++) {}
}
