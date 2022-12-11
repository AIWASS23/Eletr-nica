#include <xc.h>

//declarar as suas variaveis de hardware

#define Porta_Lcd PORTD
#define RS1 PORTCbits.RC0
#define E1 PORTCbits.RC1
#define RS2 PORTCbits.RC2
#define E2 PORTCbits.RC3

void Envia_Comando_Lcd1 (char comando);
void Envia_Dado_Lcd1 (char dado);
void Inicializa_Lcd1 (void);
void Print_Mensagem_Lcd1(char *mensagem);
void Delay_inicializacao_LCD();

char mensagem1 [] = "Ola mundo";
char mensagem2 [] = "Hello world";

void Delay_LCD () // tempo ~ 1ms 
{
	int i;
	for (i=0;i<400;i++){}
}

void Envia_Comando_Lcd1 (char comando) // Envia_Comando_Lcd (0x38)
{
	E1=0; // LCD desabilitado
	RS1=0; //seleciona o envio de instrução

	E1=1;
	Porta_Lcd=comando; // Envia a metade alta (D4-D7)
	E1=0; // Habilita o LCD a receber informação

	E1=1; // Prepara para transferir dados
	comando = comando<<4;
	Porta_Lcd = comando;
	E1=0; // Habilita o lcd a receber a informacao

	Delay_LCD(); // Da o delay sugerido pelo fabricante
}

void Envia_Dado_Lcd1 (char dado)
{
	E1=0;
	RS1=1; // Seleciona o registrador de dados(LCD)
	
	E1=1;
	Porta_Lcd=dado;
	E1=0;
	
	E1=1;
	dado=dado<<4;
	Porta_Lcd=dado;
	E1=0;

	Delay_LCD();
}

void Inicializa_Lcd1 (void)
{
	Delay_inicializacao_LCD ();
	Envia_Comando_Lcd1 (0b000000010); // Lcd 4 bits
	Envia_Comando_Lcd1 (0b001001000); // 
	Envia_Comando_Lcd1 (0b000001110); // 
	Envia_Comando_Lcd1(0b000000001); // 
    
    //Envia_Dado_Lcd ('A');
    //while(1);
}

void Print_Mensagem_Lcd1(char *mensagem)
{
	int i=0;
	while (mensagem[i]) {
		Envia_Dado_Lcd1(mensagem[i]);
		i++;
	}
}
//----------------------------------------------------------------------------
void Envia_Comando_Lcd2 (char comando) // Envia_Comando_Lcd (0x38)
{
	E2=0; // LCD desabilitado
	RS2=0; //seleciona o envio de instrução

	E2=1;
	Porta_Lcd=comando; // Envia a metade alta (D4-D7)
	E2=0; // Habilita o LCD a receber informação

	E2=1; // Prepara para transferir dados
	comando = comando<<4;
	Porta_Lcd = comando;
	E2=0; // Habilita o lcd a receber a informacao

	Delay_LCD(); // Da o delay sugerido pelo fabricante
}

void Envia_Dado_Lcd2 (char dado)
{
	E2=0;
	RS2=1; // Seleciona o registrador de dados(LCD)
	
	E2=1;
	Porta_Lcd=dado;
	E2=0;
	
	E2=1;
	dado=dado<<4;
	Porta_Lcd=dado;
	E2=0;

	Delay_LCD();
}

void Inicializa_Lcd2 (void)
{
	Delay_inicializacao_LCD ();
	Envia_Comando_Lcd2 (0b000000010); // Lcd 4 bits
	Envia_Comando_Lcd2 (0b001001000); // 
	Envia_Comando_Lcd2 (0b000001110); // 
	Envia_Comando_Lcd2(0b000000001); // 
    
    //Envia_Dado_Lcd ('A');
    //while(1);
}

void Print_Mensagem_Lcd2(char *mensagem)
{
	int i=0;
	while (mensagem[i]) {
		Envia_Dado_Lcd2(mensagem[i]);
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
	
	TRISC=0b11110000; // Pinos de controle do LCD como saida
	TRISD=0b00000000; // Configura a porta do lcd
	Inicializa_Lcd1();
    Inicializa_Lcd2();
    Print_Mensagem_Lcd1(mensagem1);
    Print_Mensagem_Lcd2(mensagem2);
	while(1){
		
    }
  
}