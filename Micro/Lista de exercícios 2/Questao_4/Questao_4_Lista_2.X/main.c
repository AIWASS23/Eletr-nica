#include <xc.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h> 
//declarar as suas variaveis de hardware

#define Porta_Lcd PORTD
#define RS PORTCbits.RC0
#define E PORTCbits.RC1
#define _XTAL_FREQ 4000000 
#define pot PORTAbits.RA0

void Envia_Comando_Lcd (char comando);
void Envia_Dado_Lcd (char dado);
void Inicializa_Lcd (void);
void Print_Mensagem_Lcd (char *mensagem);
void Delay_inicializacao_LCD();
unsigned int ADC_read(int channel);



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

void main()
{
	
	TRISC=0b11111100; // Pinos de controle do LCD como saida
	TRISD=0b00000000; // Configura a porta do lcd
    TRISAbits.RA0=1;
    TRISAbits.RA1=1;
    TRISAbits.RA2=1;
    TRISAbits.RA3=1;
    
    
   
	Inicializa_Lcd ();
 
	while(1){
        float AD0val=ADC_read(0);
        float AD1val=ADC_read(1);
        float AD2val=ADC_read(2);
        float AD3val=ADC_read(3);
        
        AD0val=(AD0val*0.0048828125)*20;
        AD1val=(AD1val*0.0048828125)*20;
        AD2val=(AD2val*0.0048828125)*20;
        AD3val=(AD3val*0.0048828125)*20;
        
        char buf[16];
        
        sprintf(buf, "%.4g", AD0val);
        Print_Mensagem_Lcd("Entrada 1: ");
        Print_Mensagem_Lcd(buf);
        __delay_ms(1000);
        Envia_Comando_Lcd(1);
        
        sprintf(buf, "%.4g", AD1val);
        Print_Mensagem_Lcd("Entrada 2: ");
        Print_Mensagem_Lcd(buf);
        __delay_ms(1000);
        Envia_Comando_Lcd(1);
        
        sprintf(buf, "%.4g", AD2val);
        Print_Mensagem_Lcd("Entrada 3: ");
        Print_Mensagem_Lcd(buf);
        __delay_ms(1000);
        Envia_Comando_Lcd(1);
        
        sprintf(buf, "%.4g", AD3val);
        Print_Mensagem_Lcd("Entrada 4: ");
        Print_Mensagem_Lcd(buf);
        __delay_ms(1000);
        Envia_Comando_Lcd(1);
    }
  
}

unsigned int ADC_read(int channel){
    ADFM = 1; //results right justified
    ADCS0 = 0; //conversion speed = 4*Tosc
    ADCS1 = 0;
    ADCS2 = 1;
    ADCON0bits.CHS = channel;  
    ADON = 1; //turn on ADC
    __delay_ms(1);   //should be enough to cover acquisition time
    GO_DONE = 1;
    while(ADCON0bits.GO_DONE == 1);
    unsigned int adval = (ADRESH << 8) + ADRESL;
    return adval;
}