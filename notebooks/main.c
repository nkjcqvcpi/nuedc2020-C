#include <msp430.h>
#define uint unsigned int
#define delay_us(x) __delay_cycles((unsigned long)(4*x))
#define delay_ms(x) __delay_cycles((unsigned long)(4000*x))
#define delay_s(x) __delay_cycles((unsigned long)(1000000*x))

void hh(void)
{
    TA2CCR1 = 150;
    TA2CCR2 = 150;
    delay_s(4.75);

    //P6OUT = 0x9;
    TA2CCR1 = 50; //right
    TA2CCR2 = 410; //left
    delay_s(2);
    TA2CCR1 = 425;
    delay_s(0.05);

    //P6OUT = 0xA;
    TA2CCR1 = 100;
    TA2CCR2 = 100;
    delay_s(4);
}

void hf(void)
{
    TA2CCR1 = 141;
    TA2CCR2 = 141;
    delay_s(7);

    //P6OUT = 0x9;
    TA2CCR1 = 50; //right
    TA2CCR2 = 340; //left
    delay_s(3);
    TA2CCR1 = 425;
    delay_s(0.05);

    //P6OUT = 0xA;
    TA2CCR1 = 93;
    TA2CCR2 = 93;
    delay_s(5);
}

void th(void)
{
    TA2CCR1 = 250;
    TA2CCR2 = 250;
    delay_s(4.05);

    P6OUT = 0x9;
    TA2CCR1 = 250; //right
    TA2CCR2 = 0; //left
    delay_s(2.2);

    P6OUT = 0xA;
    TA2CCR1 = 250;
    TA2CCR2 = 250;
    delay_s(3.75);
}

void tf(void)
{
    TA2CCR1 = 160;
    TA2CCR2 = 160;
    delay_s(4.05);

    //P6OUT = 0x9;
    TA2CCR1 = 50; //right
    TA2CCR2 = 425; //left
    delay_s(2.2);

    P6OUT = 0xA;
    TA2CCR1 = 120;
    TA2CCR2 = 120;
    delay_s(3.75);
}

int main(void)
{
    WDTCTL = WDTPW + WDTHOLD;
    P6DIR |= BIT0 + BIT1 + BIT2 + BIT3;
    P6OUT = 0xA;
    P1DIR |= BIT0;
    P8DIR |= BIT1;
    P8OUT &= ~BIT1;
    P1OUT &= ~BIT0;

    P2DIR |= BIT4 + BIT5;
    P2SEL |= BIT4 + BIT5;
    TA2CCR0 = 512 - 1;
    TA2CCTL1 = OUTMOD_7;
    TA2CCTL2 = OUTMOD_7;
    TA2CTL = TASSEL_2 + MC_1 + TACLR;
    TA2CCR1 = 450;
    TA2CCR2 = 450;
    delay_s(0.02);
    hf();
    TA2CCR1 = 0;
    TA2CCR2 = 0;
    P1OUT |= BIT0;
    P8OUT |= BIT1;
    delay_s(3);
    P8OUT &= ~BIT1;

    /*while(1){
     switch(P2IN&0x1C){
     case 0x18: case 0x10: TA0CCR3 = 250; = 50;delay_s(0.6);break;
     case 0xC: case 0x1C: TA0CCR3 = 0; = 0;P4OUT = 0x00;P8OUT |= BIT1;delay_s(1);P8OUT &= ~BIT1;break;
     case 0x4: TA0CCR3 = 100; = 200;break;
     default: TA0CCR3 = 150; = 150;break;
     }
     }*/
}
