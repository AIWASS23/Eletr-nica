#ifndef UART_H
#define	UART_H

#include <xc.h>

#define _XTAL_FREQ 40000000
#define baud_rate_generate 1040



void configura_uart();
void transmite_string(char* dado);
void transmite_char (char dado);
char recebe_char ();

#endif	