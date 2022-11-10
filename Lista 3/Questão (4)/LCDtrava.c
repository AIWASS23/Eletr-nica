#include <p18f4520.h>
#include <string.h>
#pragma config WDT = OFF
#define RS PORTCbits.RC0
#define RW PORTCbits.RC1
#define E PORTCbits.RC2

#define col1 PORTBbits.RB0
#define col2 PORTBbits.RB1
#define col3 PORTBbits.RB2

#define lin1 PORTBbits.RB3
#define lin2 PORTBbits.RB4
#define lin3 PORTBbits.RB5
#define lin4 PORTBbits.RB6

#define led PORTBbits.RB7

char tecla;
int tam_senha = 4;
char password[4] = "2121";
char senha_digitada[4];
int count = 0;

void delay_ms (int);
void milis (void);
void delay_s (int);
void envia_comando_lcd (char);
void envia_dado_lcd (char);
void inicializa_lcd (void);
void configura_pic (void);

char le_teclado (void);

void delay_ms (int ms){

	for (ms; ms > 0; ms--) {
		milis(); //loop demore 1 milisegundo
	};
}

void milis (void) {
	unsigned char j;
	unsigned int i;
	for (i = 0; i < 103; i++) {j = 0;}     
}

void delay_s (int s) {

	unsigned long int i;
	for (s; s > 0; s--) {
		for (i = 0; i < 83333; i++) {};
	}	
}

void envia_comando_lcd (char dado) {
	RS = 0;
	RW = 0;
	E = 1;
	PORTD = dado;
	E = 0;
	delay_ms(3);
}

void envia_dado_lcd (char comando) {
	RS = 1;
	RW = 0;
	E = 1;
	PORTD = comando;
	E = 0;
	delay_ms(3);
}

void inicializa_lcd () {
	delay_ms(30);   
	envia_comando_lcd(0b00111000); 
	envia_comando_lcd(0b00001110); 
	envia_comando_lcd(0x40);
	envia_comando_lcd(0b00000001);
}

void configura_pic () {
	TRISC = 0b00000000;             // controle
	TRISD = 0b00000000;             // dados
	TRISB = 0b01111000;             // numpad
	ADCON1 = 0b00001111;
}

char le_teclado () {
	delay_ms(2);
	col1 = 0;
	col2 = 1;
	col3 = 1;

	if (!lin1) {
		while (!lin1) {};
		return '1';
	}

	if (!lin2) {
		while (!lin2) {};
		return '4';
	}

	if (!lin3) {
		while (!lin3) {};
		return '7';
	}

	if (!lin4) {
		while (!lin4) {};
		return '*';
	}

	delay_ms(2);
	col1 = 1;
	col2 = 0;
	col3 = 1;

	if (!lin1) {
		while (!lin1) {};
		return '2';
	}

	if (!lin2) {
		while (!lin2) {};
		return '5';
	}

	if (!lin3) {
		while (!lin3) {};
		return '8';
	}

	if (!lin4) {
		while (!lin4) {};
		return '0';
	}

	delay_ms(2);
	col1 = 1;
	col2 = 1;
	col3 = 0;

	if (!lin1) {
		while (!lin1) {};
		return '3';
	}

	if (!lin2) {
		while (!lin2) {};
		return '6';
	}

	if (!lin3) {
		while (!lin3) {};
		return '9';
	}

	if (!lin4) {
		while (!lin4) {};
		return '#';
	}
	return 0;
}

void senha() {
	envia_dado_lcd(tecla);	
	senha_digitada[count++] = tecla;
    
	if (count == tam_senha) {
		if (strncmp(senha_digitada, password, tam_senha) == 0) {
			led = 1;
			delay_s(1);
			envia_comando_lcd(0b00000001);
			count = 0;
			led = 0;
		}
		else {
			led = 0;
			delay_s(1);
			envia_comando_lcd(0b00000001);
			count = 0;	
		}
	}	 
}

void main () {

	configura_pic();
	inicializa_lcd();
	
	while (1) {

		tecla = le_teclado();
		if (tecla) {
			senha();
		}	
	}
}
