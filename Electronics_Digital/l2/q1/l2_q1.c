#include <p18f4520.h>

#pragma config OSC = INTIO67
#pragma config WDT = OFF

// Output
#define displaySegundos LATC
#define displayMinutos LATA
#define displayHoras LATB
#define buzzer LATDbits.LATD3
// Input
#define botaoModo PORTDbits.RD0
#define botaoAjusteMinutos PORTDbits.RD1
#define botaoAjusteHoras PORTDbits.RD2

// Relógio
char unidadeSegundos = 0, dezenaSegundos = 0;
char unidadeMinutos = 0, dezenaMinutos = 0;
char unidadeHoras = 0, dezenaHoras = 0;
// Alarme
char unidadeMinutosAlarme = 0, dezenaMinutosAlarme = 0;
char unidadeHorasAlarme = 0, dezenaHorasAlarme = 0;

char modoAjuste = 0, contadorModo = 0, alarmeDefinido = 0;
char buzzerAtivo = 0, duracaoBuzzer = 3, contadorBuzzer = 0;

void incrementaRelogio() {
	if (unidadeSegundos++ >= 9) {
		unidadeSegundos = 0;
		if (dezenaSegundos++ >= 5) {
			dezenaSegundos = 0;
			if (unidadeMinutos++ >= 9) {
				unidadeMinutos = 0;
				if (dezenaMinutos++ >= 5) {
					dezenaMinutos = 0;
					if (unidadeHoras++ >= 9 || 
						unidadeHoras >= 4 && dezenaHoras >= 2) {
						unidadeHoras = 0;
						if (dezenaHoras++ >= 2) {
							dezenaHoras = 0;
						}
					}
				}
			}
		}
	}
}

void delayS() {
	unsigned int i;
	for (i = 0; i < 16665; i++) {};
}

void delayMs(int ms) {
	unsigned char i;
	for (; ms > 0; ms--) {
		for (i = 0; i < 32; i++) {};
	}
}

void mostraRelogio() {
	// Relógio
	if (modoAjuste == 0) {
		displaySegundos = unidadeSegundos | dezenaSegundos << 4;
		displayMinutos = unidadeMinutos | dezenaMinutos << 4;
		displayHoras = unidadeHoras | dezenaHoras << 4;
	// Alarme
	} else {
		displaySegundos = 0;
		displayMinutos = unidadeMinutosAlarme | dezenaMinutosAlarme << 4;
		displayHoras = unidadeHorasAlarme | dezenaHorasAlarme << 4;
	}
}

void verificaModoAjuste() {
	if (botaoModo == 1) {
		contadorModo += 1;
	} else {
		contadorModo = 0;
	}
	if (contadorModo == 2) {
		modoAjuste = !modoAjuste;
		contadorModo = 0;
	}
}

void ajustaRelogio() {
	if (modoAjuste == 0) {
		// Ajuste dos Minutos
		if (botaoAjusteMinutos == 1) {
			if (unidadeMinutos++ >= 9) {
				unidadeMinutos = 0;
				if (dezenaMinutos++ >= 5) {
					dezenaMinutos = 0;
					if (unidadeHoras++ >= 9 || 
						unidadeHoras >= 4 && dezenaHoras >= 2) {
						unidadeHoras = 0;
						if (dezenaHoras++ >= 2) {
							dezenaHoras = 0;
						}
					}
				}
			}
		}
		// Ajuste das Horas
		if (botaoAjusteHoras == 1) {
			if (unidadeHoras++ >= 9 || 
				unidadeHoras >= 4 && dezenaHoras >= 2) {
				unidadeHoras = 0;
				if (dezenaHoras++ >= 2) {
					dezenaHoras = 0;
				}
			}
		}
	}
}

void ajustaAlarme() {
	if (modoAjuste == 1) {
		// Ajuste dos Minutos
		if (botaoAjusteMinutos == 1) {
			alarmeDefinido = 1;
			if (unidadeMinutosAlarme++ >= 9) {
				unidadeMinutosAlarme = 0;
				if (dezenaMinutosAlarme++ >= 5) {
					dezenaMinutosAlarme = 0;
					if (unidadeHorasAlarme++ >= 9 || 
						unidadeHorasAlarme >= 4 && dezenaHorasAlarme >= 2) {
						unidadeHorasAlarme = 0;
						if (dezenaHorasAlarme++ >= 2) {
							dezenaHorasAlarme = 0;
						}
					}
				}
			}
		}
		// Ajuste das Horas
		if (botaoAjusteHoras == 1) {
			alarmeDefinido = 1;
			if (unidadeHorasAlarme++ >= 9 || 
				unidadeHorasAlarme >= 4 && dezenaHorasAlarme >= 2) {
				unidadeHorasAlarme = 0;
				if (dezenaHorasAlarme++ >= 2) {
					dezenaHorasAlarme = 0;
				}
			}
		}
	}
}

void verificaAlarme() {
	if (alarmeDefinido && modoAjuste == 0) {
		if (unidadeMinutosAlarme == unidadeMinutos && 
			dezenaMinutosAlarme == dezenaMinutos &&
			unidadeHorasAlarme == unidadeHoras &&
			dezenaHorasAlarme == dezenaHoras) {
			alarmeDefinido = 0;
			buzzer = 1;
			buzzerAtivo = 1;
		}
	}
}

void verificaBuzzer() {
	if (buzzerAtivo) {
		contadorBuzzer += 1;
	}
	if (contadorBuzzer == duracaoBuzzer) {
		buzzer = 0;
		buzzerAtivo = 0;
		contadorBuzzer = 0;
	}
}

main() {
	TRISA = 0b00000000;
	TRISB = 0b00000000;
	TRISC = 0b00000000;
	TRISD = 0b11110111;
	while (1) {
		// Lógica Principal
		mostraRelogio();
		delayS();
		incrementaRelogio();
		// Funções Complementares
		verificaModoAjuste();
		verificaAlarme();
		verificaBuzzer();
		ajustaRelogio();
		ajustaAlarme();
	}
}