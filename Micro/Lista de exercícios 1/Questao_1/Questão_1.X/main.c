#include <xc.h>
#include <p18f4520.h>

//declarar as suas variaveis de hardware

#define Porta_Lcd PORTD
#define RS PORTCbits.RC0
#define E PORTCbits.RC1

void Envia_Comando_Lcd (char comando);
void Envia_Dado_Lcd (char dado);
void Inicializa_Lcd (void);
void Print_Mensagem_Lcd (char *mensagem);
void Delay_inicializacao_LCD();

char mensagem1 [] = "Marcelo de Araújo";

void Delay_LCD () // tempo ~ 1ms 
{
	int i;
	for (i=0;i<400;i++){}
}

void Envia_Comando_Lcd (char comando) // Envia_Comando_Lcd (0x38)
{
	E=0; // LCD desabilitado
	RS=0; //seleciona o envio de instru��o

	E=1;
	Porta_Lcd=comando; // Envia a metade alta (D4-D7)
	E=0; // Habilita o LCD a receber informa��o

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
	Inicializa_Lcd ();
    Print_Mensagem_Lcd (mensagem1);
	while(1){
		
    }
  
}