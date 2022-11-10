#include <p18f4520.h>
#define dados PORTD

#define RS PORTCbits.RC0
#define RW PORTCbits.RC1
#define E PORTCbits.RC2

void envia_comando_lcd(char comando);
void envia_dado_lcd(char dado);
void mensagem_lcd(char *mensagem);
void inicializa_lcd(void);

char nome[] = ("MARCELO DE ARAUJO");

void delay_lcd(){
	int i;
	for (i = 0; i < 350; i++) {}
}

void delay_inicializa_lcd(){
	int i;
	for (i = 0; i < 4000; i++) {}
}

void envia_comando_lcd(char comando){
	E = 0;
	RS = 0;

	E = 1;
	dados = comando;
	E = 0;

	E = 1;
	comando = comando << 4;
	dados = comando;
	E = 0;

	delay_lcd();
}

void envia_dado_lcd(char dado){
	E = 0;
	RS = 1;

	E = 1;
	dados = dado;
	E = 0;

	E = 1;
	dado = dado << 4;
	dados = dado;
	E = 0;

	dados = dado;
	E = 0;

	delay_lcd();
}

void inicializa_lcd(void){
	delay_inicializa_lcd();
	envia_comando_lcd(0b00000010);
	envia_comando_lcd(0b00101000);
	envia_comando_lcd(0b00001110);
	envia_comando_lcd(0b00000001);
}

void mensagem_lcd(char *mensagem){
	int i = 0;
	while (mensagem[i])
	{
		envia_dado_lcd(mensagem[i]);
		i++;
	}
}

void main()
{
	int i;
	TRISC = 0b11111000;
	TRISD = 0b00000000;
	inicializa_lcd();

	while (1)
	{
		mensagem_lcd(nome);
		while (1);
	}
}

