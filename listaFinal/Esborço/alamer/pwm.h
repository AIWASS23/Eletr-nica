
#ifndef PWM_H
#define	PWM_H
void set_duty_pwm(int valor);
void set_periodo_pwm(unsigned char valor);
void configura_pwm()
{
    TRISCbits.TRISC2=1;
    TRISDbits.TRISD7=1;
    TRISDbits.TRISD6=1;
    TRISDbits.TRISD5=1;
    set_periodo_pwm(0xFF);
    CCP1CONbits.P1M1=0;
    CCP1CONbits.P1M0=0;
    CCP1CONbits.CCP1M3=1;
    CCP1CONbits.CCP1M2=1;
    CCP1CONbits.CCP1M1=0;
    CCP1CONbits.CCP1M0=0;
    set_duty_pwm(0);
    PIR1bits.TMR2IF=0;
    T2CONbits.T2CKPS1=1;//prescale 1:16
    T2CONbits.T2CKPS0=1;//prescale 1:16
    T2CONbits.TOUTPS3=0;
    T2CONbits.TOUTPS2=1;
    T2CONbits.TOUTPS1=1;
    T2CONbits.TOUTPS0=1;
    T2CONbits.TMR2ON=1;
    while(!PIR1bits.TMR2IF);
    PIR1bits.TMR2IF=0;
    TRISCbits.TRISC2=0;
    TRISDbits.TRISD7=0;
    TRISDbits.TRISD6=0;
    TRISDbits.TRISD5=0;
//    ECCP1ASbits.ECCPASE=0;
    
    PIE1bits.TMR2IE=1;
    IPR1bits.TMR2IP=0;
    PIR1bits.TMR2IF=0;

}
void set_duty_pwm(int valor)
{
    CCP1CONbits.DC1B0 = valor;
    CCP1CONbits.DC1B1 = valor >> 1;
    CCPR1L = valor >> 2;
}
void set_periodo_pwm(unsigned char valor){
   PR2 = valor;
}
#endif	/* PWM_H */

