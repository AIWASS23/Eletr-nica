#include "uart.h"

void configura_uart()
{
    TRISCbits.TRISC7=1;
    TRISCbits.TRISC6=1;
    TXSTA = 0B00100100;	
	RCSTA = 0B10010000;
	BAUDCON = 0B00001000;
	SPBRG = baud_rate_generate;
    SPBRGH= baud_rate_generate >> 8;
    PIE1bits.RCIE=1;
    PIR1bits.RCIF=0;
    IPR1bits.RCIP=1;
}
void transmite_string(char* dado)
{
    while((*dado)!='\0'&&(*dado)!=NULL)
    {
        transmite_char(*dado);
        dado=dado+1;
    }
}
void transmite_char (char dado)
{
	TXREG = dado;
	while (TXSTAbits.TRMT) {};
    while (!TXSTAbits.TRMT) {};
}

char recebe_char ()
{
	if (PIR1bits.RCIF)
	{
		PIR1bits.RCIF = 0;
		return RCREG;
	}
	else
	{
		return 0;
	}
}