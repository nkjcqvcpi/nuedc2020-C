#include <msp430.h>
#define uint unsigned int
#define delay_us(x) __delay_cycles((unsigned long)(4*x))
#define delay_ms(x) __delay_cycles((unsigned long)(4000*x))
#define delay_s(x) __delay_cycles((unsigned long)(1000000*x))



int main(void){
    WDTCTL = WDTPW + WDTHOLD;
    P6DIR |= BIT0+BIT1+BIT2+BIT3;
    P6OUT = 0xA;

    P2DIR |= BIT4+BIT5;
    P2SEL |= BIT4+BIT5;
    TA2CCR0 = 512-1;
    TA2CCTL1 = OUTMOD_7;
    TA2CCTL2 = OUTMOD_7;

    //set duty cycle and start PWM output
    TA2CCR1 = 350;
    TA2CCR2 = 350;
    TA2CTL = TASSEL_2 + MC_1 + TACLR;
    delay_s(0.01);
    TA2CCR1 = 200;
    TA2CCR2 = 200;
    delay_s(3);
    P6OUT = 0x9;
    TA2CCR1 = 300;
    TA2CCR2 = 300;
    delay_s(4);
    P6OUT = 0xA;
    TA2CCR1 = 150;
    TA2CCR2 = 150;
    delay_s(3);
    TA2CCR1 = 0;
    TA2CCR2 = 0;
    P1OUT |= BIT0;
    delay_s(3);
    P1OUT &= BIT0;

    /*while(1){
        switch(P2IN&0x1C){
            case 0x18: case 0x10: TA0CCR3 = 250; = 50;delay_s(0.6);break;
            case 0xC: case 0x1C: TA0CCR3 = 0; = 0;P4OUT = 0x00;P8OUT |= BIT1;delay_s(1);P8OUT &= ~BIT1;break;
            case 0x4: TA0CCR3 = 100; = 200;break;
            default: TA0CCR3 = 150; = 150;break;
        }
    }*/
}
