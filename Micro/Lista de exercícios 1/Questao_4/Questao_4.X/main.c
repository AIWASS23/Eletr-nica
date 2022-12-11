#include <xc.h>
#include <string.h>

#define _XTAL_FREQ 4000000

#define roll_1 PORTCbits.RC2
#define roll_2 PORTCbits.RC3
#define roll_3 PORTCbits.RC4
#define roll_4 PORTCbits.RC5

#define col_1 PORTDbits.RD3
#define col_2 PORTDbits.RD2
#define col_3 PORTDbits.RD1

char password[]={0,1,2,3};
char entry[]={0,0,0,0};
int i=0;
int key_pressed();
int get_key();

int key_pressed(){
    
    col_1=0;col_2=1;col_3=1;
    if(roll_1==0){__delay_ms(250);return 1;}
    if(roll_2==0){__delay_ms(250);return 4;}
    if(roll_3==0){__delay_ms(250);return 7;}
    if(roll_4==0){__delay_ms(250);return 11;}
    
    
    col_1=1;col_2=0;col_3=1;
    if(roll_1==0){__delay_ms(250);return 2;}
    if(roll_2==0){__delay_ms(250);return 5;}
    if(roll_3==0){__delay_ms(250);return 8;}
    if(roll_4==0){__delay_ms(250);return 0;}
    
    
    col_1=1;col_2=1;col_3=0;
    if(roll_1==0){__delay_ms(250);return 3;}
    if(roll_2==0){__delay_ms(250);return 6;}
    if(roll_3==0){__delay_ms(250);return 9;}
    if(roll_4==0){__delay_ms(250);return 12;}
    
    
    return -1;
}
int get_key(){
    int key=-1;
    while(key==-1)key=key_pressed();
    return key;
}

void main(){
    TRISC=0b11111111;
    TRISD=0b00000000;
    while(1){
        if(i>3)i=0;
        int key=get_key();
        if(key==12){
            if(memcmp(entry,password,sizeof(entry))==0){
                PORTDbits.RD7=1;
                __delay_ms(2000);
                PORTDbits.RD7=0;
                
                entry[0]=0;
                entry[1]=0;
                entry[2]=0;
                entry[3]=0;
                i=0;
            }
        }
        else if(key==11){
            entry[0]=0;
            entry[1]=0;
            entry[2]=0;
            entry[3]=0;
            i=0;
        }
        else{
            entry[i++]=key;
            
        }
 
        
    }
}