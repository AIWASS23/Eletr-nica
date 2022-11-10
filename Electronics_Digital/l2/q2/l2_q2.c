#include <p18f4520.h>

#pragma config OSC = INTIO67
#pragma config WDT = OFF

#define selecao LATA
#define dadosPrimeiraMatriz LATC
#define dadosSegundaMatriz LATD

char contadorVarredura = 0, selecaoVarredura = 0;
char contadorMensagem = 0, tamanhoMensagem = 0;
char contadorFrame = 0, duracaoFrame = 2;

char mensagem[] = {
	// Início
	0b11111111,
	0b11111111,
	0b11111111,
	0b11111111,
	0b11111111,
	0b11111111,
	0b11111111,
	0b11111111,
	// Conteúdo
	0b00000001,
	0b11101101,
	0b11101101,
	0b11101101,
	0b00010011,
	0b11111111,
	0b10000011,
	0b01111101,
	0b01111101,
	0b01111101,
	0b10000011,
	0b11111111,
	0b00000001,
	0b01111101,
	0b01111101,
	0b01111101,
	0b10000011,
	0b11111111,
	0b00000001,
	0b11101101,
	0b11101101,
	0b11101101,
	0b00010011,
	0b11111111,
	0b01111101,
	0b00000001,
	0b01111101,
	0b11111111,
	0b10000011,
	0b01111101,
	0b01101101,
	0b01101101,
	0b10001011,
	0b11111111,
	0b10000011,
	0b01111101,
	0b01111101,
	0b01111101,
	0b10000011,
	0b11111111,
	// Fim
	0b11111111,
	0b11111111,
	0b11111111,
	0b11111111,
	0b11111111,
	0b11111111,
	0b11111111,
	0b11111111
};

void delayVarredura() {
	char i;
	for (i = 0; i < 40; i++);
}

void mostraMensagem() {
	char indiceSegundaMatriz = (contadorMensagem + contadorVarredura) % tamanhoMensagem;
	char indicePrimeiraMatriz = (indiceSegundaMatriz - 8);
	if (indicePrimeiraMatriz < 0) {
		indicePrimeiraMatriz = tamanhoMensagem + indicePrimeiraMatriz;
	}
	dadosPrimeiraMatriz = mensagem[indicePrimeiraMatriz];
	dadosSegundaMatriz = mensagem[indiceSegundaMatriz];
}

void selecionaColunas() {
	if (selecaoVarredura == 0) {
		selecaoVarredura = 1 << contadorVarredura;
	} else {
		selecaoVarredura = 0;
	}
	selecao = selecaoVarredura;
}

void preparaVarredura() {
	if (selecaoVarredura == 0) {
		contadorVarredura += 1;
		if (contadorVarredura == 8) {
			contadorVarredura = 0;
			contadorFrame += 1;
			if (contadorFrame == duracaoFrame) {
				contadorFrame = 0;
				contadorMensagem += 1;
				if (contadorMensagem == tamanhoMensagem) {
					contadorMensagem = 0;
				}	
			}
		}
	}
}

main() {
	TRISA = 0b00000000;
	TRISC = 0b00000000;
	TRISD = 0b00000000;

	tamanhoMensagem = sizeof(mensagem) / sizeof(mensagem[0]);
	selecao = 0b00000000;
	while (1) {
		mostraMensagem();
		delayVarredura();
		selecionaColunas();
		preparaVarredura();
	}
}