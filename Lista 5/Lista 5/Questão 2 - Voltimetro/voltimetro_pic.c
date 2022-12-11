#include <p18f4520.h>
#include <float.h>

#pragma config WDT = OFF //Configura o PIC para não reiniciar depois de um tempo

#define RS PORTCbits.RC0
#define E PORTCbits.RC1

char ctrl;

int resultADo, volt_high, volt_low1, volt_low2;
 
void delay_milisegundo();
void envia_comando();
void config_pic();
void config_lcd();
void config_regist();
void mostra_volt();
void inrerrupcao_alta();


void delay_milisegundo(unsigned long int milisegundos){
    int i;

    for(milisegundos; milisegundos >0; milisegundos--){
        for(i=0; i<79; i++) {};
    }
}


//**************************     LCD    *********************************

void envia_comando(int comando){
    RS = 0; 
    PORTD = comando;
    E = 1;
    E = 0;
    delay_milisegundo(3);
}


void mostra_dado(char dado){

    RS = 1; //Enviando dado

    PORTD = dado;
    E = 1;
    E = 0;
    delay_milisegundo(3);
    PORTD = dado << 4;    
    E = 1;
    E = 0;
    delay_milisegundo(3);
}

void config_lcd(void){

    delay_milisegundo(75);
    envia_comando(0b00100000);
    envia_comando(0b00100000);
    envia_comando(0b10000000);
    //Display ligado, cursor ligado e piscamento desligado
    envia_comando(0b00000000);
    envia_comando(0b11000000);
    //Limpa display
    envia_comando(0b00000000);
    envia_comando(0b00010000);
}


//*************************    Voltagem    ********************************

    
void mostra_volt(int ordem){

    mostra_dado('v');
    mostra_dado(ordem + 48);
    mostra_dado(':');
    mostra_dado(volt_high + 48);
    mostra_dado(',');
    mostra_dado(volt_low1 + 48);
    mostra_dado(volt_low2 + 48);
    mostra_dado(' ');

    ADCON0bits.GO = 1;
}

//**************************     PIC     *********************************

void config_pic(void){
    TRISC = 0;
    TRISD = 0;
    TRISA = 0xFF;
}

void config_regist(void){

    INTCONbits.GIE = 1;   
    INTCONbits.PEIE = 1; 
    
    //Conversão AD    
    ADCON0bits.CHS3 = 0;
    ADCON0bits.CHS2 = 0;  //Escolhendo canal para receber voltagem
    ADCON0bits.CHS1 = 0;
    ADCON0bits.CHS0 = 0;
    ADCON0bits.ADON = 1;  //Habilita conversor AD
    
    ADCON1 = 0b00001011;  

    ADCON2bits.ADFM = 1;  //10 bits mais a direita
    ADCON2bits.ACQT2 = 1;
    ADCON2bits.ACQT1 = 1; //Tempo de aquisição = 20 TaD
    ADCON2bits.ACQT0 = 1;
    ADCON2bits.ADCS2 = 1;
    ADCON2bits.ADCS1 = 0; //O clock do conversor é Fosc/4
    ADCON2bits.ADCS0 = 0;
    
    //Habilitando interrupções AD    
    PIE1bits.ADIE = 1;   
    IPR1bits.ADIP = 1;   //interrupções AD possuem alta prioridade 
    
    delay_milisegundo(1);
    
    ADCON0bits.GO = 1;   //Inicia conversão
}


//*************************     Interrup    ********************************

//Interrupção de alta prioridade (0x08)

#pragma code alta_prioridade = 0x08

void interrupcao_de_alta_prioridade(void)
{
  _asm goto inrerrupcao_alta _endasm
}

#pragma code

#pragma interrupt inrerrupcao_alta 

void inrerrupcao_alta(void){

    if(PIR1bits.ADIF){
        
        resultADo = (((int)ADRESH<<8) | (ADRESL));
        volt_high = resultADo*0.0048828125;
        volt_low1 = (((resultADo*0.0048828125)*10)-(volt_high*10));
        volt_low2 =  ((((resultADo*0.0048828125)*100)-(volt_high*100))-(volt_low1*10));
        PIR1bits.ADIF = 0;
        ctrl++;
        ADCON0 = (ctrl << 2) | 0b00000001;
        if(ctrl == 4){
            ctrl = 0;
            ADCON0 = (ctrl<<2) | 0b00000001;
        }
    }
}
    
    
void main()
{
    config_pic();
    config_lcd();
    config_regist();
    while(1)
     {
        mostra_volt(1);
        mostra_volt(2);
        envia_comando(0b11000000);
        envia_comando(0b00000000);
        mostra_volt(3);
        mostra_volt(4);
        envia_comando(0b00000000);
        envia_comando(0b00100000);
      }

}
