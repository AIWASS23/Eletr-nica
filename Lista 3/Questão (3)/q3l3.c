#include <p18f4520.h>
#define porta_lcd PORTD
#define RS PORTCbits.RC0
#define E PORTCbits.RC2

void envia_comando_lcd(char comando);
void envia_dado_lcd(char dado);
void mensagem_lcd(char *mensagem);
void inicializa_lcd(void);

char mensagem1[] = (" Egipt");

unsigned char desenho[] = { 
	0b00000,
	0b00000,
	0b00100,
	0b01010,
	0b10001,
	0b11111,
	0b00000,
	0b00000
};

void delay_lcd(){
	int i;
	for (i = 0; i < 400; i++) {}
}

void delay_inicializacao_lcd(){
	int i;
	for (i = 0; i < 4000; i++) {}
}

void spechar(unsigned char *desenho, const char location){
	int i = 0;
	envia_comando_lcd(0x40 + (location *8));
	for (i = 0; i < 8; i++)
		envia_dado_lcd(desenho[i]);
}

void envia_comando_lcd(char comando){
	E = 0;
	RS = 0;

	E = 1;
	porta_lcd = comando;
	E = 0;

	E = 1;
	comando = comando << 4;
	porta_lcd = comando;
	E = 0;

	delay_lcd();
}

void envia_dado_lcd(char dado){
	char m;
	E = 0;
	RS = 1;

	E = 1;
	porta_lcd = dado;
	E = 0;

	E = 1;
	dado = dado << 4;
	porta_lcd = dado;
	E = 0;

	porta_lcd = dado;
	E = 0;

	delay_lcd();
}

void inicializa_lcd(void){
	delay_inicializacao_lcd();
	envia_comando_lcd(0b00000010);
	envia_comando_lcd(0b00101000);
	envia_comando_lcd(0b00001110);
	envia_comando_lcd(0b00000001);
}

void mensagem_lcd(char *mensagem){
	int i = 0;
	while (mensagem[i])	{
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
	spechar(desenho, 1);

	while (1)
	{
		envia_comando_lcd(0x01);
		envia_dado_lcd(1);
        envia_dado_lcd(1);
        envia_dado_lcd(1);
		mensagem_lcd(mensagem1);
		envia_comando_lcd(0x106); 
		
		while (1);
	}
}
