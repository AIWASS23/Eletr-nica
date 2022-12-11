#include <p18f4520.h>
#define barramento_dados PORTD

#define RS PORTCbits.RC0
#define RW PORTCbits.RC1
#define E0 PORTCbits.RC2
#define E1 PORTCbits.RC3

void envia_comando_lcd1(char comando);
void envia_dado_lcd1(char dado);
void envia_comando_lcd2(char comando);
void envia_dado_lcd2(char dado);
void mensagem_lcd1(char *mensagem);
void mensagem_lcd2(char *mensagem);
void inicializa_lcd1(void);
void inicializa_lcd2(void);

char palavra_pt_br[] = ("Ola");
char palavra_en[] = ("Hello");

void delay_lcd(){
	int i;
	for (i = 0; i < 400; i++) {}
}

void delay_inicializa_lcd(){
	int i;
	for (i = 0; i < 4000; i++) {}
}

void envia_comando_lcd1(char comando){
	E0 = 0;
	RS = 0;

	E0 = 1;
	barramento_dados = comando;
	E0 = 0;

	E0 = 1;
	comando = comando << 4;
	barramento_dados = comando;
	E0 = 0;

	delay_lcd();
}

void envia_comando_lcd2(char comando){
	E1 = 0;
	RS = 0;

	E1 = 1;
	barramento_dados = comando;
	E1 = 0;

	E1 = 1;
	comando = comando << 4;
	barramento_dados = comando;
	E1 = 0;

	delay_lcd();
}

void envia_dado_lcd1(char dado){
	E0 = 0;
	RS = 1;

	E0 = 1;
	barramento_dados = dado;
	E0 = 0;

	E0 = 1;
	dado = dado << 4;
	barramento_dados = dado;
	E0 = 0;

	barramento_dados = dado;
	E0 = 0;

	delay_lcd();
}

void envia_dado_lcd2(char dado){
	E1 = 0;
	RS = 1;

	E1 = 1;
	barramento_dados = dado;
	E1 = 0;

	E1 = 1;
	dado = dado << 4;
	barramento_dados = dado;
	E1 = 0;

	barramento_dados = dado;
	E1 = 0;

	delay_lcd();
}

void inicializa_lcd1(void){
	delay_inicializa_lcd();
	envia_comando_lcd1(0b00000010);
	envia_comando_lcd1(0b00101000);
	envia_comando_lcd1(0b00001110);
	envia_comando_lcd1(0b00000001);
}

void inicializa_lcd2(void){
	delay_inicializa_lcd();
	envia_comando_lcd2(0b00000010);
	envia_comando_lcd2(0b00101000);
	envia_comando_lcd2(0b00001110);
	envia_comando_lcd2(0b00000001);
}

void mensagem_lcd1(char *mensagem){
	int i = 0;
	while (mensagem[i])
	{
		envia_dado_lcd1(mensagem[i]);
		i++;
	}
}

void mensagem_lcd2(char *mensagem){
	int i = 0;
	while (mensagem[i])
	{
		envia_dado_lcd2(mensagem[i]);
		i++;
	}
}

void main()
{
	int i;

	TRISC = 0b11110000;
	TRISD = 0b00000000;
	inicializa_lcd1();
	inicializa_lcd2();

	while (1)
	{
		mensagem_lcd1(palavra_pt_br);
		mensagem_lcd2(palavra_en);
		while (1);
	}
}
