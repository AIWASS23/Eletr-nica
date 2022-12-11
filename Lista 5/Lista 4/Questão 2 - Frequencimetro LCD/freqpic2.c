#include <p18f4520.h>
#include <stdlib.h>
#pragma config WDT = OFF    // Configura o PIC para não reiniciar depois de um tempo

#define RS PORTCbits.RC0
#define E PORTCbits.RC1

long int ctrl, P1, P2, freq1, freq2; 
unsigned char P_high1, P_low1, P_high2, P_low2;

int digits[10] = {'0','1','2','3','4','5','6','7','8','9'};
 
void delay_milisegundo();
void envia_comando();
void config_pic();
void config_lcd();
void config_regist();
void calcula_P();
void calcula_freq();
void mostra_freq();
void interrupcao_alta();


void delay_milisegundo(unsigned long int milisegundos){
    unsigned long int i;

    for(milisegundos; milisegundos >0; milisegundos--){
    	for(i=0; i<79; i++) {};
    }
}

//**************************     LCD    *********************************

void envia_comando(int Comando){
    RS = 0;             
    PORTD = Comando;    
    // Clock pro lcd
    E = 1;
    E = 0;
    delay_milisegundo(3);
}

void config_lcd(void){
    delay_milisegundo(70);
    envia_comando(0b00100000);
    envia_comando(0b00100000);
    envia_comando(0b10000000);
    // Display ligado, cursor desligado e piscamento desligado
    envia_comando(0b00000000);
    envia_comando(0b11000000);
    // Limpa display
    envia_comando(0b00000000);
    envia_comando(0b00010000);
}

// Frequência ou LCD , eis a questão
void mostra_freq(void){

    RS = 1; 
    //F
    PORTD = 0b01000110; 
    E = 1;
    E = 0;
    delay_milisegundo(3);
    PORTD = 0b01000110 << 4;
    E = 1;
    E = 0;
    delay_milisegundo(3);
    
    //1
    PORTD = 0b00110001; 
    E = 1;
    E = 0;
    delay_milisegundo(3);
    PORTD = 0b00110001 << 4;
    E = 1;
    E = 0;
    delay_milisegundo(3);
    
    //:
    PORTD = 0b00111010; 
    E = 1;
    E = 0;
    delay_milisegundo(3);
    PORTD = 0b00111010 << 4;
    E = 1;
    E = 0;
    delay_milisegundo(3);
    
    // freq1
    ctrl = freq1/1000 ;
    
    PORTD = digits[ctrl]; 
    E = 1;
    E = 0;
    delay_milisegundo(3);
    PORTD = digits[ctrl] << 4;
    E = 1;
    E = 0;
    delay_milisegundo(3);
    
    ctrl = (freq1/100) % 10;
    
    PORTD = digits[ctrl]; 
    E = 1;
    E = 0;
    delay_milisegundo(3);
    PORTD = digits[ctrl] << 4;
    E = 1;
    E = 0;
    delay_milisegundo(3);
    
    ctrl = (freq1/10) % 10;
    
    PORTD = digits[ctrl]; 
    E = 1;
    E = 0;
    delay_milisegundo(3);
    PORTD = digits[ctrl] << 4;
    E = 1;
    E = 0;
    delay_milisegundo(3);
    
    ctrl = freq1 % 10;
    
    PORTD = digits[ctrl]; 
    E = 1;
    E = 0;
    delay_milisegundo(3);
    PORTD = digits[ctrl] << 4;
    E = 1;
    E = 0;
    delay_milisegundo(3);
    
    //H
    PORTD = 0b01001000; 
    E = 1;
    E = 0;
    delay_milisegundo(3);
    PORTD = 0b01001000 << 4;
    E = 1;
    E = 0;
    delay_milisegundo(3);
    
    //z
    PORTD = 0b01111010; 
    E = 1;
    E = 0;
    delay_milisegundo(3);
    PORTD = 0b01111010 << 4;
    E = 1;
    E = 0;
    delay_milisegundo(3);
    
    envia_comando(0b11000000);
    envia_comando(0b00000000);
    
    RS = 1;
    
    //F
    PORTD = 0b01000110; 
    E = 1;
    E = 0;
    delay_milisegundo(3);
    PORTD = 0b01000110 << 4;
    E = 1;
    E = 0;
    delay_milisegundo(3);
    
    //2
    PORTD = 0b00110010; 
    E = 1;
    E = 0;
    delay_milisegundo(3);
    PORTD = 0b00110010 << 4;
    E = 1;
    E = 0;
    delay_milisegundo(3);
    
    //:
    PORTD = 0b00111010; 
    E = 1;
    E = 0;
    delay_milisegundo(3);
    PORTD = 0b00111010 << 4;
    E = 1;
    E = 0;
    delay_milisegundo(3);
    
    ctrl = freq2/1000 ;
    
    PORTD = digits[ctrl]; 
    E = 1;
    E = 0;
    delay_milisegundo(3);
    PORTD = digits[ctrl] << 4;
    E = 1;
    E = 0;
    delay_milisegundo(3);
    
    ctrl = (freq2/100) % 10;
    
    PORTD = digits[ctrl]; 
    E = 1;
    E = 0;
    delay_milisegundo(3);
    PORTD = digits[ctrl] << 4;
    E = 1;
    E = 0;
    delay_milisegundo(3);
    
    ctrl = (freq2/10) % 10;
    
    PORTD = digits[ctrl]; 
    E = 1;
    E = 0;
    delay_milisegundo(3);
    PORTD = digits[ctrl] << 4;
    E = 1;
    E = 0;
    delay_milisegundo(3);
    
    ctrl = freq2 % 10;
    
    PORTD = digits[ctrl]; 
    E = 1;
    E = 0;
    delay_milisegundo(3);
    PORTD = digits[ctrl] << 4;
    E = 1;
    E = 0;
    delay_milisegundo(3);
    
    //H
    PORTD = 0b01001000; 
    E = 1;
    E = 0;
    delay_milisegundo(3);
    PORTD = 0b01001000 << 4;
    E = 1;
    E = 0;
    delay_milisegundo(3);
    
    //z
    PORTD = 0b01111010; 
    E = 1;
    E = 0;
    delay_milisegundo(3);
    PORTD = 0b01111010 << 4;
    E = 1;
    E = 0;
    delay_milisegundo(3);
    
    envia_comando(0b00000000);
    envia_comando(0b00100000);
}


//***********************     Frequencia    ******************************

void calcula_P(void){
    P1 = ((long int)P_high1<<8 | P_low1)*2;
    P2 = ((long int)P_high2<<8 | P_low2)*2;
}

void calcula_freq(void){
    freq1 = (1000000.0/(P1)) + 0.5;
    freq2 = (1000000.0/(P2)) + 0.5;
}



//**************************     PIC    *********************************

void config_pic(void){
    TRISC = 0;         
    TRISD = 0;         
    TRISBbits.RB0 = 1; 
    TRISBbits.RB1 = 1; 
    //TRISBbits.RB2 = 1;
    
    ADCON1 = 0x0F; 
}

void config_regist(void){
    RCONbits.IPEN = 1;      // Habilita níveis de prioridade
    INTCONbits.GIE = 1;     // Habilita interrupções de alta prioridade
    INTCONbits.PEIE = 1;    // Habilita interrupções de baixa prioridade
    INTCONbits.INT0IE = 1;  // Habilita interrupções externas em INT0
    INTCON3bits.INT1IE = 1; // Desabilita interrupções externas em INT1
    INTCON3bits.INT1IP = 1; // Interrupções INT1 como alta priori
    INTCON2bits.INTEDG0 = 1;// Interrupções INT0 na subida
    INTCON2bits.INTEDG1 = 1;// Interrupções INT1 na subida
    
    // Timer0 (config)
    T0CONbits.TMR0ON = 1;   // Liga timer 0
    T0CONbits.T08BIT = 0;   // Timer0 16 bits
    T0CONbits.T0CS = 0;     // Fonte do timer 0 clock interno
    T0CONbits.PSA = 0;      // Habilitando Prescaler
    T0CONbits.TMR0ON = 1;   // Ligando timer 0
    T0CONbits.T0PS2 = 0;
    T0CONbits.T0PS1 = 0;    // Prescaler 1:4
    T0CONbits.T0PS0 = 1;
    
    // Timer 0 habilitado 
    INTCONbits.TMR0IE = 1;
    INTCON2bits.TMR0IP = 1;
    
    // Timer 1 (config)    
    T1CONbits.T1RUN = 0;    // Clock de fonte diferente do oscilador do timer 0
    T1CONbits.T1OSCEN = 0;  // Oscilador do timer 1 desligado
    T1CONbits.RD16 = 1;     // Timer  16 bits
    T1CONbits.TMR1CS = 0;   // Fonte do timer 1 clock interno
    T1CONbits.T1CKPS1 = 1;
    T1CONbits.T1CKPS0 = 0;  // Prescaler 1:4
    T1CONbits.TMR1ON = 1;   // Liga timer 1
    
    // Timer 1 habilitado    
    PIE1bits.TMR1IE = 1;
    IPR1bits.TMR1IP = 1;    
}

//**************************     Interrup    *********************************


#pragma code alta_prioridade = 0x08     //deixa prioridade merm que é mais seguro kkkk

void interrupcao_de_alta_prioridade(void)
{
  _asm goto interrupcao_alta _endasm
}

#pragma code

#pragma interrupt interrupcao_alta 

void interrupcao_alta(void){

    if(INTCONbits.INT0IF){  
        P_low1 = TMR0L;
        P_high1 = TMR0H;
        TMR0H = 0x00;      
        TMR0L = 0x03;       
        INTCONbits.INT0IF = 0;
    }

    if(INTCON3bits.INT1IF){ 
        P_low2 = TMR1L;
        P_high2 = TMR1H;
        TMR1H = 0x00;       
        TMR1L = 0x01;       
        INTCON3bits.INT1IF = 0;
    }
    
}
    
    
void main(){
    config_pic();
    config_lcd();
    config_regist();
    
    while(1){
        //aqui funciona
        calcula_P();
        calcula_freq();
        mostra_freq();
        delay_milisegundo(300);     // diminuir oscilação por precisão
    }
}