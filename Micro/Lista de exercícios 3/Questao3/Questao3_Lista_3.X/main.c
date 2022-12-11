#include <xc.h>
#include <stdio.h> 
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#define _XTAL_FREQ 4000000

#pragma config PBADEN = OFF

void UART_PutC(const char data);
char UART_GetC();
void UART_Init(const uint32_t baud_rate);
void UART_Print(const char *data);
__bit UART_Data_Ready();
void interrupt ISR();
void init_interruption();
void init_timer();

char buffer[30];
char command[30];
char send[30];
int Timer0_count=0;

int trigger_alarm=0;
int flag_uart=0;

void interrupt ISR(){
    if(INTCONbits.INT0IF==1){
        INTCONbits.INT0IF=0;
        if(trigger_alarm>0){
            trigger_alarm=2;
            flag_uart=1;
            strcpy(send,"Alarme 1 acionado\r\n");
        }   
    }
    
    if(INTCON3bits.INT1IF==1){
        INTCON3bits.INT1IF=0;
        if(trigger_alarm>0){
            trigger_alarm=2;
            flag_uart=1;
            strcpy(send,"Alarme 2 acionado\r\n");
        }    
    }
    
    if(INTCON3bits.INT2IF==1){
        INTCON3bits.INT2IF=0;
        if(trigger_alarm>0){
            trigger_alarm=2;
            flag_uart=1;
            strcpy(send,"Alarme 3 acionado\r\n");
        }    
    }
    
    if(INTCONbits.RBIF==1){
        INTCONbits.RBIF=0;
        if(trigger_alarm>0){
            trigger_alarm=2;
            flag_uart=1;
            strcpy(send,"Alarme 4 acionado\r\n");
        }   
    }
    if(INTCONbits.TMR0IF==1){
        if(trigger_alarm>1)PORTBbits.RB7=!PORTBbits.RB7;
        INTCONbits.TMR0IF=0;
        TMR0L=0;
    }
}
void init_interruption(){
    INTCON2bits.RBPU=0;//habilita resistores de pullup
    
    INTCONbits.INT0IF=0;//limpa flag da int0
    INTCONbits.INT0IE=1;//habilita int0
    
    INTCON3bits.INT1IF=0;//limpa flag da int1
    INTCON3bits.INT1IE=1;//habilita int1
    
    INTCON3bits.INT2IF=0;//limpa flag da int1
    INTCON3bits.INT2IE=1;//habilita int1
    
    INTCONbits.RBIE=1;
    INTCONbits.RBIF=0;
    IOCBbits.IOCB4=1;
    IOCBbits.IOCB5=0;
    IOCBbits.IOCB6=0;
    IOCBbits.IOCB7=0;
    INTCON2bits.RBIP=1;
    
    INTCON2bits.INTEDG0=1;//configura int0 para ativar na subida
    INTCON2bits.INTEDG1=1;//configura int1 para ativar na subida
    INTCON2bits.INTEDG2=1;//configura int1 para ativar na subida
    
    INTCONbits.GIE=1;//habilita interrupcoes
}
void init_timer(){
    INTCONbits.PEIE=1;
    INTCONbits.TMR0IE=1;//habilita TIMER0
    INTCONbits.TMR0IF=0;//limpa flag do TIMER0
    T0CONbits.TMR0ON=1;//habilita TIMER0
    T0CONbits.T08BIT=1;//configura TIMER0 com 8bits
    T0CONbits.T0CS=0;//seleciona fonte de clock do TIMER0
    
    TMR0L=0;//zera contador do TIMER0
    
    INTCONbits.GIE=1;//habilita interrupcoes
}

void UART_Init(const uint32_t baud_rate)
{
  int16_t n = ( _XTAL_FREQ / (16 * baud_rate) ) - 1;
  
  if (n < 0)
    n = 0;
 
  if (n > 255)  // low speed
  {
    n = ( _XTAL_FREQ / (64 * baud_rate) ) - 1;
    if (n > 255)
      n = 255;
    SPBRG = n;
    TXSTA = 0x20;  // transmit enabled, low speed mode
  }
 
  else   // high speed
  {
    SPBRG = n;
    TXSTA = 0x24;  // transmit enabled, high speed mode
  }
 
  RCSTA = 0x90;  // serial port enabled, continues receive enabled
 
}
__bit UART_Data_Ready()
{
  return RCIF;  // return RCIF bit (register PIR1, bit 5)
}
char UART_GetC()
{
  while (RCIF == 0) ;  // wait for data receive
  if (OERR)  // if there is overrun error
  {  // clear overrun error bit
    CREN = 0;
    CREN = 1;
  }
  return RCREG;        // read from EUSART receive data register
}
 
void UART_PutC(const char data)
{
  while (TRMT == 0);  // wait for transmit shift register to be empty
  TXREG = data;       // update EUSART transmit data register
}
 
void UART_Print(const char *data)
{
  uint8_t i = 0;
  while (data[i]!='\0')
    UART_PutC (data[i++]);
}
void main()
{
    TRISB=0b01111111;// Configura interrupcaoes como entrada
    UART_Init(9600);
    init_interruption();
    init_timer();

	while(1)
	{
        if(flag_uart==1){
            flag_uart=0;
            UART_Print(send);
        } 
        if(UART_Data_Ready()){
            char c=UART_GetC();
            if(c=='<'){
                UART_PutC(c);
                while(c!='>'){
                    if(UART_Data_Ready()){
                        c=UART_GetC();
                        strcat(buffer,&c);
                        
                        UART_PutC(c);
                     
                    }
                }  
                memcpy(command,buffer,sizeof(buffer));
                memset(buffer, 0, sizeof(buffer));
  
            }
            //UART_Print(command);
            
            if(strcmp(command,"ARMA>")==0){
                UART_Print("Armando alarame\r\n");
                trigger_alarm=1;
                INTCONbits.GIE=1;
                
            }
            if(strcmp(command,"DESARMA>")==0){
                UART_Print("Desarmando alarame\r\n");
                trigger_alarm=0;
                INTCONbits.GIE=0;
                
            }          
        }		
	}
}
