#include <p18f4520.h>

#define RS PORTCbits.RC0
#define E PORTCbits.RC1

//colunas do teclado
#define col1 PORTBbits.RB0
#define col2 PORTBbits.RB1
#define col3 PORTBbits.RB2

//linhas
#define lin1 PORTBbits.RB3
#define lin2 PORTBbits.RB4
#define lin3 PORTBbits.RB5
#define lin4 PORTBbits.RB6

int digits;

void delay_milisegundo();
void envia_comando();
void mostra_dado();
void config_lcd();
void reset_lcd();

void config_pic();
void config_regist();

void varre_teclado();
void interrupcao_alta();



void delay_milisegundo(unsigned long int milisegundos){
    int i;

    for(milisegundos; milisegundos >0; milisegundos--){
        for(i=0; i<109; i++) {};
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
    
    delay_milisegundo(70);
    envia_comando(0b00100000);
    envia_comando(0b00100000);
    envia_comando(0b10000000);
    // Display ligado, cursor desligado e piscamento desligado
    envia_comando(0b00000000);
    envia_comando(0b11000000);
    //Limpa display
    envia_comando(0b00000000);
    envia_comando(0b00010000);
}
    
void reset_lcd(void){
    
    if(digits == 16){
        envia_comando(0b11000000);
        envia_comando(0b00000000);
    }
    
    if(digits == 33){
    //Limpando display
        envia_comando(0b00000000);
        envia_comando(0b00010000);
        digits = 0;
    }
}


//*************************    Teclado   *********************************

void varre_teclado(void){

    //Varre da lin1 até a lin4
    lin1 = 1;    
    delay_milisegundo(2);
    
    lin1 = 0;   
    lin2 = 1;    
    delay_milisegundo(2);
    
    lin2 = 0;    
    lin3 = 1;    
    delay_milisegundo(2);
    
    lin3 = 0;    
    lin4 = 1;    
    delay_milisegundo(2);
    
    lin4 = 0;
}


//**************************     PIC     *********************************

void config_pic(void){
    TRISB = 0b00000111;      
    lin1 = 0;
    lin2 = 0;
    lin3 = 0;
    lin4 = 0;
    TRISC = 0;           
    TRISD = 0;           
    ADCON1 = 0b00001111;     //Config todas as portas para digital
}

void config_regist(void){

    INTCONbits.GIE = 1;      //Habilita interrupções de alta prioridade
    INTCONbits.PEIE = 1;     //Habilita interrupções de baixa prioridade
    INTCONbits.INT0IE = 1;   //Habilita interrupções externas no INT0
    INTCON3bits.INT1IE = 1;  //Habilita interrupções externas no INT1
    INTCON3bits.INT2IE = 1;  //Habilita interrupções externas no INT2
    INTCON3bits.INT1IP = 1;  //Interrupções de INT1 como alta prioridade
    INTCON3bits.INT2IP = 1;  //Interrupções de INT2 como alta prioridade
    INTCON2bits.INTEDG0 = 1; //Interrupções INT0 na subida
    INTCON2bits.INTEDG1 = 1; //Interrupções INT1 na subida
    INTCON2bits.INTEDG2 = 1; //Interrupções INT2 na subida 
}
    

//*************************     Interrup    ********************************

//Interrupção de alta prioridade (0x08)

#pragma code alta_prioridade = 0x08

void interrupcao_de_alta_prioridade(void)
{
  _asm goto interrupcao_alta _endasm
}

#pragma code

#pragma interrupt interrupcao_alta 

void interrupcao_alta(void){

    // analisa interrup da coluna 1
    if(INTCONbits.INT0IF){
        
        if(lin1){
            while(col1){};
            
            mostra_dado('1');
            digits++;
        }
    
        if(lin2){
            while(col1){}
            mostra_dado('4');
            digits++;
        }
    
        if(lin3){
            while(col1){}
            mostra_dado('7');
            digits++;
        }
    
        if(lin4){
            while(col1){}
            mostra_dado('*'); 
            digits++;
        }
    
        INTCONbits.INT0IF = 0;
    }
    
    // analisa interrup da coluna 2
    if(INTCON3bits.INT1IF){ 
            
        if(lin1){
            while(col2){}
            mostra_dado('2'); 
            digits++;
        }
    
        if(lin2){
            while(col2){}
            mostra_dado('5'); 
            digits++;
        }
    
        if(lin3){
            while(col2){}
            mostra_dado('8'); 
            digits++;
        }
    
        if(lin4){
            while(col2){}
            mostra_dado('0'); 
            digits++;
        }
    
        INTCON3bits.INT1IF = 0;
        }
    
    // analisa interrup da coluna 3
    if(INTCON3bits.INT2IF){
            
        if(lin1){
            while(col3){}
            mostra_dado('3');
            digits++;
        }
    
        if(lin2){
            while(col3){}
            mostra_dado('6');
            digits++;
        }
    
        if(lin3){
            while(col3){}
            mostra_dado('9');
            digits++;
        }
    
        if(lin4){
            while(col3){}
            mostra_dado('#');
            digits++;
        }
    
        INTCON3bits.INT2IF = 0;
    }
}

void main(){
    config_pic();
    config_regist();
    config_lcd();
    
    while(1){
       varre_teclado();
       reset_lcd();
     }

}
