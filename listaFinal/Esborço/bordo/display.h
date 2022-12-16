#ifndef DISPLAY_H
#define	DISPLAY_H
#include <xc.h>
#include <stdio.h>

#define _XTAL_FREQ 40000000

#define Display_rs PORTDbits.RD0
#define Display_rw PORTDbits.RD1
#define Display_e PORTDbits.RD2
#define Display_d4 PORTDbits.RD4
#define Display_d5 PORTDbits.RD5
#define Display_d6 PORTDbits.RD6
#define Display_d7 PORTDbits.RD7

#define T_rs TRISDbits.TRISD0
#define T_rw TRISDbits.TRISD1
#define T_e TRISDbits.TRISD2
#define T_d4 TRISDbits.TRISD4
#define T_d5 TRISDbits.TRISD5
#define T_d6 TRISDbits.TRISD6
#define T_d7 TRISDbits.TRISD7

#define FUNCTIONSET_4BIT_1LINE_HIGHFONT 0b00100100
#define FUNCTIONSET_4BIT_2LINE 0b00101000
#define DISPLAY_OFF 0b00001011
#define DISPLAY_CLEAR 0b00000001
#define ENTRYMODE_INCREMENTON_SHIFTOFF 0b00000110
#define ENTRYMODE_INCREMENTON_SHIFTON 0b00000111
#define DISPLAYON_CURSORON_BLINKON 0b00001111
#define DISPLAYON_CURSOROFF_BLINKOFF 0b00001100
   
void display_setup();
void display_send_string(char* str);
void display_send_command(char data);
void display_send_char(char data);
void display_send_nibble(char data);
void display_set_position(char linha, char coluna);
void display_send_int(long numero);
void display_send_float(float numero);
char busy();
#endif	/* DISPLAY_H */

